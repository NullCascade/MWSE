[CmdletBinding()]
param(
    [string]$MorrowindDirectory = 'C:\Games\Morrowind',
    [string]$FixturePath = 'C:\Games\Morrowind\Data Files\ncg.omwaddon',
    [string]$CSSEDllPath = 'build\Debug\CSSE.dll'
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
Import-Module (Join-Path $PSScriptRoot '..\HarnessProtocol.psm1') -Force
Import-Module (Join-Path $PSScriptRoot '..\OpenMWAddon.psm1') -Force
$runId = New-OwchRunId
$runDirectory = Join-Path ([IO.Path]::GetFullPath($MorrowindDirectory)) ('Data Files\MWSE\tmp\openmw-compat-csse\' + $runId)
$cache = Join-Path $runDirectory 'cache'
[IO.Directory]::CreateDirectory($runDirectory) | Out-Null
$assertions = [Collections.Generic.List[object]]::new()

function Assert-CSSE([string]$Name, [bool]$Passed, $Actual, $Expected) {
    $assertions.Add([pscustomobject]@{ name = $Name; passed = $Passed; actual = $Actual; expected = $Expected })
    if (-not $Passed) { throw "CSSE addon assertion failed: $Name" }
}

try {
    $inspection = Read-OpenMWAddonContent $FixturePath
    Assert-CSSE 'fixture-direct-load-safe' ($inspection.classification -eq 'direct-load') $inspection.classification 'direct-load'
    Assert-CSSE 'fixture-record-inventory' ($inspection.recordCounts.GMST -eq 8 -and $inspection.recordCounts.SKIL -eq 27) $inspection.recordCounts @{ GMST = 8; SKIL = 27 }
    Assert-CSSE 'fixture-masters' ($inspection.masters.Count -eq 3) @($inspection.masters.name) @('Morrowind.esm', 'Tribunal.esm', 'Bloodmoon.esm')

    $native = New-OpenMWAddonNativeFile $inspection $cache
    $displayName = Get-OpenMWAddonAliasSourceName $native.aliasName
    Assert-CSSE 'identity-preserving-display-name' ($displayName -ceq $inspection.source.filename) $displayName $inspection.source.filename
    Assert-CSSE 'read-only-save-policy' ($native.report.csse.readOnly -eq $true -and $native.report.csse.saveEditsAs -eq '.esp') $native.report.csse @{ readOnly = $true; saveEditsAs = '.esp' }
    Assert-CSSE 'source-not-modified' ($native.report.source.sha256 -eq $native.report.native.sha256) $native.report.native.sha256 $native.report.source.sha256

    $dll = [IO.Path]::GetFullPath($CSSEDllPath)
    Assert-CSSE 'csse-dll-exists' (Test-Path -LiteralPath $dll -PathType Leaf) $dll 'existing CSSE.dll'
    $binaryText = [Text.Encoding]::ASCII.GetString([IO.File]::ReadAllBytes($dll))
    Assert-CSSE 'read-only-warning-compiled' ($binaryText.Contains('OpenMW Addon (Read-Only)')) 'compiled warning string' 'OpenMW Addon (Read-Only)'

    $result = [ordered]@{
        schemaVersion = 1; runId = $runId; passed = $true; fixture = $inspection.source
        nativeAlias = $native.aliasName; compatibilityReport = $native.reportPath; assertions = @($assertions)
    }
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'result.json') -Value $result
    $result | ConvertTo-Json -Depth 12
}
catch {
    $result = [ordered]@{ schemaVersion = 1; runId = $runId; passed = $false; failure = $_.Exception.ToString(); assertions = @($assertions) }
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'result.json') -Value $result
    $result | ConvertTo-Json -Depth 12
    exit 1
}
