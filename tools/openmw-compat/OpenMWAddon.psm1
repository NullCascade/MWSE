Set-StrictMode -Version Latest

$script:ClassicRecordNames = @(
    'TES3', 'GMST', 'GLOB', 'CLAS', 'FACT', 'RACE', 'SOUN', 'SKIL', 'MGEF', 'SCPT',
    'REGN', 'BSGN', 'LTEX', 'STAT', 'DOOR', 'MISC', 'WEAP', 'CONT', 'SPEL', 'CREA',
    'BODY', 'LIGH', 'ENCH', 'NPC_', 'ARMO', 'CLOT', 'REPA', 'ACTI', 'APPA', 'LOCK',
    'PROB', 'INGR', 'BOOK', 'ALCH', 'LEVI', 'LEVC', 'CELL', 'LAND', 'PGRD', 'SNDG',
    'DIAL', 'INFO', 'SSCR', 'CNTC', 'CREC', 'NPCC'
)

# Records implemented by OpenMW extensions, but not by the original TES3 loader.
$script:OpenMWOnlyRecordNames = @('LUAL', 'FILT', 'DBGP', 'SELG', 'ATTR', 'LUAM', 'RAND')

function Get-UInt32([byte[]]$Bytes, [int]$Offset) {
    return [BitConverter]::ToUInt32($Bytes, $Offset)
}

function Get-Tag([byte[]]$Bytes, [int]$Offset) {
    return [Text.Encoding]::ASCII.GetString($Bytes, $Offset, 4)
}

function Get-ZeroTerminatedString([byte[]]$Bytes, [int]$Offset, [int]$Length) {
    $value = [Text.Encoding]::GetEncoding(1252).GetString($Bytes, $Offset, $Length)
    return $value.TrimEnd([char]0)
}

function New-InspectionError([string]$Code, [string]$Message, [long]$Offset = -1) {
    return [pscustomobject][ordered]@{ code = $Code; message = $Message; offset = $Offset }
}

