[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Debug',
    [string]$MorrowindDirectory = 'C:\Games\Morrowind',
    [string]$FixtureSave = 'TestMWSE0000.ess',
    [string]$TeleportCell = 'Balmora, Guild of Mages',
    [ValidateSet('Smoke')][string]$Suite = 'Smoke',
    [int]$ReadyTimeoutSeconds = 45,
    [int]$RequestTimeoutSeconds = 20,
    [switch]$SkipBuild
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
Import-Module (Join-Path $PSScriptRoot 'HarnessProtocol.psm1') -Force
$morrowindRoot = [IO.Path]::GetFullPath($MorrowindDirectory).TrimEnd('\')
$executable = Join-Path $morrowindRoot 'Morrowind.exe'
$fixturePath = Join-Path (Join-Path $morrowindRoot 'Saves') $FixtureSave
$runId = New-OwchRunId
$runtimeRoot = Join-Path $morrowindRoot 'Data Files\MWSE\tmp\openmw-compat-harness'
$runDirectory = Join-Path $runtimeRoot $runId
$eventsPath = Join-Path $runDirectory 'events.jsonl'
$commandPath = Join-Path $runDirectory 'command.json'
$readyPath = Join-Path $runDirectory 'ready.json'
$resultPath = Join-Path $runDirectory 'result.json'
$backupRoot = Join-Path ([IO.Path]::GetTempPath()) ('owch-backup-' + $runId)
$smokeSaveBase = 'owc_m1_' + $runId.Substring($runId.Length - 8)
$smokeSavePath = Join-Path (Join-Path $morrowindRoot 'Saves') ($smokeSaveBase + '.ess')
$process = $null
$seenEvents = 0
$messageInbox = [Collections.Generic.List[object]]::new()
$staged = @()
$assertions = [Collections.Generic.List[object]]::new()
$startedAt = [DateTime]::UtcNow
$failure = $null
$exitCode = $null
$shutdownGraceful = $false

function Assert-UnderRoot([string]$Path, [string]$Root) {
    $resolvedPath = [IO.Path]::GetFullPath($Path)
    $resolvedRoot = [IO.Path]::GetFullPath($Root).TrimEnd('\') + '\'
    if (-not $resolvedPath.StartsWith($resolvedRoot, [StringComparison]::OrdinalIgnoreCase)) {
        throw "Path '$resolvedPath' is outside expected root '$resolvedRoot'."
    }
}

function Backup-Target([string]$Target) {
    Assert-UnderRoot $Target $morrowindRoot
    $entry = [pscustomobject]@{
        Target = $Target
        Backup = Join-Path $backupRoot ([Guid]::NewGuid().ToString('N'))
        Existed = Test-Path -LiteralPath $Target
        IsDirectory = Test-Path -LiteralPath $Target -PathType Container
    }
    if ($entry.Existed) {
        [IO.Directory]::CreateDirectory($backupRoot) | Out-Null
        if ($entry.IsDirectory) {
            Copy-Item -LiteralPath $Target -Destination $entry.Backup -Recurse
        }
        else {
            Copy-Item -LiteralPath $Target -Destination $entry.Backup
        }
    }
    $script:staged += $entry
}

function Stage-File([string]$Source, [string]$Target) {
    if (-not (Test-Path -LiteralPath $Source -PathType Leaf)) { throw "Required staging source is missing: $Source" }
    Backup-Target $Target
    [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($Target)) | Out-Null
    Copy-Item -LiteralPath $Source -Destination $Target -Force
}

function Restore-StagedFiles {
    foreach ($entry in @($script:staged)[($script:staged.Count - 1)..0]) {
        Assert-UnderRoot $entry.Target $morrowindRoot
        if (Test-Path -LiteralPath $entry.Target) {
            Remove-Item -LiteralPath $entry.Target -Recurse -Force
        }
        if ($entry.Existed) {
            [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($entry.Target)) | Out-Null
            Copy-Item -LiteralPath $entry.Backup -Destination $entry.Target -Recurse:$entry.IsDirectory
        }
    }
}

function Get-NewHarnessEvents {
    $read = Read-OwchJsonLines -Path $eventsPath
    $messages = @($read.Messages)
    if ($messages.Count -gt $script:seenEvents) {
        $newMessages = @($messages[$script:seenEvents..($messages.Count - 1)])
        $script:seenEvents = $messages.Count
        foreach ($message in $newMessages) {
            if ($message.type -in @('ready', 'response', 'assertion', 'fatal', 'shutdown')) {
                Write-Host ($message | ConvertTo-Json -Depth 15 -Compress)
            }
        }
        return $newMessages
    }
    return @()
}

function Write-TimeoutDiagnostic([string]$Operation) {
    $read = Read-OwchJsonLines -Path $eventsPath
    $messages = @($read.Messages)
    $diagnostic = [ordered]@{
        operation = $Operation
        timestamp = [DateTime]::UtcNow.ToString('o')
        processExited = if ($script:process) { $script:process.HasExited } else { $null }
        lastHeartbeat = @($messages | Where-Object type -eq 'heartbeat' | Select-Object -Last 1)
        lastMessages = @($messages | Select-Object -Last 20)
        mwseLogTail = if (Test-Path -LiteralPath (Join-Path $morrowindRoot 'MWSE.log')) { @(Get-Content -LiteralPath (Join-Path $morrowindRoot 'MWSE.log') -Tail 40) } else { @() }
    }
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'timeout-diagnostic.json') -Value $diagnostic
    return $diagnostic
}

function Wait-HarnessMessage([scriptblock]$Predicate, [int]$TimeoutSeconds, [string]$Operation) {
    $deadline = [DateTime]::UtcNow.AddSeconds($TimeoutSeconds)
    while ([DateTime]::UtcNow -lt $deadline) {
        for ($index = $script:messageInbox.Count - 1; $index -ge 0; $index--) {
            $message = $script:messageInbox[$index]
            if ($message.type -eq 'fatal') { throw "Harness fatal message: $($message.error.message)" }
            if (& $Predicate $message) {
                $script:messageInbox.RemoveAt($index)
                return $message
            }
        }

        $matched = $null
        foreach ($message in @(Get-NewHarnessEvents)) {
            if ($message.type -eq 'fatal') { throw "Harness fatal message: $($message.error.message)" }
            if ($null -eq $matched -and (& $Predicate $message)) {
                $matched = $message
            }
            else {
                $script:messageInbox.Add($message)
            }
        }
        if ($null -ne $matched) {
            return $matched
        }
        if ($script:process -and $script:process.HasExited) {
            Get-NewHarnessEvents | Out-Null
            throw "Morrowind exited before completing '$Operation' (exit code $($script:process.ExitCode))."
        }
        Start-Sleep -Milliseconds 100
    }
    $diagnostic = Write-TimeoutDiagnostic $Operation
    throw "Timed out after $TimeoutSeconds seconds during '$Operation'. Diagnostic: $(Join-Path $runDirectory 'timeout-diagnostic.json')"
}

function Send-HarnessCommand([string]$Command, $Arguments = $null, [int]$TimeoutSeconds = $RequestTimeoutSeconds, [switch]$NoWait) {
    $requestId = New-OwchRequestId
    $deadline = [DateTime]::UtcNow.AddSeconds(5)
    while (Test-Path -LiteralPath $commandPath) {
        if ([DateTime]::UtcNow -ge $deadline) { throw "Previous harness command was not consumed: $commandPath" }
        Start-Sleep -Milliseconds 50
    }
    $request = [ordered]@{
        protocolVersion = 1
        runId = $runId
        requestId = $requestId
        type = 'command'
        command = $Command
        arguments = $Arguments
        timeoutMs = $TimeoutSeconds * 1000
        sentAt = [DateTime]::UtcNow.ToString('o')
    }
    Write-OwchAtomicJson -Path $commandPath -Value $request

    if ($NoWait) {
        $consumeDeadline = [DateTime]::UtcNow.AddSeconds(5)
        while (Test-Path -LiteralPath $commandPath) {
            if ($script:process.HasExited) { throw "Morrowind exited while consuming command '$Command'." }
            if ([DateTime]::UtcNow -ge $consumeDeadline) { throw "Harness did not consume command '$Command'." }
            Start-Sleep -Milliseconds 50
        }
        return $requestId
    }

    $response = Wait-HarnessMessage -TimeoutSeconds $TimeoutSeconds -Operation "$Command ($requestId)" -Predicate { param($message) $message.type -eq 'response' -and $message.requestId -eq $requestId }
    if (-not $response.ok) { throw "Harness command '$Command' failed: $($response.error.message)" }
    return $response
}

function Wait-HarnessResponse([string]$RequestId, [int]$TimeoutSeconds, [string]$Operation) {
    $response = Wait-HarnessMessage -TimeoutSeconds $TimeoutSeconds -Operation $Operation -Predicate { param($message) $message.type -eq 'response' -and $message.requestId -eq $RequestId }
    if (-not $response.ok) { throw "Harness request failed: $($response.error.message)" }
    return $response
}

function Add-SmokeAssertion([string]$Name, [bool]$Passed, $Actual, $Expected) {
    $entry = [pscustomobject]@{ name = $Name; passed = $Passed; actual = $Actual; expected = $Expected }
    $script:assertions.Add($entry)
    Write-Host ($entry | ConvertTo-Json -Depth 10 -Compress)
    if (-not $Passed) { throw "Smoke assertion failed: $Name" }
}

function Invoke-NamedProbe([string]$Name, $Arguments = @{}) {
    return Send-HarnessCommand -Command 'evalNamedProbe' -Arguments ([ordered]@{ name = $Name; arguments = $Arguments })
}

try {
    if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) { throw "Morrowind executable not found: $executable" }
    if (-not (Test-Path -LiteralPath $fixturePath -PathType Leaf)) { throw "Fixture save not found: $fixturePath" }
    $existing = @(Get-Process -Name 'Morrowind' -ErrorAction SilentlyContinue)
    if ($existing.Count -gt 0) { throw 'Morrowind is already running. Close it before starting the automated harness.' }

    [IO.Directory]::CreateDirectory($runDirectory) | Out-Null
    [IO.Directory]::CreateDirectory((Join-Path $runDirectory 'screenshots')) | Out-Null
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'run.json') -Value ([ordered]@{
        protocolVersion = 1; runId = $runId; suite = $Suite; configuration = $Configuration
        repository = $repoRoot; morrowindDirectory = $morrowindRoot; fixtureSave = $FixtureSave
        startedAt = $startedAt.ToString('o')
    })

    if (-not $SkipBuild) {
        $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
        if (-not (Test-Path -LiteralPath $vswhere)) { throw "vswhere was not found: $vswhere" }
        $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
        if (-not $msbuild) { throw 'MSBuild could not be located with vswhere.' }
        $buildStage = Join-Path $runDirectory 'build-stage'
        [IO.Directory]::CreateDirectory($buildStage) | Out-Null
        $buildArguments = @(
            (Join-Path $repoRoot 'MWSE\MWSE.vcxproj'), '/t:Build', "/p:Configuration=$Configuration", '/p:Platform=Win32',
            "/p:SolutionDir=$repoRoot/", "/p:MorrowindDir=$buildStage/", '/p:PostBuildEventUseInBuild=false', '/nr:false'
        )
        & $msbuild @buildArguments 2>&1 | Tee-Object -FilePath (Join-Path $runDirectory 'build.log')
        if ($LASTEXITCODE -ne 0) { throw "MWSE build failed with exit code $LASTEXITCODE. See $(Join-Path $runDirectory 'build.log')" }
    }

    $buildOutput = Join-Path $repoRoot ("build\$Configuration")
    Stage-File (Join-Path $buildOutput 'MWSE.dll') (Join-Path $morrowindRoot 'MWSE.dll')
    if (Test-Path -LiteralPath (Join-Path $buildOutput 'MWSE.pdb')) {
        Stage-File (Join-Path $buildOutput 'MWSE.pdb') (Join-Path $morrowindRoot 'MWSE.pdb')
    }
    if (Test-Path -LiteralPath (Join-Path $buildOutput 'lua51.dll')) {
        Stage-File (Join-Path $buildOutput 'lua51.dll') (Join-Path $morrowindRoot 'lua51.dll')
    }

    foreach ($requiredCoreFile in @('Data Files\MWSE\core\initialize.lua', 'Data Files\MWSE\core\startLuaMods.lua', 'Data Files\MWSE\core\lib\dkjson.lua', 'Data Files\MWSE\core\lib\lfs.dll')) {
        if (-not (Test-Path -LiteralPath (Join-Path $morrowindRoot $requiredCoreFile))) { throw "The test installation is missing required MWSE core file: $requiredCoreFile" }
    }

    $harnessSource = Join-Path $repoRoot 'misc\package\Data Files\MWSE\mods\openmw_compat_harness\main.lua'
    $harnessTarget = Join-Path $morrowindRoot 'Data Files\MWSE\mods\openmw_compat_harness\main.lua'
    Stage-File $harnessSource $harnessTarget
    $configTarget = Join-Path $morrowindRoot 'Data Files\MWSE\config\openmw_compat_harness.json'
    Backup-Target $configTarget
    $relativeScreenshotDirectory = 'Data Files\MWSE\tmp\openmw-compat-harness\' + $runId + '\screenshots'
    Write-OwchAtomicJson -Path $configTarget -Value ([ordered]@{ enabled = $true; protocolVersion = 1; runId = $runId; runtimeDirectory = $runDirectory; screenshotDirectory = $relativeScreenshotDirectory })

    $process = Start-Process -FilePath $executable -WorkingDirectory $morrowindRoot -PassThru
    $readyDeadline = [DateTime]::UtcNow.AddSeconds($ReadyTimeoutSeconds)
    while (-not (Test-Path -LiteralPath $readyPath)) {
        Get-NewHarnessEvents | Out-Null
        if ($process.HasExited) { throw "Morrowind exited before the harness became ready (exit code $($process.ExitCode))." }
        if ([DateTime]::UtcNow -ge $readyDeadline) {
            Write-TimeoutDiagnostic 'ready handshake' | Out-Null
            throw "Harness ready handshake timed out. See $(Join-Path $runDirectory 'timeout-diagnostic.json')"
        }
        Start-Sleep -Milliseconds 100
    }
    $ready = Get-Content -LiteralPath $readyPath -Raw | ConvertFrom-Json
    Add-SmokeAssertion 'ready-handshake' ($ready.protocolVersion -eq 1 -and $ready.runId -eq $runId) $ready.runId $runId

    $ping = Send-HarnessCommand 'ping'
    Add-SmokeAssertion 'ping' ($ping.result.pong -eq $true) $ping.result.pong $true
    $initialState = (Send-HarnessCommand 'getState').result
    Add-SmokeAssertion 'main-menu-detection' ($initialState.mainMenu -eq $true) $initialState.name 'mainMenu'
    Invoke-NamedProbe 'mwseInitialization' | Out-Null

    $loadedRequest = Send-HarnessCommand 'waitForEvent' @{ event = 'loaded' } -TimeoutSeconds $ReadyTimeoutSeconds -NoWait
    Send-HarnessCommand 'loadGame' @{ filename = $FixtureSave } -TimeoutSeconds $ReadyTimeoutSeconds | Out-Null
    Wait-HarnessResponse $loadedRequest $ReadyTimeoutSeconds 'fixture loaded event' | Out-Null
    $playerProbe = Invoke-NamedProbe 'playerAndCell'
    Add-SmokeAssertion 'known-game-state' ($playerProbe.result.value.inGame -and $playerProbe.result.value.playerValid) $playerProbe.result.value.name 'inGame'

    $cellEventRequest = Send-HarnessCommand 'waitForEvent' @{ event = 'cellChanged' } -NoWait
    Send-HarnessCommand 'teleport' @{ cell = $TeleportCell; position = @(0, 0, 0); forceCellChange = $true } | Out-Null
    $cellEvent = Wait-HarnessResponse $cellEventRequest $RequestTimeoutSeconds 'native cellChanged event'
    Add-SmokeAssertion 'native-event-observed' ($cellEvent.result.event -eq 'cellChanged') $cellEvent.result.event 'cellChanged'

    $persistentKey = 'milestone1Smoke'
    $persistentValue = 'persisted-' + $runId
    Invoke-NamedProbe 'writeReferencePersistentValue' @{ key = $persistentKey; value = $persistentValue } | Out-Null
    $savedRequest = Send-HarnessCommand 'waitForEvent' @{ event = 'saved' } -TimeoutSeconds $RequestTimeoutSeconds -NoWait
    Invoke-NamedProbe 'saveGame' @{ file = $smokeSaveBase; name = 'OpenMW compatibility harness M1' } | Out-Null
    Wait-HarnessResponse $savedRequest $RequestTimeoutSeconds 'smoke save event' | Out-Null
    Invoke-NamedProbe 'writeReferencePersistentValue' @{ key = $persistentKey; value = 'mutated-after-save' } | Out-Null
    $reloadedRequest = Send-HarnessCommand 'waitForEvent' @{ event = 'loaded' } -TimeoutSeconds $ReadyTimeoutSeconds -NoWait
    Send-HarnessCommand 'loadGame' @{ filename = ($smokeSaveBase + '.ess') } -TimeoutSeconds $ReadyTimeoutSeconds | Out-Null
    Wait-HarnessResponse $reloadedRequest $ReadyTimeoutSeconds 'smoke reload event' | Out-Null
    $readProbe = Invoke-NamedProbe 'readReferencePersistentValue' @{ key = $persistentKey; expected = $persistentValue }
    Add-SmokeAssertion 'reference-persistence-save-load' ($readProbe.result.value.value -eq $persistentValue) $readProbe.result.value.value $persistentValue

    Send-HarnessCommand 'shutdown' -TimeoutSeconds 10 | Out-Null
    if (-not $process.HasExited) {
        $closeRequested = $process.CloseMainWindow()
        if (-not $closeRequested) { throw 'The Morrowind top-level window rejected the graceful close request.' }
    }
    if (-not $process.WaitForExit(15000)) { throw 'Morrowind did not exit within 15 seconds of the graceful shutdown request.' }
    $exitCode = $process.ExitCode
    $shutdownGraceful = $true
    Add-SmokeAssertion 'graceful-shutdown' ($exitCode -eq 0) $exitCode 0
}
catch {
    $failure = $_.Exception.ToString()
    Write-Error -ErrorAction Continue $failure
}
finally {
    if ($process -and -not $process.HasExited) {
        Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
        $process.WaitForExit(5000) | Out-Null
    }
    if (Test-Path -LiteralPath (Join-Path $morrowindRoot 'MWSE.log')) {
        Copy-Item -LiteralPath (Join-Path $morrowindRoot 'MWSE.log') -Destination (Join-Path $runDirectory 'MWSE.log') -Force -ErrorAction SilentlyContinue
    }
    if (Test-Path -LiteralPath $smokeSavePath) {
        Copy-Item -LiteralPath $smokeSavePath -Destination (Join-Path $runDirectory ($smokeSaveBase + '.ess')) -Force -ErrorAction SilentlyContinue
        Remove-Item -LiteralPath $smokeSavePath -Force -ErrorAction SilentlyContinue
    }
    if ($staged.Count -gt 0) { Restore-StagedFiles }
    $harnessDirectory = Join-Path $morrowindRoot 'Data Files\MWSE\mods\openmw_compat_harness'
    if ((Test-Path -LiteralPath $harnessDirectory -PathType Container) -and @(Get-ChildItem -LiteralPath $harnessDirectory -Force).Count -eq 0) {
        Remove-Item -LiteralPath $harnessDirectory -Force
    }
    if (Test-Path -LiteralPath $backupRoot) { Remove-Item -LiteralPath $backupRoot -Recurse -Force -ErrorAction SilentlyContinue }

    $allEvents = @(if (Test-Path -LiteralPath $eventsPath) { (Read-OwchJsonLines -Path $eventsPath).Messages } else { @() })
    $gameAssertions = @($allEvents | Where-Object type -eq 'assertion')
    $passed = $null -eq $failure -and $shutdownGraceful -and @($assertions | Where-Object { -not $_.passed }).Count -eq 0 -and @($gameAssertions | Where-Object { -not $_.passed }).Count -eq 0
    $result = [ordered]@{
        protocolVersion = 1; runId = $runId; suite = $Suite; passed = $passed
        startedAt = $startedAt.ToString('o'); finishedAt = [DateTime]::UtcNow.ToString('o')
        processExitCode = $exitCode; gracefulShutdown = $shutdownGraceful; failure = $failure
        launcherAssertions = @($assertions); gameAssertions = $gameAssertions
        artifacts = [ordered]@{ runDirectory = $runDirectory; events = $eventsPath; result = $resultPath; mwseLog = (Join-Path $runDirectory 'MWSE.log') }
    }
    Write-OwchAtomicJson -Path $resultPath -Value $result
    Write-Output ($result | ConvertTo-Json -Depth 30)
}

if ($failure) { exit 1 }
exit 0
