[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Debug',
    [string]$MorrowindDirectory = 'C:\Games\Morrowind',
    [string]$FixtureSave = 'TestMWSE0000.ess',
    [string]$TeleportCell = 'Balmora, Guild of Mages',
    [ValidateSet('Smoke', 'OpenMWAddon', 'OpenMWLuaHost', 'OpenMWLuaFoundation', 'OpenMWLuaPlayerBindings')][string]$Suite = 'Smoke',
    [string]$OpenMWAddonPath = 'C:\Games\Morrowind\Data Files\ncg.omwaddon',
    [switch]$ProbeNativeAddonExtension,
    [int]$ReadyTimeoutSeconds = 45,
    [int]$RequestTimeoutSeconds = 20,
    [switch]$SkipBuild
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$isOpenMWLuaSuite = $Suite -in @('OpenMWLuaHost', 'OpenMWLuaFoundation', 'OpenMWLuaPlayerBindings')
$openMWLuaScriptsName = if ($Suite -eq 'OpenMWLuaFoundation') { 'milestone4-foundation.omwscripts' } elseif ($Suite -eq 'OpenMWLuaPlayerBindings') { 'milestone4-player-bindings.omwscripts' } else { 'milestone3.omwscripts' }

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
Import-Module (Join-Path $PSScriptRoot 'HarnessProtocol.psm1') -Force
Import-Module (Join-Path $PSScriptRoot 'OpenMWAddon.psm1') -Force
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
$addonPreparations = [Collections.Generic.List[object]]::new()
$originalIniHash = $null
$restoredIniHash = $null
$loadOrderRestored = $null
$originalGameFiles = @()
$enabledNativeFiles = [Collections.Generic.List[string]]::new()
$openMWEnvironmentNames = @('MWSE_OPENMW_LUA_VFS_ROOT', 'MWSE_OPENMW_LUA_SCRIPTS_FILE', 'MWSE_OPENMW_LUA_CONTENT_FILE', 'MWSE_OPENMW_LUA_REPORT_DIRECTORY', 'MWSE_OPENMW_LUA_HARNESS', 'MWSE_OPENMW_LUA_DISABLED')
$originalOpenMWEnvironment = @{}
$syntheticVfs = Join-Path $runDirectory 'synthetic-vfs'
$nativeTestPath = Join-Path $runDirectory 'native-tests.json'

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
        OriginalSha256 = if (Test-Path -LiteralPath $Target -PathType Leaf) { (Get-FileHash -LiteralPath $Target -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }
        SkipRestore = $false
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
    if (Test-Path -LiteralPath $Target -PathType Leaf) {
        $sourceHash = (Get-FileHash -LiteralPath $Source -Algorithm SHA256).Hash.ToLowerInvariant()
        $targetHash = (Get-FileHash -LiteralPath $Target -Algorithm SHA256).Hash.ToLowerInvariant()
        if ($sourceHash -eq $targetHash) {
            $script:staged += [pscustomobject]@{ Target = $Target; Backup = $null; Existed = $true; IsDirectory = $false; OriginalSha256 = $targetHash; SkipRestore = $true }
            return
        }
    }
    Backup-Target $Target
    [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($Target)) | Out-Null
    Copy-Item -LiteralPath $Source -Destination $Target -Force
}

function Restore-StagedFiles {
    foreach ($entry in @($script:staged)[($script:staged.Count - 1)..0]) {
        if ($entry.SkipRestore) { continue }
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

function Enable-TemporaryGameFiles([string[]]$Filenames) {
    $iniPath = Join-Path $morrowindRoot 'Morrowind.ini'
    if (-not (Test-Path -LiteralPath $iniPath -PathType Leaf)) { throw "Morrowind.ini was not found: $iniPath" }
    $script:originalIniHash = (Get-FileHash -LiteralPath $iniPath -Algorithm SHA256).Hash.ToLowerInvariant()
    $raw = [IO.File]::ReadAllText($iniPath)
    $matches = [regex]::Matches($raw, '(?m)^GameFile(?<index>\d+)=(?<name>[^\r\n]*)')
    $script:originalGameFiles = @($matches | ForEach-Object { $_.Groups['name'].Value })
    $nextIndex = if ($matches.Count -gt 0) { 1 + ($matches | ForEach-Object { [int]$_.Groups['index'].Value } | Measure-Object -Maximum).Maximum } else { 0 }
    $newline = if ($raw.Contains("`r`n")) { "`r`n" } else { "`n" }
    $sectionPattern = '(?ms)(?<header>^\[Game Files\]\r?\n)(?<body>.*?)(?=^\[|\z)'
    $section = [regex]::Match($raw, $sectionPattern)
    if (-not $section.Success) { throw 'Morrowind.ini does not contain a [Game Files] section.' }
    $body = $section.Groups['body'].Value
    if ($body.Length -gt 0 -and -not $body.EndsWith("`n")) { $body += $newline }
    foreach ($filename in $Filenames) {
        if ($script:originalGameFiles -icontains $filename) { continue }
        $body += "GameFile$nextIndex=$filename$newline"
        $nextIndex++
    }
    Backup-Target $iniPath
    $replacement = $section.Groups['header'].Value + $body
    $updated = $raw.Substring(0, $section.Index) + $replacement + $raw.Substring($section.Index + $section.Length)
    [IO.File]::WriteAllText($iniPath, $updated, [Text.Encoding]::GetEncoding(1252))
    [IO.File]::WriteAllText((Join-Path $runDirectory 'Morrowind.ini.active'), $updated, [Text.Encoding]::GetEncoding(1252))
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
    if ($isOpenMWLuaSuite) {
        [IO.Directory]::CreateDirectory((Join-Path $syntheticVfs 'fixture')) | Out-Null
        $utf8NoBom = [Text.UTF8Encoding]::new($false)
        [IO.File]::WriteAllText((Join-Path $syntheticVfs 'fixture\shared.lua'), 'return { value = 42 }' + "`n", $utf8NoBom)
        [IO.File]::WriteAllText((Join-Path $syntheticVfs 'menu.lua'), @'
local shared = require('fixture.shared')
if rawget(_G, 'environmentSentinel') ~= nil then error('MENU sandbox leaked') end
environmentSentinel = 'MENU'
return {
    interfaceName = 'MenuFixture',
    interface = { value = shared.value },
    engineHandlers = { onUpdate = function(dt) assert(type(dt) == 'number') end },
    eventHandlers = { Milestone3Event = function(data) assert(type(data) == 'string') end },
}
'@, $utf8NoBom)
        [IO.File]::WriteAllText((Join-Path $syntheticVfs 'global.lua'), @'
if rawget(_G, 'environmentSentinel') ~= nil then error('GLOBAL sandbox leaked') end
environmentSentinel = 'GLOBAL'
return {
    interfaceName = 'GlobalFixture',
    interface = {},
    engineHandlers = { onUpdate = function(dt) end },
    eventHandlers = { Milestone3Event = function(data) error('intentional Milestone 3 handler failure') end },
}
'@, $utf8NoBom)
        [IO.File]::WriteAllText((Join-Path $syntheticVfs 'player.lua'), @'
if rawget(_G, 'environmentSentinel') ~= nil then error('PLAYER sandbox leaked') end
environmentSentinel = 'PLAYER'
return {
    interfaceName = 'PlayerFixture',
    interface = {},
    engineHandlers = { onUpdate = function(dt) end },
    eventHandlers = { Milestone3Event = function(data) end },
}
'@, $utf8NoBom)
        [IO.File]::WriteAllText((Join-Path $syntheticVfs 'milestone3.omwscripts'), @'
# Milestone 3 live fixture. Ordering is significant.
MENU: menu.lua
GLOBAL: global.lua
PLAYER: player.lua
'@, $utf8NoBom)
        if ($Suite -eq 'OpenMWLuaFoundation') {
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'foundation_provider.lua'), @'
local async = require('openmw.async')
local compat = require('openmw.compatibility')
local core = require('openmw.core')
local storage = require('openmw.storage')
local section = storage.globalSection('FoundationGlobal')
local original = { value = 1 }
section:set('copy', original)
original.value = 99
assert(section:getCopy('copy').value == 1)
section:subscribe(async:callback(function(name, key)
    compat.recordFoundationProbe('storage-global-subscription', name == 'FoundationGlobal' and key == 'changed')
end))
section:set('changed', 7)
local registered = async:registerTimerCallback('registered', function(value)
    compat.recordFoundationProbe('async-registered-simulation', value == 9)
end)
async:newSimulationTimer(0, registered, 9)
async:newUnsavableGameTimer(0, function()
    compat.recordFoundationProbe('async-unsavable-game', true)
end)
core.sendGlobalEvent('FoundationGlobalEvent', { value = 7 })
return { interfaceName = 'FoundationInterface', interface = { answer = 42 } }
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'foundation_consumer.lua'), @'
local async = require('openmw.async')
local compat = require('openmw.compatibility')
local core = require('openmw.core')
local I = require('openmw.interfaces')
local storage = require('openmw.storage')
local util = require('openmw.util')
assert(core.API_REVISION == 70 and type(core.getGMST('sHealth')) == 'string')
assert(core.contentFiles.has('mOrRoWiNd.EsM') and core.contentFiles.indexOf('Morrowind.esm') == 1)
local vector = util.vector2(3, 4)
local normalized, length = vector:normalize()
local color = util.color.rgb(.8, .3, .4)
assert(vector:length() == 5 and vector:length2() == 25 and (vector * 2).x == 6)
assert(normalized:length() > .999 and length == 5 and color.a == 1)
compat.recordFoundationProbe('util-vector-color', not pcall(function() vector.x = 10 end) and util.round(-1.5) == -2)
assert(I.FoundationInterface.answer == 42)
compat.recordFoundationProbe('interfaces-lookup-readonly', not pcall(function() I.FoundationInterface.answer = 0 end))
assert(storage.globalSection('FoundationGlobal'):get('changed') == 7)
local callback = async:callback(function(value) return value + 1 end)
compat.recordFoundationProbe('async-callback-callable', callback(4) == 5)
compat.recordFoundationProbe('self-context-rejected-global', not pcall(function() return require('openmw.self') end))
compat.recordFoundationProbe('core-time-content-gmst', type(core.getSimulationTime()) == 'number' and core.contentFiles.list[1] == 'morrowind.esm')
return { eventHandlers = { FoundationGlobalEvent = function(data)
    compat.recordFoundationProbe('core-delayed-global-event', data.value == 7)
end } }
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'foundation_player.lua'), @'
local async = require('openmw.async')
local compat = require('openmw.compatibility')
local self = require('openmw.self')
local storage = require('openmw.storage')
assert(self._mwseFoundationAvailable == true)
local section = storage.playerSection('FoundationPlayer')
section:subscribe(async:callback(function(name, key)
    compat.recordFoundationProbe('storage-player-subscription', name == 'FoundationPlayer' and key == 'value')
end))
section:set('value', 11)
local globalWritable = pcall(function() storage.globalSection('FoundationGlobal'):set('bad', 1) end)
compat.recordFoundationProbe('storage-context-permissions', not globalWritable and section:get('value') == 11)
compat.recordFoundationProbe('self-player-context', true)
return {}
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'foundation_menu.lua'), @'
local compat = require('openmw.compatibility')
local storage = require('openmw.storage')
storage.playerSection('FoundationMenu'):set('value', 3)
compat.recordFoundationProbe('self-context-rejected-menu', not pcall(function() return require('openmw.self') end))
compat.recordFoundationProbe('storage-menu-player-scope', storage.playerSection('FoundationMenu'):get('value') == 3)
return {}
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'milestone4-foundation.omwscripts'), @'
# Milestone 4.1 foundation package live fixture. Ordering is significant.
GLOBAL: foundation_provider.lua
GLOBAL: foundation_consumer.lua
PLAYER: foundation_player.lua
MENU: foundation_menu.lua
'@, $utf8NoBom)
        }
        elseif ($Suite -eq 'OpenMWLuaPlayerBindings') {
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'player_bindings.lua'), @'
local compat = require('openmw.compatibility')
local core = require('openmw.core')
local T = require('openmw.types')
local self = require('openmw.self')

compat.recordFoundationProbe('player-bindings-identity', rawequal(self, require('openmw.self')) and self.recordId ~= '')
compat.recordFoundationProbe('player-bindings-types', self.type == T.Player and T.Actor.objectIsInstance(self)
    and T.NPC.objectIsInstance(self) and T.Player.objectIsInstance(self))
compat.recordFoundationProbe('player-bindings-cell', self.cell ~= nil and type(self.cell.name) == 'string'
    and type(self.cell.isExterior) == 'boolean' and type(self.cell.hasSky) == 'boolean')

local playerRecord = assert(T.Player.record(self))
local playerClass = assert(T.NPC.classes.record(string.upper(playerRecord.class)))
local playerRace = assert(T.NPC.races.record(string.upper(playerRecord.race)))
local birthsignId = T.Player.getBirthSign(self)
local birthsign = birthsignId and T.Player.birthSigns.record(string.upper(birthsignId))
assert(#core.stats.Attribute.records == 8, 'unexpected attribute record count: ' .. #core.stats.Attribute.records)
assert(#core.stats.Skill.records == 27, 'unexpected skill record count: ' .. #core.stats.Skill.records)
assert(core.stats.Attribute.records.strength.id == 'strength', 'lowercase attribute lookup failed')
assert(playerClass.id == string.lower(playerRecord.class), 'class lookup mismatch: ' .. playerClass.id .. ' / ' .. playerRecord.class)
assert(playerRace.id == string.lower(playerRecord.race), 'race lookup mismatch: ' .. playerRace.id .. ' / ' .. playerRecord.race)
assert(birthsignId == nil or birthsign ~= nil, 'birthsign lookup failed: ' .. tostring(birthsignId))
compat.recordFoundationProbe('player-bindings-records', true)

local strength = T.Actor.stats.attributes.strength(self)
local block = T.NPC.stats.skills.block(self)
local level = T.Actor.stats.level(self)
local health = T.Actor.stats.dynamic.health(self)
local originalStrength = strength.base
if compat.runtimeGeneration == 1 then
    strength.base = originalStrength + 1
    assert(strength.base == originalStrength + 1)
end
compat.recordFoundationProbe('player-bindings-mutation', true)
compat.recordFoundationProbe('player-bindings-stats', type(strength.modified) == 'number'
    and type(block.base) == 'number' and type(level.current) == 'number'
    and type(level.progress) == 'number' and type(health.base) == 'number'
    and type(health.current) == 'number' and not pcall(function() strength.modified = 0 end))

local spells = T.Player.spells(self)
local activeSpells = T.Actor.activeSpells(self)
compat.recordFoundationProbe('player-bindings-spells', type(spells) == 'table' and type(activeSpells) == 'table'
    and core.magic.spells.record ~= nil and core.magic.EFFECT_TYPE.FortifyAttribute == 'fortifyattribute')
local invalid = compat.probeInvalidHandles()
compat.recordFoundationProbe('player-bindings-handle-rejection', invalid.wrongType == 13 and invalid.stale == 12)

local failed = false
return { engineHandlers = { onUpdate = function()
    if not failed then failed = true; error('intentional Milestone 4.2 binding handler failure') end
end } }
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'player_bindings_safe.lua'), @'
local compat = require('openmw.compatibility')
local recorded = false
return { engineHandlers = { onUpdate = function()
    if not recorded then recorded = true; compat.recordFoundationProbe('player-bindings-failure-isolation', true) end
end } }
'@, $utf8NoBom)
            [IO.File]::WriteAllText((Join-Path $syntheticVfs 'milestone4-player-bindings.omwscripts'), @'
# Milestone 4.2 native player binding live fixture. Ordering is significant.
PLAYER: player_bindings.lua
PLAYER: player_bindings_safe.lua
'@, $utf8NoBom)
        }
    }
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'run.json') -Value ([ordered]@{
        protocolVersion = 1; runId = $runId; suite = $Suite; configuration = $Configuration
        repository = $repoRoot; morrowindDirectory = $morrowindRoot; fixtureSave = $FixtureSave; openMWAddonPath = if ($Suite -eq 'OpenMWAddon') { $OpenMWAddonPath } else { $null }
        openMWLuaScripts = if ($isOpenMWLuaSuite) { Join-Path $syntheticVfs $openMWLuaScriptsName } else { $null }
        startedAt = $startedAt.ToString('o')
    })

    if ($Suite -eq 'OpenMWAddon') {
        $dataDirectory = Join-Path $morrowindRoot 'Data Files'
        $discovery = Get-OpenMWAddonDiscovery -DataDirectory $dataDirectory -EnabledAddonPaths @($OpenMWAddonPath)
        if ($discovery.orderedAddons.Count -eq 0) { throw 'OpenMW addon suite did not discover an enabled addon.' }
        $primaryInspection = @($discovery.orderedAddons | Where-Object { $_.source.path -ieq [IO.Path]::GetFullPath($OpenMWAddonPath) })[0]
        if ($primaryInspection.masters.Count -ne 3 -or $primaryInspection.recordCounts.GMST -ne 8 -or $primaryInspection.recordCounts.SKIL -ne 27) {
            throw "NCG fixture structure mismatch: masters=$($primaryInspection.masters.Count), GMST=$($primaryInspection.recordCounts.GMST), SKIL=$($primaryInspection.recordCounts.SKIL)."
        }
        $cacheRoot = Join-Path $morrowindRoot 'Data Files\MWSE\tmp\openmw-addon-cache'
        $aliasMap = @{}
        $reportsDirectory = Join-Path $runDirectory 'compatibility-reports'
        [IO.Directory]::CreateDirectory($reportsDirectory) | Out-Null
        foreach ($inspection in $discovery.orderedAddons) {
            $preparation = New-OpenMWAddonNativeFile -Inspection $inspection -CacheRoot $cacheRoot -DependencyAliases $aliasMap
            $aliasMap[$inspection.source.filename] = $preparation.aliasName
            $sourceDirectory = [IO.Path]::GetDirectoryName($inspection.source.path).TrimEnd('\')
            if ($ProbeNativeAddonExtension -and $inspection.classification -eq 'direct-load' -and $sourceDirectory -ieq $dataDirectory.TrimEnd('\')) {
                $enabledNativeFiles.Add($inspection.source.filename)
            }
            else {
                Stage-File $preparation.nativePath (Join-Path $dataDirectory $preparation.aliasName)
                $enabledNativeFiles.Add($preparation.aliasName)
            }
            Copy-Item -LiteralPath $preparation.reportPath -Destination (Join-Path $reportsDirectory ($inspection.source.filename + '.json')) -Force
            $addonPreparations.Add($preparation)
        }
        Enable-TemporaryGameFiles @($enabledNativeFiles)
        Write-OwchAtomicJson -Path (Join-Path $runDirectory 'addon-plan.json') -Value ([ordered]@{
            schemaVersion = 1; enablement = $discovery.enablement; sourceIdentities = @($discovery.orderedAddons | ForEach-Object source)
            nativeExtensionProbe = $ProbeNativeAddonExtension.IsPresent
            enabledNativeFiles = @($enabledNativeFiles)
            nativeAliases = @($addonPreparations | ForEach-Object { [ordered]@{ source = $_.report.source.filename; alias = $_.aliasName; cacheKey = $_.cacheKey; cachePath = $_.nativePath } })
            originalGameFiles = $originalGameFiles
        })
    }

    if (-not $SkipBuild) {
        $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
        if (-not (Test-Path -LiteralPath $vswhere)) { throw "vswhere was not found: $vswhere" }
        $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
        if (-not $msbuild) { throw 'MSBuild could not be located with vswhere.' }
        $buildStage = Join-Path $runDirectory 'build-stage'
        [IO.Directory]::CreateDirectory($buildStage) | Out-Null
        $buildLog = Join-Path $runDirectory 'build.log'
        if ($isOpenMWLuaSuite) {
            $hostBuildArguments = @(
                (Join-Path $repoRoot 'OpenMWLuaTests\OpenMWLuaTests.vcxproj'), '/t:Build', "/p:Configuration=$Configuration", '/p:Platform=Win32',
                "/p:SolutionDir=$repoRoot/", '/nr:false'
            )
            & $msbuild @hostBuildArguments 2>&1 | Tee-Object -FilePath $buildLog
            if ($LASTEXITCODE -ne 0) { throw "OpenMW Lua host/test build failed with exit code $LASTEXITCODE. See $buildLog" }
            & (Join-Path $repoRoot "build\$Configuration\OpenMWLuaTests.exe") (Join-Path $repoRoot "build\$Configuration\openmw-lua.dll") 'C:\Games\Morrowind\Data Files\ncg.omwscripts' 2>&1 | Tee-Object -FilePath $nativeTestPath
            if ($LASTEXITCODE -ne 0) { throw "OpenMW Lua native tests failed with exit code $LASTEXITCODE. See $nativeTestPath" }
        }
        $buildArguments = @(
            (Join-Path $repoRoot 'MWSE\MWSE.vcxproj'), '/t:Build', "/p:Configuration=$Configuration", '/p:Platform=Win32',
            "/p:SolutionDir=$repoRoot/", "/p:MorrowindDir=$buildStage/", '/p:PostBuildEventUseInBuild=false', '/nr:false'
        )
        & $msbuild @buildArguments 2>&1 | Tee-Object -FilePath $buildLog -Append
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
    if ($isOpenMWLuaSuite) {
        Stage-File (Join-Path $buildOutput 'openmw-lua.dll') (Join-Path $morrowindRoot 'Data Files\MWSE\core\lib\openmw-lua.dll')
        Stage-File (Join-Path $repoRoot 'misc\package\Data Files\MWSE\core\lib\openmw-lua-LICENSE.txt') (Join-Path $morrowindRoot 'Data Files\MWSE\core\lib\openmw-lua-LICENSE.txt')
        foreach ($name in $openMWEnvironmentNames) { $originalOpenMWEnvironment[$name] = [Environment]::GetEnvironmentVariable($name, 'Process') }
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_VFS_ROOT', $syntheticVfs, 'Process')
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_SCRIPTS_FILE', $openMWLuaScriptsName, 'Process')
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_CONTENT_FILE', $openMWLuaScriptsName, 'Process')
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_REPORT_DIRECTORY', $runDirectory, 'Process')
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_HARNESS', '1', 'Process')
        [Environment]::SetEnvironmentVariable('MWSE_OPENMW_LUA_DISABLED', $null, 'Process')
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

    if ($Suite -eq 'OpenMWLuaHost') {
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaHostReport' | Out-Null
        Invoke-NamedProbe 'reloadOpenMWLuaHost' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaHostReport' | Out-Null
    }
    elseif ($Suite -eq 'OpenMWLuaFoundation') {
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaFoundationReport' | Out-Null
        Invoke-NamedProbe 'reloadOpenMWLuaHost' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaFoundationReport' | Out-Null
    }
    elseif ($Suite -eq 'OpenMWLuaPlayerBindings') {
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaPlayerBindingsReport' | Out-Null
        Invoke-NamedProbe 'openMWLuaPlayerNativeState' @{ phase = 'mutated' } | Out-Null
        Invoke-NamedProbe 'reloadOpenMWLuaHost' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Send-HarnessCommand 'ping' | Out-Null
        Invoke-NamedProbe 'openMWLuaPlayerBindingsReport' | Out-Null
        Invoke-NamedProbe 'openMWLuaPlayerNativeState' @{ phase = 'restored' } | Out-Null
        Invoke-NamedProbe 'mwseInitialization' | Out-Null
    }

    if ($Suite -eq 'OpenMWAddon') {
        $addonProbeArguments = @{ expectedAlias = $enabledNativeFiles[$enabledNativeFiles.Count - 1]; expectedOrdinaryFiles = @($originalGameFiles) }
        $addonProbe = Invoke-NamedProbe 'openMWAddonState' $addonProbeArguments
        Add-SmokeAssertion 'ncg-content-live-before-save' ($addonProbe.result.value.aliasActive -and $addonProbe.result.value.ordinaryFilesUnchanged -and $addonProbe.result.value.gmst.value -eq 0) $addonProbe.result.value $true
    }

    if ($Suite -ne 'OpenMWLuaPlayerBindings') {
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

        if ($Suite -eq 'OpenMWAddon') {
            $reloadedAddonProbe = Invoke-NamedProbe 'openMWAddonState' $addonProbeArguments
            Add-SmokeAssertion 'ncg-content-live-after-reload' ($reloadedAddonProbe.result.value.aliasActive -and $reloadedAddonProbe.result.value.gmst.value -eq 0) $reloadedAddonProbe.result.value $true
        }
    }

    if ($isOpenMWLuaSuite) {
        Invoke-NamedProbe 'shutdownOpenMWLuaHost' | Out-Null
    }
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
    foreach ($name in $openMWEnvironmentNames) {
        if ($originalOpenMWEnvironment.ContainsKey($name)) {
            [Environment]::SetEnvironmentVariable($name, $originalOpenMWEnvironment[$name], 'Process')
        }
    }
    $restoredStagedFiles = @($staged | ForEach-Object {
        $exists = Test-Path -LiteralPath $_.Target
        $hash = if ($exists -and (Test-Path -LiteralPath $_.Target -PathType Leaf)) { (Get-FileHash -LiteralPath $_.Target -Algorithm SHA256).Hash.ToLowerInvariant() } else { $null }
        [ordered]@{ target = $_.Target; originallyExisted = $_.Existed; originalSha256 = $_.OriginalSha256; restoredExists = $exists; restoredSha256 = $hash; restored = if ($_.Existed) { $exists -and ($_.IsDirectory -or $hash -eq $_.OriginalSha256) } else { -not $exists } }
    })
    $stagingRestored = @($restoredStagedFiles | Where-Object { -not $_.restored }).Count -eq 0
    if ($null -ne $originalIniHash) {
        $restoredIniHash = (Get-FileHash -LiteralPath (Join-Path $morrowindRoot 'Morrowind.ini') -Algorithm SHA256).Hash.ToLowerInvariant()
        $loadOrderRestored = $restoredIniHash -eq $originalIniHash
        $remainingAliases = @($addonPreparations | Where-Object { Test-Path -LiteralPath (Join-Path $morrowindRoot ('Data Files\' + $_.aliasName)) } | ForEach-Object aliasName)
        if (-not $loadOrderRestored -or $remainingAliases.Count -gt 0) {
            $failure = "Temporary OpenMW addon state was not fully restored. Morrowind.ini restored: $loadOrderRestored; remaining aliases: $($remainingAliases -join ', ')"
        }
    }
    $remainingMorrowindProcesses = @(Get-Process -Name 'Morrowind' -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Id)
    Write-OwchAtomicJson -Path (Join-Path $runDirectory 'restoration.json') -Value ([ordered]@{
        originalMorrowindIniSha256 = $originalIniHash; restoredMorrowindIniSha256 = $restoredIniHash
        loadOrderRestored = $loadOrderRestored; temporaryAliasesRemaining = if ($null -ne $originalIniHash) { $remainingAliases } else { @() }
        harnessConfigurationRestored = -not (Test-Path -LiteralPath (Join-Path $morrowindRoot 'Data Files\MWSE\config\openmw_compat_harness.json'))
        stagedFiles = $restoredStagedFiles; stagedFilesRestored = $stagingRestored
        syntheticFixtureRetained = if ($isOpenMWLuaSuite) { Test-Path -LiteralPath (Join-Path $syntheticVfs $openMWLuaScriptsName) } else { $null }
        remainingMorrowindProcesses = $remainingMorrowindProcesses
    })
    if (-not $stagingRestored -or $remainingMorrowindProcesses.Count -gt 0) {
        $failure = "Temporary staged files or Morrowind process state was not fully restored. stagedFilesRestored=$stagingRestored; remainingProcesses=$($remainingMorrowindProcesses -join ',')"
    }
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
        addon = if ($Suite -eq 'OpenMWAddon') { [ordered]@{ source = $OpenMWAddonPath; preparations = @($addonPreparations | ForEach-Object { $_.report }); loadOrderRestored = $loadOrderRestored; originalIniSha256 = $originalIniHash; restoredIniSha256 = $restoredIniHash } } else { $null }
        artifacts = [ordered]@{
            runDirectory = $runDirectory; events = $eventsPath; result = $resultPath; mwseLog = (Join-Path $runDirectory 'MWSE.log')
            addonPlan = if ($Suite -eq 'OpenMWAddon') { Join-Path $runDirectory 'addon-plan.json' } else { $null }
            compatibilityReports = if ($Suite -eq 'OpenMWAddon') { Join-Path $runDirectory 'compatibility-reports' } else { $null }
            restoration = Join-Path $runDirectory 'restoration.json'
            buildLog = if (-not $SkipBuild) { Join-Path $runDirectory 'build.log' } else { $null }
            nativeTests = if ($isOpenMWLuaSuite) { $nativeTestPath } else { $null }
            bridgeRuntimeReport = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'bridge-runtime-report.json' } else { $null }
            parsedContainerReport = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'parsed-container-report.json' } else { $null }
            handlerOrderReport = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'handler-order-report.json' } else { $null }
            foundationPackageReport = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'foundation-package-report.json' } else { $null }
            reloadShutdownReport = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'reload-shutdown-report.json' } else { $null }
            handleGenerationReport = if ($Suite -eq 'OpenMWLuaPlayerBindings') { Join-Path $runDirectory 'handle-generation-report.json' } else { $null }
            playerTypeReport = if ($Suite -eq 'OpenMWLuaPlayerBindings') { Join-Path $runDirectory 'player-type-report.json' } else { $null }
            recordsStatReport = if ($Suite -eq 'OpenMWLuaPlayerBindings') { Join-Path $runDirectory 'records-stat-report.json' } else { $null }
            mutationRestorationReport = if ($Suite -eq 'OpenMWLuaPlayerBindings') { Join-Path $runDirectory 'mutation-restoration-report.json' } else { $null }
            hostEvents = if ($isOpenMWLuaSuite) { Join-Path $runDirectory 'openmw-host-events.jsonl' } else { $null }
            syntheticFixture = if ($isOpenMWLuaSuite) { $syntheticVfs } else { $null }
            save = if (Test-Path -LiteralPath (Join-Path $runDirectory ($smokeSaveBase + '.ess'))) { Join-Path $runDirectory ($smokeSaveBase + '.ess') } else { $null }
        }
    }
    Write-OwchAtomicJson -Path $resultPath -Value $result
    Write-Output ($result | ConvertTo-Json -Depth 30)
}

if (-not $passed) { exit 1 }
exit 0