function Read-OpenMWAddonContent {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)][string]$Path,
        [long]$MaximumBytes = 268435456
    )

    $resolved = [IO.Path]::GetFullPath($Path)
    $errors = [Collections.Generic.List[object]]::new()
    $records = [Collections.Generic.List[object]]::new()
    $masters = [Collections.Generic.List[object]]::new()
    $unknownRecords = [Collections.Generic.List[string]]::new()
    $openMWOnlyRecords = [Collections.Generic.List[string]]::new()
    $formLocations = [Collections.Generic.List[object]]::new()
    $version = $null
    $formatVersion = 0
    $headerRecordCount = $null
    $sourceHash = $null
    $sourceLength = $null
    $sourceModifiedUtc = $null

    if (-not (Test-Path -LiteralPath $resolved -PathType Leaf)) {
        $errors.Add((New-InspectionError 'file_not_found' "Addon file was not found: $resolved"))
    }
    else {
        $file = Get-Item -LiteralPath $resolved
        $sourceLength = $file.Length
        $sourceModifiedUtc = $file.LastWriteTimeUtc.ToString('o')
        if ($file.Length -gt $MaximumBytes) {
            $errors.Add((New-InspectionError 'file_too_large' "Addon is $($file.Length) bytes; limit is $MaximumBytes bytes."))
        }
        elseif ($file.Length -lt 16) {
            $errors.Add((New-InspectionError 'truncated_record_header' 'File is shorter than one TES3 record header.' 0))
        }
        else {
            $bytes = [IO.File]::ReadAllBytes($resolved)
            $sourceHash = (Get-FileHash -LiteralPath $resolved -Algorithm SHA256).Hash.ToLowerInvariant()
            [long]$offset = 0
            $recordIndex = 0
            while ($offset -lt $bytes.LongLength -and $errors.Count -eq 0) {
                if ($bytes.LongLength - $offset -lt 16) {
                    $errors.Add((New-InspectionError 'truncated_record_header' 'A top-level record header is truncated.' $offset))
                    break
                }
                $name = Get-Tag $bytes ([int]$offset)
                [uint64]$size = Get-UInt32 $bytes ([int]$offset + 4)
                [uint64]$dataOffset = [uint64]$offset + 16
                [uint64]$end = $dataOffset + $size
                if ($end -lt $dataOffset -or $end -gt [uint64]$bytes.LongLength) {
                    $errors.Add((New-InspectionError 'record_size_out_of_bounds' "Record '$name' extends beyond the file." $offset))
                    break
                }
                $flags = Get-UInt32 $bytes ([int]$offset + 12)
                $records.Add([pscustomobject][ordered]@{
                    index = $recordIndex; name = $name; offset = $offset; dataOffset = [long]$dataOffset
                    size = [long]$size; flags = ('0x{0:X8}' -f $flags)
                })

                if ($recordIndex -eq 0 -and $name -ne 'TES3') {
                    $errors.Add((New-InspectionError 'missing_tes3_header' "First record is '$name', not TES3." $offset))
                    break
                }
                if ($recordIndex -gt 0 -and $name -eq 'TES3') {
                    $errors.Add((New-InspectionError 'duplicate_tes3_header' 'TES3 header must only appear as the first record.' $offset))
                    break
                }

                if ($script:ClassicRecordNames -notcontains $name) {
                    if ($script:OpenMWOnlyRecordNames -contains $name) {
                        if (-not $openMWOnlyRecords.Contains($name)) { $openMWOnlyRecords.Add($name) }
                    }
                    elseif (-not $unknownRecords.Contains($name)) { $unknownRecords.Add($name) }
                }

                if ($name -eq 'TES3') {
                    [uint64]$subOffset = $dataOffset
                    $pendingMaster = $null
                    $hedrCount = 0
                    while ($subOffset -lt $end -and $errors.Count -eq 0) {
                        if ($end - $subOffset -lt 8) {
                            $errors.Add((New-InspectionError 'truncated_subrecord_header' 'TES3 subrecord header is truncated.' ([long]$subOffset)))
                            break
                        }
                        $subName = Get-Tag $bytes ([int]$subOffset)
                        [uint64]$subSize = Get-UInt32 $bytes ([int]$subOffset + 4)
                        [uint64]$subData = $subOffset + 8
                        [uint64]$subEnd = $subData + $subSize
                        if ($subEnd -lt $subData -or $subEnd -gt $end) {
                            $errors.Add((New-InspectionError 'subrecord_size_out_of_bounds' "TES3 subrecord '$subName' extends beyond its record." ([long]$subOffset)))
                            break
                        }
                        switch ($subName) {
                            'HEDR' {
                                $hedrCount++
                                if ($hedrCount -gt 1) { $errors.Add((New-InspectionError 'duplicate_hedr' 'TES3 contains more than one HEDR.' ([long]$subOffset))); break }
                                if ($subSize -ne 300) { $errors.Add((New-InspectionError 'invalid_hedr_size' "HEDR size is $subSize, expected 300." ([long]$subOffset))); break }
                                $version = [BitConverter]::ToSingle($bytes, [int]$subData)
                                $headerRecordCount = Get-UInt32 $bytes ([int]$subData + 296)
                            }
                            'FORM' {
                                if ($subSize -ne 4) { $errors.Add((New-InspectionError 'invalid_form_size' "FORM size is $subSize, expected 4." ([long]$subOffset))); break }
                                if ($formLocations.Count -gt 0) { $errors.Add((New-InspectionError 'duplicate_form' 'TES3 contains more than one FORM.' ([long]$subOffset))); break }
                                $formatVersion = Get-UInt32 $bytes ([int]$subData)
                                $formLocations.Add([pscustomobject]@{ offset = [long]$subOffset; size = [long](8 + $subSize) })
                            }
                            'MAST' {
                                if ($null -ne $pendingMaster) { $errors.Add((New-InspectionError 'master_missing_data' "Master '$pendingMaster' is not followed by DATA." ([long]$subOffset))); break }
                                if ($subSize -eq 0 -or $subSize -gt 260) { $errors.Add((New-InspectionError 'invalid_master_name_size' "MAST size $subSize is invalid." ([long]$subOffset))); break }
                                $pendingMaster = Get-ZeroTerminatedString $bytes ([int]$subData) ([int]$subSize)
                                if ([string]::IsNullOrWhiteSpace($pendingMaster)) { $errors.Add((New-InspectionError 'empty_master_name' 'MAST name is empty.' ([long]$subOffset))); break }
                            }
                            'DATA' {
                                if ($null -ne $pendingMaster) {
                                    if ($subSize -ne 8) { $errors.Add((New-InspectionError 'invalid_master_data_size' "DATA for master '$pendingMaster' is $subSize bytes, expected 8." ([long]$subOffset))); break }
                                    $masters.Add([pscustomobject][ordered]@{ name = $pendingMaster; declaredSize = [BitConverter]::ToUInt64($bytes, [int]$subData) })
                                    $pendingMaster = $null
                                }
                            }
                        }
                        $subOffset = $subEnd
                    }
                    if ($errors.Count -eq 0 -and $null -ne $pendingMaster) {
                        $errors.Add((New-InspectionError 'master_missing_data' "Master '$pendingMaster' has no following DATA." ([long]$end)))
                    }
                    if ($errors.Count -eq 0 -and $hedrCount -ne 1) {
                        $errors.Add((New-InspectionError 'missing_hedr' 'TES3 must contain exactly one HEDR.' ([long]$dataOffset)))
                    }
                }
                $offset = [long]$end
                $recordIndex++
            }
        }
    }

    $classification = 'malformed'
    $reasons = [Collections.Generic.List[string]]::new()
    if ($errors.Count -eq 0) {
        if ([Math]::Abs([double]$version - 1.2) -gt 0.0001 -and [Math]::Abs([double]$version - 1.3) -gt 0.0001) {
            $classification = 'unsupported'
            $reasons.Add("Unsupported TES3 HEDR version: $version")
        }
        elseif ($formatVersion -notin @(0, 1)) {
            $classification = 'unsupported'
            $reasons.Add("Unsupported OpenMW format version: $formatVersion")
        }
        elseif ($openMWOnlyRecords.Count -gt 0 -or $unknownRecords.Count -gt 0) {
            $classification = 'unsupported'
            if ($openMWOnlyRecords.Count -gt 0) { $reasons.Add('OpenMW-only records cannot be omitted safely: ' + ($openMWOnlyRecords -join ', ')) }
            if ($unknownRecords.Count -gt 0) { $reasons.Add('Unknown top-level records cannot be proven safe: ' + ($unknownRecords -join ', ')) }
        }
        elseif ($formatVersion -eq 0) { $classification = 'direct-load' }
        else { $classification = 'transformable' }
    }

    $counts = [ordered]@{}
    foreach ($record in $records) {
        if (-not $counts.Contains($record.name)) { $counts[$record.name] = 0 }
        $counts[$record.name]++
    }

    return [pscustomobject][ordered]@{
        schemaVersion = 1
        source = [ordered]@{ path = $resolved; filename = [IO.Path]::GetFileName($resolved); size = $sourceLength; modifiedUtc = $sourceModifiedUtc; sha256 = $sourceHash }
        classification = $classification
        reasons = @($reasons)
        tes3Version = $version
        formatVersion = $formatVersion
        declaredRecordCount = $headerRecordCount
        actualContentRecordCount = [Math]::Max(0, $records.Count - 1)
        masters = @($masters)
        records = @($records)
        recordCounts = [pscustomobject]$counts
        recordsWithFlags = @($records | Where-Object flags -ne '0x00000000')
        openMWOnlyRecords = @($openMWOnlyRecords)
        unknownRecords = @($unknownRecords)
        formLocations = @($formLocations)
        errors = @($errors)
    }
}

