[CmdletBinding()]
param(
    [string]$MorrowindDirectory = 'C:\Games\Morrowind',
    [string]$FixturePath = 'C:\Games\Morrowind\Data Files\ncg.omwaddon',
    [string]$CSSEDllPath = 'build\Debug\CSSE.dll',
    [int]$TimeoutSeconds = 60
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
Import-Module (Join-Path $PSScriptRoot 'HarnessProtocol.psm1') -Force
Import-Module (Join-Path $PSScriptRoot 'OpenMWAddon.psm1') -Force
$root = [IO.Path]::GetFullPath($MorrowindDirectory).TrimEnd('\')
$dataDirectory = Join-Path $root 'Data Files'
$executable = Join-Path $root 'TES Construction Set.exe'
$runId = New-OwchRunId
$runDirectory = Join-Path $dataDirectory ('MWSE\tmp\openmw-compat-csse-harness\' + $runId)
$nativeResultPath = Join-Path $runDirectory 'native-result.json'
$resultPath = Join-Path $runDirectory 'result.json'
$backupRoot = Join-Path ([IO.Path]::GetTempPath()) ('owca-csse-harness-' + $runId)
$staged = [Collections.Generic.List[object]]::new()
$process = $null
$failure = $null
$exitCode = $null
$nativeResult = $null
$previousTestResultEnvironment = $env:MWSE_CSSE_OPENMW_ADDON_TEST_RESULT
$configTarget = Join-Path $root 'csse.toml'
$csseTarget = Join-Path $root 'CSSE.dll'
$originalConfigHash = if (Test-Path -LiteralPath $configTarget) { (Get-FileHash -LiteralPath $configTarget -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }
$originalCSSEHash = if (Test-Path -LiteralPath $csseTarget) { (Get-FileHash -LiteralPath $csseTarget -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }

function Stage-HarnessFile([string]$Source, [string]$Target) {
    if (-not (Test-Path -LiteralPath $Source -PathType Leaf)) { throw "Staging source was not found: $Source" }
    $entry = [pscustomobject]@{ target = $Target; existed = Test-Path -LiteralPath $Target; backup = Join-Path $backupRoot ([Guid]::NewGuid().ToString('N')) }
    [IO.Directory]::CreateDirectory($backupRoot) | Out-Null
    if ($entry.existed) { Copy-Item -LiteralPath $Target -Destination $entry.backup -Force }
    $staged.Add($entry)
    Copy-Item -LiteralPath $Source -Destination $Target -Force
}

try {
    if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) { throw "Construction Set executable was not found: $executable" }
    if (@(Get-Process -Name 'TES Construction Set' -ErrorAction SilentlyContinue).Count) { throw 'The Construction Set is already running.' }
    [IO.Directory]::CreateDirectory($runDirectory) | Out-Null
    $inspection = Read-OpenMWAddonContent $FixturePath
    if ($inspection.classification -notin @('direct-load', 'transformable')) { throw "Fixture is not loadable: $($inspection.classification)" }
    $native = New-OpenMWAddonNativeFile $inspection (Join-Path $runDirectory 'cache')
    Stage-HarnessFile $native.nativePath (Join-Path $dataDirectory $native.aliasName)
    Stage-HarnessFile ([IO.Path]::GetFullPath($CSSEDllPath)) $csseTarget
    $pdbPath = [IO.Path]::ChangeExtension([IO.Path]::GetFullPath($CSSEDllPath), '.pdb')
    if (Test-Path -LiteralPath $pdbPath) { Stage-HarnessFile $pdbPath (Join-Path $root 'CSSE.pdb') }

    $configSource = Join-Path $runDirectory 'csse-test.toml'
    $existingConfig = if (Test-Path -LiteralPath $configTarget) { [IO.File]::ReadAllText($configTarget) } else { "title = `"Construction Set Extender`"`r`nenabled = true`r`n" }
    $newline = if ($existingConfig.Contains("`r`n")) { "`r`n" } else { "`n" }
    $quickStart = @(
        '[quickstart]',
        'enabled = true',
        ('active_file = "{0}"' -f $native.aliasName),
        'load_cell = false',
        'position = [0.0,0.0,0.0]',
        ('data_files = ["Morrowind.esm","Tribunal.esm","Bloodmoon.esm","{0}"]' -f $native.aliasName),
        'orientation = [0.0,0.0,0.0]',
        'cell = ""',
        ''
    ) -join $newline
    $quickStartPattern = '(?ms)^\[quickstart\]\r?\n.*?(?=^\[|\z)'
    $testConfig = if ([regex]::IsMatch($existingConfig, $quickStartPattern)) { [regex]::Replace($existingConfig, $quickStartPattern, $quickStart) } else { $existingConfig + $newline + $quickStart }
    [IO.File]::WriteAllText($configSource, $testConfig, [Text.UTF8Encoding]::new($false))
    Stage-HarnessFile $configSource $configTarget

    $env:MWSE_CSSE_OPENMW_ADDON_TEST_RESULT = $nativeResultPath
    $process = Start-Process -FilePath $executable -WorkingDirectory $root -WindowStyle Hidden -PassThru
    $deadline = [DateTime]::UtcNow.AddSeconds($TimeoutSeconds)
    while (-not $process.HasExited -and [DateTime]::UtcNow -lt $deadline) { Start-Sleep -Milliseconds 100 }
    if (-not $process.HasExited) { throw "Construction Set did not finish the addon test within $TimeoutSeconds seconds." }
    $process.WaitForExit()
    $exitCode = $process.ExitCode
    if (-not (Test-Path -LiteralPath $nativeResultPath -PathType Leaf)) { throw 'CSSE did not write the native addon result.' }
    $nativeResult = Get-Content -LiteralPath $nativeResultPath -Raw | ConvertFrom-Json
    if ($exitCode -ne 0 -or $nativeResult.passed -ne $true -or $nativeResult.addonSource -cne $inspection.source.filename) {
        throw "CSSE native addon load failed (exit=$exitCode, source=$($nativeResult.addonSource))."
    }
}
catch {
    $failure = $_.Exception.ToString()
}
finally {
    if ($process -and -not $process.HasExited) { Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue; $process.WaitForExit(5000) | Out-Null }
    if (Test-Path -LiteralPath (Join-Path $root 'CSSE.log')) { Copy-Item -LiteralPath (Join-Path $root 'CSSE.log') -Destination (Join-Path $runDirectory 'CSSE.log') -Force }
    $restoreFailure = $null
    for ($index = $staged.Count - 1; $index -ge 0; $index--) {
        $entry = $staged[$index]
        for ($attempt = 1; $attempt -le 20; $attempt++) {
            try {
                if ($entry.existed) { Copy-Item -LiteralPath $entry.backup -Destination $entry.target -Force }
                elseif (Test-Path -LiteralPath $entry.target) { Remove-Item -LiteralPath $entry.target -Force }
                break
            }
            catch {
                if ($attempt -eq 20) { $restoreFailure = $_.Exception.ToString() }
                else { Start-Sleep -Milliseconds 100 }
            }
        }
    }
    if ($null -eq $previousTestResultEnvironment) { Remove-Item Env:MWSE_CSSE_OPENMW_ADDON_TEST_RESULT -ErrorAction SilentlyContinue } else { $env:MWSE_CSSE_OPENMW_ADDON_TEST_RESULT = $previousTestResultEnvironment }
    if (Test-Path -LiteralPath $backupRoot) { Remove-Item -LiteralPath $backupRoot -Recurse -Force }
    $remainingAliases = @(Get-ChildItem -LiteralPath $dataDirectory -File -Filter 'ncg.omwaddon-*.esp')
    $restoredConfigHash = if (Test-Path -LiteralPath $configTarget) { (Get-FileHash -LiteralPath $configTarget -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }
    $restoredCSSEHash = if (Test-Path -LiteralPath $csseTarget) { (Get-FileHash -LiteralPath $csseTarget -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }
    $configRestored = $restoredConfigHash -eq $originalConfigHash
    $csseRestored = $restoredCSSEHash -eq $originalCSSEHash
    if ($null -eq $failure -and $restoreFailure) { $failure = $restoreFailure }
    if ($null -eq $failure -and (-not $configRestored -or -not $csseRestored -or $remainingAliases.Count -ne 0)) {
        $failure = 'CSSE harness did not restore all staged state.'
    }
    $result = [ordered]@{
        schemaVersion = 1; runId = $runId; passed = $null -eq $failure -and $remainingAliases.Count -eq 0
        failure = $failure; processExitCode = $exitCode; nativeResult = $nativeResult
        restored = [ordered]@{
            temporaryAliasesRemaining = $remainingAliases.Count
            csseConfigRestored = $configRestored; originalConfigSha256 = $originalConfigHash; restoredConfigSha256 = $restoredConfigHash
            csseBinaryRestored = $csseRestored; originalCSSESha256 = $originalCSSEHash; restoredCSSESha256 = $restoredCSSEHash
        }
        artifacts = [ordered]@{ runDirectory = $runDirectory; result = $resultPath; nativeResult = $nativeResultPath; csseLog = (Join-Path $runDirectory 'CSSE.log') }
    }
    Write-OwchAtomicJson -Path $resultPath -Value $result
    $result | ConvertTo-Json -Depth 12
}

if ($failure) { exit 1 }
