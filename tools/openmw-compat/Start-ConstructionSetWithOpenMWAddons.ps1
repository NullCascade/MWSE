[CmdletBinding()]
param(
    [string]$MorrowindDirectory = 'C:\Games\Morrowind',
    [string[]]$AddonPath = @(),
    [string]$ConfigurationPath,
    [switch]$PrepareOnly
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
Import-Module (Join-Path $PSScriptRoot 'HarnessProtocol.psm1') -Force
Import-Module (Join-Path $PSScriptRoot 'OpenMWAddon.psm1') -Force
$root = [IO.Path]::GetFullPath($MorrowindDirectory).TrimEnd('\')
$dataDirectory = Join-Path $root 'Data Files'
$executable = Join-Path $root 'TES Construction Set.exe'
if (-not $ConfigurationPath) { $ConfigurationPath = Join-Path $dataDirectory 'MWSE\config\openmw-addon-loader.json' }
$sessionId = New-OwchRunId
$sessionDirectory = Join-Path $dataDirectory ('MWSE\tmp\openmw-addon-csse-sessions\' + $sessionId)
$cacheRoot = Join-Path $dataDirectory 'MWSE\tmp\openmw-addon-cache'
$backupRoot = Join-Path ([IO.Path]::GetTempPath()) ('owca-csse-session-' + $sessionId)
$staged = [Collections.Generic.List[object]]::new()
$prepared = [Collections.Generic.List[object]]::new()
$failure = $null
$processExitCode = $null

function Stage-SessionFile([string]$Source, [string]$Target) {
    $entry = [pscustomobject]@{ target = $Target; existed = Test-Path -LiteralPath $Target; backup = Join-Path $backupRoot ([Guid]::NewGuid().ToString('N')) }
    [IO.Directory]::CreateDirectory($backupRoot) | Out-Null
    if ($entry.existed) { Copy-Item -LiteralPath $Target -Destination $entry.backup -Force }
    $staged.Add($entry)
    Copy-Item -LiteralPath $Source -Destination $Target -Force
}

try {
    if (-not $PrepareOnly -and -not (Test-Path -LiteralPath $executable -PathType Leaf)) { throw "Construction Set executable was not found: $executable" }
    if (@(Get-Process -Name 'TES Construction Set' -ErrorAction SilentlyContinue).Count) { throw 'The Construction Set is already running.' }
    [IO.Directory]::CreateDirectory($sessionDirectory) | Out-Null
    $discovery = Get-OpenMWAddonDiscovery -DataDirectory $dataDirectory -EnabledAddonPaths $AddonPath -ConfigurationPath $ConfigurationPath
    if ($discovery.orderedAddons.Count -eq 0) { throw "No addons are enabled. Pass -AddonPath or enable entries in '$ConfigurationPath'." }
    $aliasMap = @{}
    foreach ($inspection in $discovery.orderedAddons) {
        $native = New-OpenMWAddonNativeFile $inspection $cacheRoot $aliasMap
        $aliasMap[$inspection.source.filename] = $native.aliasName
        Stage-SessionFile $native.nativePath (Join-Path $dataDirectory $native.aliasName)
        $prepared.Add($native)
    }
    Write-OwchAtomicJson -Path (Join-Path $sessionDirectory 'addon-plan.json') -Value ([ordered]@{
        schemaVersion = 1; sessionId = $sessionId; aliases = @($prepared | ForEach-Object { [ordered]@{
            source = $_.report.source.filename; nativeAlias = $_.aliasName; displayFilename = $_.report.csse.displayFilename
            readOnly = $_.report.csse.readOnly; saveEditsAs = $_.report.csse.saveEditsAs; report = $_.reportPath
        } })
    })
    if (-not $PrepareOnly) {
        $process = Start-Process -FilePath $executable -WorkingDirectory $root -PassThru
        $process.WaitForExit()
        $processExitCode = $process.ExitCode
    }
}
catch {
    $failure = $_.Exception.ToString()
}
finally {
    for ($index = $staged.Count - 1; $index -ge 0; $index--) {
        $entry = $staged[$index]
        if (Test-Path -LiteralPath $entry.target) { Remove-Item -LiteralPath $entry.target -Force }
        if ($entry.existed) { Copy-Item -LiteralPath $entry.backup -Destination $entry.target -Force }
    }
    if (Test-Path -LiteralPath $backupRoot) { Remove-Item -LiteralPath $backupRoot -Recurse -Force }
    $remainingAliases = @($prepared | Where-Object { Test-Path -LiteralPath (Join-Path $dataDirectory $_.aliasName) } | ForEach-Object aliasName)
    $result = [ordered]@{
        schemaVersion = 1; sessionId = $sessionId; passed = $null -eq $failure -and $remainingAliases.Count -eq 0
        prepareOnly = $PrepareOnly.IsPresent; failure = $failure; processExitCode = $processExitCode
        aliasesRestored = $remainingAliases.Count -eq 0; remainingAliases = $remainingAliases
        addons = @($prepared | ForEach-Object { $_.report }); sessionDirectory = $sessionDirectory
    }
    Write-OwchAtomicJson -Path (Join-Path $sessionDirectory 'result.json') -Value $result
    $result | ConvertTo-Json -Depth 20
}

if ($failure) { exit 1 }