function Resolve-OpenMWAddonDependencies {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)][object[]]$Inspections,
        [Parameter(Mandatory)][string]$DataDirectory
    )

    $nameMap = @{}
    $duplicates = [Collections.Generic.List[object]]::new()
    foreach ($inspection in $Inspections) {
        $key = $inspection.source.filename.ToLowerInvariant()
        if ($nameMap.ContainsKey($key)) {
            $duplicates.Add([pscustomobject]@{ filename = $inspection.source.filename; paths = @($nameMap[$key].source.path, $inspection.source.path) })
        }
        else { $nameMap[$key] = $inspection }
    }
    if ($duplicates.Count -gt 0) { throw "Duplicate addon filenames were found: $($duplicates.filename -join ', ')" }

    $ordinary = @{}
    foreach ($file in @(Get-ChildItem -LiteralPath $DataDirectory -File)) {
        if ($file.Extension -in @('.esm', '.esp')) { $ordinary[$file.Name.ToLowerInvariant()] = $file.FullName }
    }
    $missing = [Collections.Generic.List[object]]::new()
    foreach ($inspection in $Inspections) {
        foreach ($master in $inspection.masters) {
            $key = $master.name.ToLowerInvariant()
            if (-not $ordinary.ContainsKey($key) -and -not $nameMap.ContainsKey($key)) {
                $missing.Add([pscustomobject]@{ addon = $inspection.source.filename; master = $master.name })
            }
        }
    }
    if ($missing.Count -gt 0) { throw "Missing addon masters: $(@($missing | ForEach-Object { '$($_.addon) -> $($_.master)' }) -join '; ')" }

    $ordered = [Collections.Generic.List[object]]::new()
    $visiting = @{}
    $visited = @{}
    function Visit-Addon($inspection, [string[]]$chain) {
        $key = $inspection.source.filename.ToLowerInvariant()
        if ($visiting[$key]) { throw 'Addon dependency cycle: ' + (($chain + $inspection.source.filename) -join ' -> ') }
        if ($visited[$key]) { return }
        $visiting[$key] = $true
        foreach ($master in $inspection.masters) {
            $masterKey = $master.name.ToLowerInvariant()
            if ($nameMap.ContainsKey($masterKey)) { Visit-Addon $nameMap[$masterKey] ($chain + $inspection.source.filename) }
        }
        $visiting.Remove($key)
        $visited[$key] = $true
        $ordered.Add($inspection)
    }
    foreach ($inspection in $Inspections) { Visit-Addon $inspection @() }

    return [pscustomobject][ordered]@{ orderedAddons = @($ordered); ordinaryFiles = [pscustomobject]$ordinary; missingMasters = @(); duplicates = @() }
}

