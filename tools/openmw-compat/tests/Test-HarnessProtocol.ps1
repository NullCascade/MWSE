[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'
Import-Module (Join-Path $PSScriptRoot '..\HarnessProtocol.psm1') -Force
$root = Join-Path ([IO.Path]::GetTempPath()) ('owch-protocol-' + [Guid]::NewGuid().ToString('N'))
[IO.Directory]::CreateDirectory($root) | Out-Null

try {
    $runId = New-OwchRunId
    $requestId = New-OwchRequestId
    if (-not (Test-OwchIdentifier $runId)) { throw 'Run ID failed validation.' }
    if (-not (Test-OwchIdentifier $requestId)) { throw 'Request ID failed validation.' }

    $commandPath = Join-Path $root 'command.json'
    Write-OwchAtomicJson -Path $commandPath -Value ([ordered]@{ protocolVersion = 1; runId = $runId; requestId = $requestId; type = 'command'; command = 'ping'; timeoutMs = 1000 })
    if (Get-ChildItem -LiteralPath $root -Filter '*.tmp') { throw 'Atomic command temporary file was retained.' }
    $command = Get-Content -LiteralPath $commandPath -Raw | ConvertFrom-Json
    if ($command.protocolVersion -ne 1 -or $command.command -ne 'ping') { throw 'Atomic command content was invalid.' }

    $eventsPath = Join-Path $root 'events.jsonl'
    [IO.File]::WriteAllText($eventsPath, "{`"type`":`"ready`"}`n{`"type`":`"heart", [Text.UTF8Encoding]::new($false))
    $read = Read-OwchJsonLines -Path $eventsPath
    if ($read.Messages.Count -ne 1 -or -not $read.PartialLine) { throw 'Partial JSONL record was not detected and excluded.' }

    [pscustomobject]@{
        passed = $true
        tests = @('unique-identifiers', 'atomic-json-command', 'partial-jsonl-detection')
    } | ConvertTo-Json -Depth 5
}
finally {
    Remove-Item -LiteralPath $root -Recurse -Force -ErrorAction SilentlyContinue
}
