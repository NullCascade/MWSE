[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
Import-Module (Join-Path $PSScriptRoot '..\OpenMWAddon.psm1') -Force
$root = Join-Path ([IO.Path]::GetTempPath()) ('owca-tests-' + [Guid]::NewGuid().ToString('N'))
[IO.Directory]::CreateDirectory($root) | Out-Null

function Join-Bytes([object[]]$Parts) {
    $stream = [IO.MemoryStream]::new()
    try {
        foreach ($part in $Parts) {
            $bytes = [byte[]]$part
            $stream.Write($bytes, 0, $bytes.Length)
        }
        return $stream.ToArray()
    }
    finally { $stream.Dispose() }
}

function New-Subrecord([string]$Name, [byte[]]$Data) {
    return Join-Bytes @([Text.Encoding]::ASCII.GetBytes($Name), [BitConverter]::GetBytes([uint32]$Data.Length), $Data)
}

function New-Record([string]$Name, [byte[]]$Data, [uint32]$Flags = 0) {
    return Join-Bytes @([Text.Encoding]::ASCII.GetBytes($Name), [BitConverter]::GetBytes([uint32]$Data.Length), [BitConverter]::GetBytes([uint32]0), [BitConverter]::GetBytes($Flags), $Data)
}

function New-Addon([string]$Path, [string[]]$Masters = @(), [switch]$Format1, [string]$RecordName = 'GMST') {
    $hedr = [byte[]]::new(300)
    [BitConverter]::GetBytes([single]1.3).CopyTo($hedr, 0)
    [BitConverter]::GetBytes([uint32]1).CopyTo($hedr, 296)
    $headerParts = [Collections.Generic.List[object]]::new()
    $headerParts.Add((New-Subrecord 'HEDR' $hedr))
    if ($Format1) { $headerParts.Add((New-Subrecord 'FORM' ([BitConverter]::GetBytes([uint32]1)))) }
    foreach ($master in $Masters) {
        $headerParts.Add((New-Subrecord 'MAST' ([Text.Encoding]::GetEncoding(1252).GetBytes($master + [char]0))))
        $headerParts.Add((New-Subrecord 'DATA' ([BitConverter]::GetBytes([uint64]1234))))
    }
    $payload = New-Subrecord 'NAME' ([Text.Encoding]::ASCII.GetBytes("fixture`0"))
    $bytes = Join-Bytes @((New-Record 'TES3' (Join-Bytes $headerParts)), (New-Record $RecordName $payload))
    [IO.File]::WriteAllBytes($Path, $bytes)
}

function Assert-True([bool]$Condition, [string]$Message) {
    if (-not $Condition) { throw $Message }
}

$passed = [Collections.Generic.List[string]]::new()
try {
    $format0 = Join-Path $root 'format0.omwaddon'
    New-Addon $format0 -Masters @('Morrowind.esm')
    $inspection0 = Read-OpenMWAddonContent $format0
    Assert-True ($inspection0.classification -eq 'direct-load') 'Format 0 was not classified direct-load.'
    Assert-True ($inspection0.masters[0].name -eq 'Morrowind.esm') 'Format 0 master was not read.'
    $passed.Add('format-0-direct-load')

    $format1 = Join-Path $root 'format1.omwaddon'
    New-Addon $format1 -Masters @('Morrowind.esm') -Format1
    $inspection1 = Read-OpenMWAddonContent $format1
    Assert-True ($inspection1.classification -eq 'transformable' -and $inspection1.formatVersion -eq 1) 'Format 1 FORM was not classified transformable.'
    $prepared = New-OpenMWAddonNativeFile $inspection1 (Join-Path $root 'cache')
    $native = Read-OpenMWAddonContent $prepared.nativePath
    Assert-True ($native.classification -eq 'direct-load' -and $native.formLocations.Count -eq 0) 'Format 1 alias did not remove FORM safely.'
    Assert-True ((Get-Item $format1).Length - (Get-Item $prepared.nativePath).Length -eq 12) 'FORM removal did not rewrite the expected byte count.'
    $passed.Add('format-1-form-transformation')

    $unsupportedFormat = Join-Path $root 'format2.omwaddon'
    $format2Bytes = [IO.File]::ReadAllBytes($format1)
    [BitConverter]::GetBytes([uint32]2).CopyTo($format2Bytes, $inspection1.formLocations[0].offset + 8)
    [IO.File]::WriteAllBytes($unsupportedFormat, $format2Bytes)
    Assert-True ((Read-OpenMWAddonContent $unsupportedFormat).classification -eq 'unsupported') 'Unsupported FORM version was not rejected.'
    $passed.Add('unsupported-format-version')

    $malformed = Join-Path $root 'malformed.omwaddon'
    $bad = [IO.File]::ReadAllBytes($format0)
    [BitConverter]::GetBytes([uint32]::MaxValue).CopyTo($bad, 4)
    [IO.File]::WriteAllBytes($malformed, $bad)
    $badInspection = Read-OpenMWAddonContent $malformed
    Assert-True ($badInspection.classification -eq 'malformed' -and $badInspection.errors[0].code -eq 'record_size_out_of_bounds') 'Malformed record size was not rejected.'
    $passed.Add('malformed-size-rejection')

    $malformedSubrecord = Join-Path $root 'malformed-subrecord.omwaddon'
    $badSubrecord = [IO.File]::ReadAllBytes($format0)
    [BitConverter]::GetBytes([uint32]::MaxValue).CopyTo($badSubrecord, 20)
    [IO.File]::WriteAllBytes($malformedSubrecord, $badSubrecord)
    Assert-True ((Read-OpenMWAddonContent $malformedSubrecord).errors[0].code -eq 'subrecord_size_out_of_bounds') 'Malformed subrecord size was not rejected.'
    $passed.Add('malformed-subrecord-size-rejection')

    $unknown = Join-Path $root 'unknown.omwaddon'
    New-Addon $unknown -RecordName 'ZZZZ'
    $unknownInspection = Read-OpenMWAddonContent $unknown
    Assert-True ($unknownInspection.classification -eq 'unsupported' -and $unknownInspection.unknownRecords[0] -eq 'ZZZZ') 'Unknown record was not inventoried and rejected.'
    $passed.Add('unknown-record-inventory')

    $openMWOnly = Join-Path $root 'openmw-only.omwaddon'
    New-Addon $openMWOnly -RecordName 'LUAL'
    $openMWOnlyInspection = Read-OpenMWAddonContent $openMWOnly
    Assert-True ($openMWOnlyInspection.classification -eq 'unsupported' -and $openMWOnlyInspection.openMWOnlyRecords[0] -eq 'LUAL') 'OpenMW-only record was not explicitly inventoried.'
    $passed.Add('openmw-only-record-inventory')

    $cacheA = New-OpenMWAddonNativeFile $inspection0 (Join-Path $root 'identity-cache')
    $secondPath = Join-Path $root 'same-content-different-path.omwaddon'
    Copy-Item -LiteralPath $format0 -Destination $secondPath
    $cacheB = New-OpenMWAddonNativeFile (Read-OpenMWAddonContent $secondPath) (Join-Path $root 'identity-cache')
    Assert-True ($cacheA.cacheKey -ne $cacheB.cacheKey) 'Cache identity did not include source path.'
    (Get-Item -LiteralPath $format0).LastWriteTimeUtc = (Get-Item -LiteralPath $format0).LastWriteTimeUtc.AddMinutes(1)
    $cacheC = New-OpenMWAddonNativeFile (Read-OpenMWAddonContent $format0) (Join-Path $root 'identity-cache')
    Assert-True ($cacheA.cacheKey -ne $cacheC.cacheKey) 'Cache identity did not include modification time.'
    $changedBytes = [IO.File]::ReadAllBytes($secondPath)
    $changedBytes[12] = 1
    [IO.File]::WriteAllBytes($secondPath, $changedBytes)
    $cacheD = New-OpenMWAddonNativeFile (Read-OpenMWAddonContent $secondPath) (Join-Path $root 'identity-cache')
    Assert-True ($cacheB.cacheKey -ne $cacheD.cacheKey) 'Cache identity did not include content hash.'
    $passed.Add('cache-source-identity')

    [IO.File]::WriteAllBytes((Join-Path $root 'Morrowind.esm'), [byte[]](0))
    $dependency = Join-Path $root 'dependency.omwaddon'
    New-Addon $dependency -Masters @('mOrRoWiNd.EsM', 'FORMAT0.OMWADDON')
    $dependencyInspection = Read-OpenMWAddonContent $dependency
    $resolved = Resolve-OpenMWAddonDependencies @($inspection0, $dependencyInspection) $root
    Assert-True ($resolved.orderedAddons.Count -eq 2 -and $resolved.orderedAddons[0].source.filename -eq 'format0.omwaddon') 'Case-insensitive dependency chain was not topologically ordered.'
    $passed.Add('case-insensitive-dependency-chain')

    $cycleA = Join-Path $root 'cycle-a.omwaddon'
    $cycleB = Join-Path $root 'cycle-b.omwaddon'
    New-Addon $cycleA -Masters @('cycle-b.omwaddon')
    New-Addon $cycleB -Masters @('CYCLE-A.OMWADDON')
    $cycleRejected = $false
    try { Resolve-OpenMWAddonDependencies @((Read-OpenMWAddonContent $cycleA), (Read-OpenMWAddonContent $cycleB)) $root | Out-Null } catch { $cycleRejected = $_.Exception.Message -like '*cycle*' }
    Assert-True $cycleRejected 'Dependency cycle was not diagnosed.'
    $passed.Add('dependency-cycle-diagnostic')

    $missingPath = Join-Path $root 'missing.omwaddon'
    New-Addon $missingPath -Masters @('absent.esm')
    $missingRejected = $false
    try { Resolve-OpenMWAddonDependencies @((Read-OpenMWAddonContent $missingPath)) $root | Out-Null } catch { $missingRejected = $_.Exception.Message -like '*Missing addon masters*' }
    Assert-True $missingRejected 'Missing master was not diagnosed.'
    $passed.Add('missing-master-diagnostic')

    $duplicateDirectory = Join-Path $root 'duplicate'
    [IO.Directory]::CreateDirectory($duplicateDirectory) | Out-Null
    $duplicatePath = Join-Path $duplicateDirectory 'FORMAT0.OMWADDON'
    New-Addon $duplicatePath
    $duplicateRejected = $false
    try { Resolve-OpenMWAddonDependencies @($inspection0, (Read-OpenMWAddonContent $duplicatePath)) $root | Out-Null } catch { $duplicateRejected = $_.Exception.Message -like '*Duplicate addon filenames*' }
    Assert-True $duplicateRejected 'Case-insensitive duplicate filename was not diagnosed.'
    $passed.Add('duplicate-filename-diagnostic')

    [pscustomobject]@{ passed = $true; tests = @($passed) } | ConvertTo-Json -Depth 5
}
finally {
    Remove-Item -LiteralPath $root -Recurse -Force -ErrorAction SilentlyContinue
}