function Get-OpenMWAddonDiscovery {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)][string]$DataDirectory,
        [string[]]$EnabledAddonPaths = @(),
        [string]$ConfigurationPath
    )

    $dataRoot = [IO.Path]::GetFullPath($DataDirectory)
    if ($EnabledAddonPaths.Count -eq 0 -and $ConfigurationPath -and (Test-Path -LiteralPath $ConfigurationPath -PathType Leaf)) {
        $configuration = Get-Content -LiteralPath $ConfigurationPath -Raw | ConvertFrom-Json
        if ($configuration.enabled -eq $true) { $EnabledAddonPaths = @($configuration.enabledAddons) }
    }
    $discovered = @{}
    $duplicates = [Collections.Generic.List[object]]::new()
    foreach ($file in @(Get-ChildItem -LiteralPath $dataRoot -File -Filter '*.omwaddon')) {
        $key = $file.Name.ToLowerInvariant()
        if ($discovered.ContainsKey($key)) { $duplicates.Add([pscustomobject]@{ filename = $file.Name; paths = @($discovered[$key], $file.FullName) }) }
        else { $discovered[$key] = $file.FullName }
    }
    if ($duplicates.Count -gt 0) { throw "Duplicate discovered addon filenames: $($duplicates.filename -join ', ')" }

    $enabled = [Collections.Generic.List[string]]::new()
    foreach ($configuredPath in $EnabledAddonPaths) {
        $candidate = if ([IO.Path]::IsPathRooted($configuredPath)) { [IO.Path]::GetFullPath($configuredPath) } else { [IO.Path]::GetFullPath((Join-Path $dataRoot $configuredPath)) }
        if (-not (Test-Path -LiteralPath $candidate -PathType Leaf)) { throw "Enabled addon was not found: $candidate" }
        $enabled.Add($candidate)
        $nameKey = [IO.Path]::GetFileName($candidate).ToLowerInvariant()
        if ($discovered.ContainsKey($nameKey) -and $discovered[$nameKey] -ine $candidate) { throw "Duplicate addon identity '$([IO.Path]::GetFileName($candidate))': $($discovered[$nameKey]); $candidate" }
        $discovered[$nameKey] = $candidate
    }

    $inspectionsByName = @{}
    function Add-EnabledAddon([string]$addonPath) {
        $inspection = Read-OpenMWAddonContent $addonPath
        if ($inspection.classification -notin @('direct-load', 'transformable')) {
            throw "Enabled addon '$($inspection.source.filename)' is $($inspection.classification): $($inspection.reasons -join '; ')"
        }
        $key = $inspection.source.filename.ToLowerInvariant()
        if ($inspectionsByName.ContainsKey($key)) { return }
        $inspectionsByName[$key] = $inspection
        foreach ($master in $inspection.masters) {
            $masterKey = $master.name.ToLowerInvariant()
            if ($masterKey.EndsWith('.omwaddon') -and $discovered.ContainsKey($masterKey)) { Add-EnabledAddon $discovered[$masterKey] }
        }
    }
    foreach ($addonPath in $enabled) { Add-EnabledAddon $addonPath }
    $resolution = Resolve-OpenMWAddonDependencies @($inspectionsByName.Values) $dataRoot
    return [pscustomobject][ordered]@{
        dataDirectory = $dataRoot
        enablement = if ($EnabledAddonPaths.Count -gt 0) { 'explicit-paths' } elseif ($ConfigurationPath) { 'configuration' } else { 'disabled' }
        configuredPaths = @($EnabledAddonPaths)
        discoveredAddons = @($discovered.GetEnumerator() | ForEach-Object { [pscustomobject]@{ filename = [IO.Path]::GetFileName($_.Value); path = $_.Value } })
        orderedAddons = @($resolution.orderedAddons)
    }
}

