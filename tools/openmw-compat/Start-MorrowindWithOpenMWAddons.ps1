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
$iniPath = Join-Path $root 'Morrowind.ini'
$executable = Join-Path $root 'Morrowind.exe'
if (-not $ConfigurationPath) { $ConfigurationPath = Join-Path $dataDirectory 'MWSE\config\openmw-addon-loader.json' }
$sessionId = New-OwchRunId
$sessionDirectory = Join-Path $dataDirectory ('MWSE\tmp\openmw-addon-sessions\' + $sessionId)
$cacheRoot = Join-Path $dataDirectory 'MWSE\tmp\openmw-addon-cache'
$backupRoot = Join-Path ([IO.Path]::GetTempPath()) ('owca-session-' + $sessionId)
$staged = [Collections.Generic.List[object]]::new()
$processExitCode = $null
$failure = $null
$originalIniHash = $null
$restoredIniHash = $null
$prepared = [Collections.Generic.List[object]]::new()

function Stage-SessionFile([string]$Source, [string]$Target) {
    $entry = [pscustomobject]@{ target = $Target; existed = Test-Path -LiteralPath $Target; backup = Join-Path $backupRoot ([Guid]::NewGuid().ToString('N')) }
    [IO.Directory]::CreateDirectory($backupRoot) | Out-Null
    if ($entry.existed) { Copy-Item -LiteralPath $Target -Destination $entry.backup -Force }
    $staged.Add($entry)
    Copy-Item -LiteralPath $Source -Destination $Target -Force
}

function Set-SessionGameFiles([string[]]$Filenames) {
    $raw = [IO.File]::ReadAllText($iniPath)
    $matches = [regex]::Matches($raw, '(?m)^GameFile(?<index>\d+)=(?<name>[^\r\n]*)')
    $next = if ($matches.Count) { 1 + ($matches | ForEach-Object { [int]$_.Groups['index'].Value } | Measure-Object -Maximum).Maximum } else { 0 }
    $newline = if ($raw.Contains("`r`n")) { "`r`n" } else { "`n" }
    $section = [regex]::Match($raw, '(?ms)(?<header>^\[Game Files\]\r?\n)(?<body>.*?)(?=^\[|\z)')
    if (-not $section.Success) { throw 'Morrowind.ini does not contain a [Game Files] section.' }
    $body = $section.Groups['body'].Value
    if ($body.Length -and -not $body.EndsWith("`n")) { $body += $newline }
    foreach ($filename in $Filenames) { $body += "GameFile$next=$filename$newline"; $next++ }
    $backup = Join-Path $backupRoot 'Morrowind.ini'
    Copy-Item -LiteralPath $iniPath -Destination $backup -Force
    $staged.Add([pscustomobject]@{ target = $iniPath; existed = $true; backup = $backup })
    $updated = $raw.Substring(0, $section.Index) + $section.Groups['header'].Value + $body + $raw.Substring($section.Index + $section.Length)
    [IO.File]::WriteAllText($iniPath, $updated, [Text.Encoding]::GetEncoding(1252))
}

try {
    if (-not (Test-Path -LiteralPath $iniPath -PathType Leaf)) { throw "Morrowind.ini was not found: $iniPath" }
    if (-not $PrepareOnly -and -not (Test-Path -LiteralPath $executable -PathType Leaf)) { throw "Morrowind.exe was not found: $executable" }
    if (@(Get-Process -Name Morrowind -ErrorAction SilentlyContinue).Count) { throw 'Morrowind is already running.' }
    [IO.Directory]::CreateDirectory($sessionDirectory) | Out-Null
    $originalIniHash = (Get-FileHash -LiteralPath $iniPath -Algorithm SHA256).Hash.ToLowerInvariant()
    $discovery = Get-OpenMWAddonDiscovery -DataDirectory $dataDirectory -EnabledAddonPaths $AddonPath -ConfigurationPath $ConfigurationPath
    if ($discovery.orderedAddons.Count -eq 0) { throw "No addons are enabled. Pass -AddonPath or enable entries in '$ConfigurationPath'." }
    $aliasMap = @{}
    foreach ($inspection in $discovery.orderedAddons) {
        $native = New-OpenMWAddonNativeFile $inspection $cacheRoot $aliasMap
        $aliasMap[$inspection.source.filename] = $native.aliasName
        Stage-SessionFile $native.nativePath (Join-Path $dataDirectory $native.aliasName)
        $prepared.Add($native)
    }
    Set-SessionGameFiles @($prepared.aliasName)
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
    if (Test-Path -LiteralPath $iniPath) { $restoredIniHash = (Get-FileHash -LiteralPath $iniPath -Algorithm SHA256).Hash.ToLowerInvariant() }
    if (Test-Path -LiteralPath $backupRoot) { Remove-Item -LiteralPath $backupRoot -Recurse -Force }
    $result = [ordered]@{
        schemaVersion = 1; sessionId = $sessionId; passed = $null -eq $failure -and $originalIniHash -eq $restoredIniHash
        prepareOnly = $PrepareOnly.IsPresent; failure = $failure; processExitCode = $processExitCode
        originalIniSha256 = $originalIniHash; restoredIniSha256 = $restoredIniHash; configurationPath = $ConfigurationPath
        addons = @($prepared | ForEach-Object { $_.report }); sessionDirectory = $sessionDirectory
    }
    Write-OwchAtomicJson -Path (Join-Path $sessionDirectory 'result.json') -Value $result
    $result | ConvertTo-Json -Depth 20
}

if ($failure) { exit 1 }
