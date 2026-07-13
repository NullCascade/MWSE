Set-StrictMode -Version Latest

function New-OwchRunId {
    return ('{0}-{1}' -f [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssZ'), [Guid]::NewGuid().ToString('N'))
}

function New-OwchRequestId {
    return [Guid]::NewGuid().ToString('N')
}

function Write-OwchAtomicJson {
    param(
        [Parameter(Mandatory)][string]$Path,
        [Parameter(Mandatory)]$Value
    )

    $directory = [IO.Path]::GetDirectoryName($Path)
    if ($directory -and -not [IO.Directory]::Exists($directory)) {
        [IO.Directory]::CreateDirectory($directory) | Out-Null
    }
    $temporaryPath = '{0}.{1}.tmp' -f $Path, [Guid]::NewGuid().ToString('N')
    $json = $Value | ConvertTo-Json -Depth 30 -Compress
    [IO.File]::WriteAllText($temporaryPath, $json, [Text.UTF8Encoding]::new($false))
    [IO.File]::Move($temporaryPath, $Path)
}

function Read-OwchJsonLines {
    param([Parameter(Mandatory)][string]$Path)

    if (-not [IO.File]::Exists($Path)) {
        return [pscustomobject]@{ Messages = @(); PartialLine = $false }
    }
    $text = $null
    for ($attempt = 0; $attempt -lt 20; $attempt++) {
        try {
            $stream = [IO.FileStream]::new($Path, [IO.FileMode]::Open, [IO.FileAccess]::Read, ([IO.FileShare]::ReadWrite -bor [IO.FileShare]::Delete))
            try {
                $reader = [IO.StreamReader]::new($stream, [Text.Encoding]::UTF8, $true, 4096, $true)
                try { $text = $reader.ReadToEnd() } finally { $reader.Dispose() }
            }
            finally { $stream.Dispose() }
            break
        }
        catch [IO.IOException] {
            if ($attempt -eq 19) { throw }
            Start-Sleep -Milliseconds 10
        }
    }
    $partialLine = $text.Length -gt 0 -and -not $text.EndsWith("`n")
    $lines = $text -split "`r?`n"
    $messages = @()
    $completeLineCount = $lines.Count - $(if ($partialLine) { 1 } else { 0 })
    for ($index = 0; $index -lt $completeLineCount; $index++) {
        if ([string]::IsNullOrWhiteSpace($lines[$index])) { continue }
        try {
            $messages += $lines[$index] | ConvertFrom-Json
        }
        catch {
            throw "Invalid complete JSONL record at line $($index + 1) in '$Path': $($_.Exception.Message)"
        }
    }
    return [pscustomobject]@{ Messages = $messages; PartialLine = $partialLine }
}

function Test-OwchIdentifier {
    param([Parameter(Mandatory)][string]$Value)
    return $Value -match '^[A-Za-z0-9][A-Za-z0-9_.:-]{7,127}$'
}

Export-ModuleMember -Function New-OwchRunId, New-OwchRequestId, Write-OwchAtomicJson, Read-OwchJsonLines, Test-OwchIdentifier