function New-OpenMWAddonNativeFile {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory)]$Inspection,
        [Parameter(Mandatory)][string]$CacheRoot,
        [hashtable]$DependencyAliases = @{}
    )

    if ($Inspection.classification -notin @('direct-load', 'transformable')) {
        throw "Addon '$($Inspection.source.filename)' is $($Inspection.classification) and cannot be prepared."
    }
    $identity = '{0}|{1}|{2}|{3}' -f $Inspection.source.path.ToLowerInvariant(), $Inspection.source.size, $Inspection.source.modifiedUtc, $Inspection.source.sha256
    $identityBytes = [Text.Encoding]::UTF8.GetBytes($identity)
    $hasher = [Security.Cryptography.SHA256]::Create()
    try { $cacheKey = ([BitConverter]::ToString($hasher.ComputeHash($identityBytes))).Replace('-', '').ToLowerInvariant() } finally { $hasher.Dispose() }
    $baseName = $Inspection.source.filename -replace '[^A-Za-z0-9_.-]', '_'
    $aliasName = '{0}-{1}.esp' -f $baseName, $cacheKey.Substring(0, 16)
    $cacheDirectory = Join-Path ([IO.Path]::GetFullPath($CacheRoot)) $cacheKey
    $nativePath = Join-Path $cacheDirectory $aliasName
    $reportPath = Join-Path $cacheDirectory 'compatibility-report.json'
    [IO.Directory]::CreateDirectory($cacheDirectory) | Out-Null

    $transformations = [Collections.Generic.List[object]]::new()
    $mustRewriteHeader = $Inspection.formatVersion -eq 1 -or $DependencyAliases.Count -gt 0
    if (-not $mustRewriteHeader) {
        [IO.File]::Copy($Inspection.source.path, $nativePath, $true)
        $transformations.Add([pscustomobject]@{ kind = 'extension-alias'; detail = 'Content bytes copied unchanged to a native .esp cache alias.' })
    }
    else {
        $bytes = [IO.File]::ReadAllBytes($Inspection.source.path)
        $tes3 = $Inspection.records[0]
        $headerStream = [IO.MemoryStream]::new()
        try {
            [long]$position = $tes3.dataOffset
            [long]$end = $tes3.dataOffset + $tes3.size
            while ($position -lt $end) {
                $tag = Get-Tag $bytes ([int]$position)
                $size = Get-UInt32 $bytes ([int]$position + 4)
                $total = 8 + [long]$size
                if ($tag -eq 'FORM') {
                    $transformations.Add([pscustomobject]@{ kind = 'remove-form'; detail = "Removed FORM format version $($Inspection.formatVersion) and rewrote TES3 size." })
                }
                elseif ($tag -eq 'MAST') {
                    $name = Get-ZeroTerminatedString $bytes ([int]$position + 8) ([int]$size)
                    $replacement = $null
                    foreach ($key in $DependencyAliases.Keys) { if ($key -ieq $name) { $replacement = [string]$DependencyAliases[$key]; break } }
                    if ($replacement) {
                        $encoded = [Text.Encoding]::GetEncoding(1252).GetBytes($replacement + [char]0)
                        $tagBytes = [Text.Encoding]::ASCII.GetBytes('MAST')
                        $sizeBytes = [BitConverter]::GetBytes([uint32]$encoded.Length)
                        $headerStream.Write($tagBytes, 0, 4); $headerStream.Write($sizeBytes, 0, 4); $headerStream.Write($encoded, 0, $encoded.Length)
                        $transformations.Add([pscustomobject]@{ kind = 'rewrite-master'; original = $name; native = $replacement })
                    }
                    else { $headerStream.Write($bytes, [int]$position, [int]$total) }
                }
                else { $headerStream.Write($bytes, [int]$position, [int]$total) }
                $position += $total
            }
            $newHeader = $headerStream.ToArray()
        }
        finally { $headerStream.Dispose() }

        $output = [IO.MemoryStream]::new()
        try {
            $output.Write($bytes, 0, 4)
            $output.Write([BitConverter]::GetBytes([uint32]$newHeader.Length), 0, 4)
            $output.Write($bytes, 8, 8)
            $output.Write($newHeader, 0, $newHeader.Length)
            $remainingOffset = [int]($tes3.dataOffset + $tes3.size)
            $output.Write($bytes, $remainingOffset, $bytes.Length - $remainingOffset)
            [IO.File]::WriteAllBytes($nativePath, $output.ToArray())
        }
        finally { $output.Dispose() }
    }

    $nativeInspection = Read-OpenMWAddonContent -Path $nativePath
    if ($nativeInspection.classification -ne 'direct-load') { throw "Prepared alias failed reinspection: $($nativeInspection.classification)" }
    $report = [ordered]@{
        schemaVersion = 1; generatedUtc = [DateTime]::UtcNow.ToString('o'); classification = $Inspection.classification
        source = $Inspection.source; cacheKey = $cacheKey
        native = [ordered]@{ filename = $aliasName; path = $nativePath; size = (Get-Item -LiteralPath $nativePath).Length; sha256 = (Get-FileHash -LiteralPath $nativePath -Algorithm SHA256).Hash.ToLowerInvariant() }
        masters = $Inspection.masters; recordCounts = $Inspection.recordCounts
        openMWOnlyRecords = $Inspection.openMWOnlyRecords; unknownRecords = $Inspection.unknownRecords
        transformations = @($transformations); ignoredFeatures = @(); nativeInspection = $nativeInspection
        csse = [ordered]@{ displayFilename = $Inspection.source.filename; readOnly = $true; saveEditsAs = '.esp' }
    }
    [IO.File]::WriteAllText($reportPath, ($report | ConvertTo-Json -Depth 30), [Text.UTF8Encoding]::new($false))
    return [pscustomobject][ordered]@{ aliasName = $aliasName; nativePath = $nativePath; reportPath = $reportPath; cacheKey = $cacheKey; report = [pscustomobject]$report }
}

function Get-OpenMWAddonAliasSourceName {
    [CmdletBinding()]
    param([Parameter(Mandatory)][string]$AliasName)

    $match = [regex]::Match($AliasName, '^(?<source>.+\.omwaddon)-[0-9a-f]{16}\.esp$', [Text.RegularExpressions.RegexOptions]::IgnoreCase)
    if (-not $match.Success) { return $null }
    return $match.Groups['source'].Value
}

Export-ModuleMember -Function Read-OpenMWAddonContent, Resolve-OpenMWAddonDependencies, Get-OpenMWAddonDiscovery, New-OpenMWAddonNativeFile, Get-OpenMWAddonAliasSourceName
