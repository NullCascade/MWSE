# OpenMW Content and Lua Compatibility Design

Status: design and execution plan  
Primary integration fixture: `C:\Games\Morrowind\Data Files\ncg.omwaddon`  
Primary script fixture: `C:\Games\Morrowind\Data Files\ncg.omwscripts`  
OpenMW source reference: `E:\Projects\Morrowind\openmw`  
Target process: 32-bit `Morrowind.exe`

## Objective

Add two independent capabilities to MWSE:

1. Load compatible OpenMW `.omwaddon` content in the original Morrowind engine.
2. Run a reasonable majority of gameplay-oriented OpenMW Lua mods through a dedicated OpenMW Lua compatibility runtime.

MWSE Lua and OpenMW Lua are separate hosts. OpenMW scripts must not execute inside the MWSE Lua state and must not receive MWSE Lua tables, functions, or userdata. Both hosts may consume the same native engine hooks through a shared C++ event broker.

The compatibility target is behavioral usefulness, not a reimplementation of the OpenMW engine. APIs with meaningful Morrowind or MGE equivalents should work. APIs that depend on OpenMW-only rendering, physics, navigation, multiplayer, ESM4, or engine internals must report explicit limitations.

## Confirmed fixture facts

`ncg.omwaddon` is 11,218 bytes and is a classic TES3-format version-0 content file. It contains:

- one `TES3` header;
- eight `GMST` records;
- twenty-seven `SKIL` records;
- masters `Morrowind.esm`, `Tribunal.esm`, and `Bloodmoon.esm`;
- no OpenMW `FORM` header subrecord;
- no OpenMW-only top-level records.

Its only immediate native-loader incompatibility is expected to be discovery and filename handling for the `.omwaddon` extension. It is therefore a good first content-loading fixture.

`ncg.omwscripts` contains:

```text
MENU: scripts/NCG/ui/renderers.lua
GLOBAL: scripts/NCG/global.lua
PLAYER: scripts/NCG/player.lua
```

The NCG scripts currently require these OpenMW packages:

- `openmw.async`
- `openmw.core`
- `openmw.input`
- `openmw.interfaces`
- `openmw.self`
- `openmw.storage`
- `openmw.types`
- `openmw.ui`
- `openmw.util`
- `openmw.world`

NCG probes or uses the interfaces `Activation`, `AI`, `Constants`, `Controls`, `MarksmansEye`, `MWUI`, `Settings`, `SkillFramework`, `SkillProgression`, `StatsWindow`, `Templates`, `TooltipBuilders`, and several UI helper functions. Optional foreign-mod interfaces may remain absent, but the native OpenMW interfaces needed by NCG must be adapted or stubbed with correct feature-detection behavior.

## Non-goals

- Expanding MWSE Lua until OpenMW Lua scripts happen to run in it.
- Passing Lua values between the MWSE and OpenMW Lua states.
- Running OpenMW's built-in AI, player-controller, combat-controller, or engine-replacement scripts.
- Exact MyGUI rendering or pixel-identical OpenMW UI.
- OpenMW multiplayer behavior.
- ESM4 game support.
- Exact OpenMW navmesh, physics, or renderer behavior.
- Silently accepting unsupported APIs and returning plausible but incorrect values.

## Architecture

```text
                         Morrowind.exe
                               |
                    native engine/event bridge
                      /                    \
             MWSE Lua host          OpenMW Lua host
             Lua 5.1-DW             LuaJIT 2.1 / 5.1
                  |                         |
          tes3.*, tes3ui.*, mge.*       openmw.*
```

### OpenMW Lua support DLL

Add a new x86 C++ project to `MWSE.sln`. Its package output is:

```text
Data Files\MWSE\core\lib\openmw-lua.dll
```

The support DLL owns:

- its LuaJIT runtime and symbols;
- sandbox creation;
- the OpenMW-specific `require` loader;
- script containers and handler registries;
- OpenMW Lua userdata and value types;
- serialization, timers, storage, interfaces, and delayed events;
- bindings for `openmw.*` packages.

The MWSE DLL owns or exposes through a versioned native bridge:

- process and game lifecycle notifications;
- safe access to TES3 records, references, mobiles, cells, UI, audio, input, and MGE;
- stable native object identifiers and handle validation;
- save/load attachment points;
- shared native event hooks.

No `sol::object`, `lua_State*`, Lua registry reference, or Lua allocator may cross the DLL boundary. Bridge data consists of POD values, opaque native handles, callbacks, and serialized byte/string data.

### Native bridge requirements

The first bridge version must provide:

- ABI version and structure-size validation;
- structured logging callback;
- current lifecycle state;
- frame, simulation-time, and game-time callbacks;
- input callbacks and current input state;
- save, saved, load, and loaded callbacks;
- player and active-reference lookup;
- record and reference lookup by stable identity;
- safe property read/write operations needed by NCG;
- message-box and basic UI entry points;
- graceful runtime shutdown before MWSE detaches.

All opaque handles must be invalidatable. Every operation that dereferences a handle must validate it and return a structured error rather than trusting an old pointer.

### Script containers

The host recognizes the `.omwscripts` flags `GLOBAL`, `MENU`, `PLAYER`, `CUSTOM`, `LOAD`, and all documented record-type flags. Each script instance has:

- an isolated environment;
- context-appropriate packages;
- its own returned interface and handlers;
- its own serializable state and reliable timers;
- deterministic load order;
- a diagnostic identity containing content file, script path, context, and attached object where applicable.

Local containers are created lazily for active references. Inactive local state remains serialized without retaining a live Lua environment. Inventory-stack merging is a declared compatibility limitation because native Morrowind does not always preserve OpenMW's per-item identity.

### Persistence

The OpenMW host implements its own serializer for the documented serializable graph. It must reject cycles, aliased tables, functions, and unsupported userdata with a path to the bad value.

Persist:

- global, menu, and player script `onSave` data;
- local script path, initialization data, and `onSave` data keyed by stable reference identity;
- reliable timers;
- global and player storage sections;
- compatibility runtime version and migration metadata.

The serialized result may be stored as reserved binary strings in MWSE reference Lua data, or in a dedicated save record if size or lifecycle testing shows reference data is unsuitable. Lua values themselves never cross runtimes.

### API compatibility policy

Every public API member has one status:

- **Unstarted**: no implementation.
- **Stubbed**: symbol exists and produces an explicit unsupported/capability result.
- **Partial**: useful documented subset works and limitations are tested.
- **Implemented**: documented Morrowind-relevant behavior is covered.
- **Not applicable**: inherently OpenMW-only, with rationale.

Do not mark an API implemented merely because the symbol exists. Each implemented or partial member requires at least one automated unit or integration assertion.

Expose the OpenMW `core.API_REVISION` required by the selected compatibility baseline. Also expose an MWSE-specific capability table for diagnostics without requiring mods to use it.

## Agent execution rules

Each task below is intended to be independently assignable after its dependencies are complete.

An agent completing a task must:

1. Read this document and the referenced OpenMW API source for its assigned surface.
2. Preserve the separation between Lua runtimes.
3. Add or update tests in the same change.
4. Provide a reproducible command or harness request that demonstrates the result.
5. Record unsupported behavior explicitly.
6. Update the milestone task and API checklists in this document.
7. Avoid unrelated refactors and generated documentation edits.

An integration task is not complete when it merely builds. It must pass its stated gate through the automated Morrowind harness.

## Milestone 1: Automated Morrowind test harness

Goal: agents can build, launch, command, observe, and terminate Morrowind without manually operating the UI.

### M1.1 Harness protocol

- [x] Define a versioned JSON request/response protocol.
- [x] Assign each run and request a unique ID.
- [x] Define `ready`, `heartbeat`, `response`, `assertion`, `log`, `fatal`, and `shutdown` messages.
- [x] Use append-only JSON Lines for process output so partial writes are detectable.
- [x] Write commands atomically through a temporary file followed by rename.
- [x] Include game state in relevant messages: main menu, loading, in game, paused, current cell, player validity.
- [x] Bound every request with a timeout and return a diagnostic snapshot on timeout.

Proposed runtime directory:

```text
Data Files\MWSE\tmp\openmw-compat-harness\
    command.json
    events.jsonl
    ready.json
    run.json
    screenshots\
```

### M1.2 MWSE Lua harness mod

- [x] Add a packaged MWSE Lua test mod at `misc\package\Data Files\MWSE\mods\openmw_compat_harness\main.lua` dedicated to external automation.
- [x] Poll for commands without blocking the simulation thread.
- [x] Implement `ping`, `getState`, `evalNamedProbe`, `loadGame`, `newGame`, `teleport`, `waitForEvent`, `screenshot`, and `shutdown` commands.
- [x] Implement named probes rather than unrestricted Lua evaluation for normal tests.
- [x] Catch errors and return traceback, current game state, and recent MWSE log lines.
- [x] Emit heartbeat messages while Morrowind remains responsive.
- [x] Disable the harness by default outside explicit test launches.

### M1.3 Command-line launcher

- [x] Add a repository PowerShell launcher under `tools/openmw-compat/`.
- [x] Locate MSBuild with `vswhere` and build the requested x86 configuration.
- [x] Stage only the required build outputs and harness files.
- [x] Create a fresh run directory and configuration.
- [x] Launch `Morrowind.exe` with the Morrowind directory as its working directory.
- [x] Wait for the ready handshake and fail clearly if MWSE or the harness did not initialize.
- [x] Send a selected test suite and stream structured results.
- [x] Request graceful shutdown, then report process exit code and retained artifacts.
- [x] Never edit the user's normal load order permanently; back up and restore any temporary configuration.

### M1.4 Harness smoke suite

- [x] Verify MWSE initialization.
- [x] Verify main-menu detection.
- [x] Start or load a deterministic game fixture.
- [x] Verify player and current-cell access.
- [x] Trigger and observe one native event.
- [x] Write and read one reference-persistent value across save/load.
- [x] Shut down without leaving Morrowind running.

Milestone gate:

```text
One command builds MWSE, launches Morrowind, reaches a known game state,
executes named probes, captures structured logs, and exits with a machine-readable result.
```

### Milestone 1 implementation record

Milestone 1 passed on 2026-07-12 with protocol version 1 and run ID `20260713T020725Z-df8fb192c8f540bbbeb03a35abe724d2`. The retained `result.json` reports `passed: true`, process exit code 0, seven passing launcher assertions, and ten passing in-game assertions. The run used `C:\Games\Morrowind`, x86 Debug, and the configurable `TestMWSE0000.ess` fixture.

Implementation decisions and observed limitations:

- Runtime files live in a unique child of `Data Files\MWSE\tmp\openmw-compat-harness`; the parent is stable while every run remains append-only and independently inspectable.
- MWSE discovers the staged `mods\openmw_compat_harness\main.lua` without an INI or load-order edit. A temporary config containing `enabled = true` and the run identity is the only enable switch. The launcher restores all staged files in `finally` and moves its generated save out of the normal `Saves` directory.
- `tes3.loadGame` does not consistently return its documented boolean in this build. The version-1 protocol treats the native `loaded` event as authoritative completion evidence.
- `paused` reports native Morrowind menu mode. It is a stable automation signal, but it does not distinguish every engine-level simulation pause reason.
- MGE can own the top-level window, making `tes3.game.windowHandle` unsuitable for `WM_CLOSE`. Lua flushes the response and shutdown record first; the launcher then calls `CloseMainWindow` on the exact process it started. The verified process exit code was 0.
- Screenshot capture is implemented as a bounded MGE request. On D3D8/MGE configurations where framebuffer capture does not materialize a file, the command returns `screenshot_timeout` with a diagnostic instead of claiming success. Screenshot capture is not part of the Milestone 1 smoke gate.
- The harness intentionally leaves ordinary Lua mods enabled because changing the user's load order would violate isolation. Errors from unrelated installed mods can therefore appear in retained `MWSE.log`; harness assertions and protocol failures remain independently structured.

## Milestone 2: `.omwaddon` loading

Goal: `ncg.omwaddon` is discovered, selected, loaded, and reflected in game data.

### M2.1 Content inspector

- [x] Implement a bounded TES3 record/subrecord inspector independent of the native loader.
- [x] Read `TES3/HEDR`, optional `FORM`, `MAST/DATA`, record flags, and top-level record names.
- [x] Validate record boundaries and reject truncation or overflow.
- [x] Classify files as direct-load, transformable, unsupported, or malformed.
- [x] Inventory OpenMW-only records and unsupported format versions.
- [x] Add unit fixtures for format 0, format 1 with `FORM`, malformed sizes, unknown records, and dependency chains.

### M2.2 Discovery and load-order integration

- [x] Discover `.omwaddon` files without changing ordinary `.esm`/`.esp` behavior.
- [x] Define how addons are enabled for Morrowind tests and normal use.
- [x] Resolve addon masters and addon-to-addon dependencies case-insensitively.
- [x] Detect duplicate filenames and cyclic or missing dependencies.
- [x] Preserve the original addon filename for diagnostics and source-content identity.
- [x] Ensure savegame/load-order warnings show useful names.

### M2.3 Native-compatible transformation

- [x] Direct-load format-0 files when safe.
- [x] For format-1 content, remove or adapt the `FORM` subrecord without corrupting `TES3` record sizes.
- [x] Rewrite dependency names only when native resolution requires a cached alias.
- [x] Cache transformed files by source path, size, modification time, and content hash.
- [x] Never overwrite the source `.omwaddon`.
- [x] Reject OpenMW-only records whose omission would change mod behavior.
- [x] Produce a compatibility report listing every transformation and ignored feature.

### M2.4 NCG addon integration test

- [x] Load `ncg.omwaddon` from a configurable fixture path.
- [x] Assert the three declared masters resolve.
- [x] Assert all eight expected `GMST` records are present or modified as expected.
- [x] Assert all twenty-seven `SKIL` records load without record-boundary errors.
- [x] Query representative NCG GMST and skill-description values through the harness.
- [x] Save and reload a game with the addon active.
- [x] Confirm ordinary ESP/ESM loading remains unchanged.

Milestone gate:

```text
The automated harness launches Morrowind with ncg.omwaddon active and proves,
through game-memory queries, that its GMST and SKIL changes were loaded.
```

### Milestone 2 implementation record

Milestone 2 passed on 2026-07-12 under build-enabled run ID `20260713T022608Z-e077f4be2f114afaa0d78d28d273862e`. Its `result.json` reports `passed: true`, process exit code 0, all launcher and in-game assertions passing, and byte-identical restoration of `Morrowind.ini` (`722bc9a0f1e4bf83cc7d56c1c00df04f20a9a9835d0cbef45c05ef1695ec5f9f` before and after). The live probe found the identity-preserving native alias `ncg.omwaddon-c67a7850d2c89833.esp`, preserved all five pre-existing ESMs, read `iLevelupMajorMult` as `0`, and read the NCG Block description text before save and after reload.

Evidence-backed decisions and limitations:

- The independent PowerShell inspector bounds the whole input to 256 MiB by default and validates every top-level record and TES3-header subrecord using widened offsets before reading. It supports TES3 HEDR versions 1.2/1.3 and OpenMW format versions 0/1. Unknown, OpenMW-only, malformed, and newer-format content is rejected rather than omitted.
- Format-0 content bytes are copied unchanged when an extension alias is required. Format-1 transformation removes the 12-byte `FORM` subrecord and rewrites the enclosing TES3 size. Addon master names are rewritten only when the referenced addon has a native alias.
- Native direct-extension evidence is retained under failed diagnostic run `20260713T022235Z-29fbf98abdaf496ca1a8645878da04ec`: `GameFileN=ncg.omwaddon` was ignored, the addon was absent from `activeMods`, and the queried values remained vanilla. Native loading therefore requires a cached `.esp` alias even for otherwise direct-load-safe format-0 content.
- Cache identity includes normalized source path, source size, modification time, and SHA-256 content hash. The source is never modified. Compatibility reports preserve the source path and exact original filename; native aliases retain the original `.omwaddon` name visibly so save/load warnings remain actionable.
- Tests opt in with `-Suite OpenMWAddon -OpenMWAddonPath <path>`. Normal users opt in through `Data Files\MWSE\config\openmw-addon-loader.json` and `Start-MorrowindWithOpenMWAddons.ps1`. Both mechanisms restore temporary aliases and `Morrowind.ini`; cached outputs and reports remain under `Data Files\MWSE\tmp\openmw-addon-cache`.
- The final retained run contains `result.json`, `events.jsonl`, `MWSE.log`, `build.log`, `addon-plan.json`, the active INI snapshot, per-addon compatibility reports, `restoration.json`, and the save/reload fixture. No OpenMW Lua DLL or runtime work was started.

### M2.5 Construction Set follow-up

- [x] Reuse the inspector and transformation policy in CSSE.
- [x] Display `.omwaddon` files in the Data Files dialog.
- [x] Initially treat addon inputs as read-only and save edits to an ESP.
- [x] Warn before operations that would discard unsupported OpenMW records.
- [x] Add a CSSE-specific headless or harness-assisted loading test.

This follow-up is not a dependency for the OpenMW Lua runtime.

### Milestone 2.5 implementation record

Milestone 2.5 passed on 2026-07-12 under native Construction Set harness run ID `20260713T024258Z-068fa508c174427d8730bc181cbc7d24`. The machine-readable result reports `passed: true`, process exit code 0, and native active files `Morrowind.esm`, `Bloodmoon.esm`, `Tribunal.esm`, and `ncg.omwaddon-c67a7850d2c89833.esp`. CSSE recovered the original source identity as `ncg.omwaddon`. The harness restored the pre-existing `csse.toml` and `CSSE.dll` to their exact SHA-256 hashes and left zero temporary aliases.

Implementation decisions and evidence:

- CSSE sessions reuse `OpenMWAddon.psm1` for the same bounded inspection, dependency resolution, format-1 transformation, rejection policy, cache identity, and compatibility report used by the Morrowind harness. `Start-ConstructionSetWithOpenMWAddons.ps1` stages only accepted native aliases for the editor session and restores them on exit.
- The Data Files dialog hook recognizes the identity-preserving `<source>.omwaddon-<hash>.esp` cache name and displays the exact original `.omwaddon` source name. Ordinary ESM/ESP rows remain unchanged.
- An addon row cannot be set active or merged to masters, and its author and summary fields are disabled. Attempted activation explains that the input is read-only, writing back could discard OpenMW-specific data, and edits must use an ESP.
- The test-only native hook is enabled only through `MWSE_CSSE_OPENMW_ADDON_TEST_RESULT`. After the normal Construction Set loader finishes, it records the active native files and source identity, then exits. It is inert in ordinary sessions.
- Offline CSSE run `20260713T023804Z-29c05edb500841228786aa97fb236732` additionally verified the NCG inventory (three masters, eight GMST, twenty-seven SKIL), unchanged source/native hashes for format 0, exact display identity, read-only policy, compiled dialog warning, and built Debug CSSE DLL.
- The final targeted x86 Debug CSSE build completed with zero warnings and zero errors using post-build deployment disabled. No OpenMW Lua runtime work was started.

## Milestone 3: OpenMW Lua DLL and host skeleton

Goal: an isolated LuaJIT host loads beside MWSE Lua and runs a trivial `.omwscripts` fixture.

### M3.1 Solution and packaging

- [x] Add the x86 support-DLL project to `MWSE.sln`.
- [x] Add isolated LuaJIT dependencies without colliding with MWSE's Lua symbols.
- [x] Emit `openmw-lua.dll` into the package `Data Files\MWSE\core\lib` directory.
- [x] Load the DLL explicitly from MWSE and validate its bridge ABI.
- [x] Log runtime version, API revision, and bridge version at startup.
- [x] Ensure missing or incompatible DLLs fail without breaking MWSE Lua.

### M3.2 Sandbox and module loader

- [x] Create an isolated environment per script instance.
- [x] Expose only the documented safe Lua libraries and functions.
- [x] Support OpenMW's documented Lua 5.2/5.3 compatibility features through LuaJIT and compatibility libraries.
- [x] Implement `require` for standard libraries, built-in packages, compatibility auxiliaries, and source files.
- [x] Reject DLL modules and precompiled Lua bytecode.
- [x] Resolve module paths through the compatibility VFS with deterministic priority.
- [x] Include script path and context in all load/runtime errors.

### M3.3 `.omwscripts` parser

- [x] Parse comments, flags, commas, whitespace, and paths.
- [x] Preserve content-file and line ordering.
- [x] Validate mutually incompatible flags.
- [x] Produce actionable errors for missing scripts and unknown flags.
- [x] Parse the NCG file into one menu, one global, and one player definition.

### M3.4 Minimal containers

- [x] Implement menu, global, and player containers.
- [x] Execute a script and validate its returned table.
- [x] Register `interfaceName`, `interface`, `engineHandlers`, and `eventHandlers`.
- [x] Preserve direct engine-handler order and reverse event-handler order.
- [x] Implement delayed script events.
- [x] Isolate failure to the offending handler and continue other scripts where safe.
- [x] Support clean shutdown and reload.

Milestone gate:

```text
MWSE Lua and the OpenMW LuaJIT host initialize in the same process. A synthetic
.omwscripts file runs menu/global/player scripts and reports handlers through the harness.
```

### Milestone 3 implementation record

Milestone 3 passed the x86 Debug in-process gate on 2026-07-12 under run ID `20260713T031035Z-afdd65c9d4a448dcb56960f9d9a00e2a`. Its machine-readable `result.json` reports `passed: true`, process exit code 0, all launcher and game assertions passing, an explicit clean OpenMW runtime shutdown, and no remaining Morrowind process. The unchanged Milestone 1 regression then passed under run ID `20260713T031121Z-8d5ff6305cda45b7b11f5860f35c8f9d`.

Evidence-backed implementation decisions and limitations:

- `OpenMWLua\OpenMWLua.vcxproj` builds an x86 `openmw-lua.dll` and packages it at `Data Files\MWSE\core\lib\openmw-lua.dll`. LuaJIT 2.1 is rebuilt as a static library in an isolated intermediate source copy and linked into the support DLL. PE inspection found exactly one support-DLL export (`OpenMWLua_QueryApi`), no `lua_*` exports or imports, no `lua51.dll` import, and only `KERNEL32.dll` as a runtime dependency. MWSE continues to import its existing `lua51.dll`; the OpenMW host owns a different `lua_State`, allocator context, registry, environments, and error path.
- The version-1 C ABI uses fixed-width POD structures, explicit `structureSize` and `abiVersion` fields, bounded pointer/length strings, a required structured-log callback, and function pointers for initialization, shutdown, lifecycle state, frame updates, delayed events, report retrieval, and reload. No Lua state, allocator, registry reference, `sol` object, table, function, or userdata crosses the DLL boundary. API revision 70 is selected for feature reporting, while gameplay packages remain explicitly unavailable until Milestone 4.
- MWSE loads the DLL by absolute package path after normal MWSE Lua mod startup. Missing entry points, ABI/layout mismatches, missing callbacks, disabled configuration, missing DLLs, and host initialization failures leave MWSE Lua active. Startup logs the LuaJIT version, API revision, bridge/ABI versions, capability mask, lifecycle state, and status. Shutdown is invoked before MWSE Lua cleanup.
- The host opens unsafe libraries only in its inaccessible owner state, then constructs every script environment from a safe allowlist and per-environment clones of `coroutine`, `math`, `string`, and `table`. The custom `require` searches safe libraries, the explicit `openmw.compatibility` capability package, compatibility auxiliary Lua paths, then the compatibility VFS; it accepts source text only and rejects native modules, bytecode, absolute paths, traversal, oversized names/files, and invalid handles/indices. The capability package reports gameplay bindings unavailable instead of returning plausible values.
- The bounded `.omwscripts` parser limits the file to 1 MiB and lines to 4096 bytes; recognizes `GLOBAL`, `MENU`, `PLAYER`, `CUSTOM`, `LOAD`, and the documented TES3 record-type flags; preserves content identity, source line, and declaration order; and rejects malformed lines, missing paths/scripts, duplicate flags, unknown flags, incompatible declarations, absolute paths, and traversal. Native tests parsed `C:\Games\Morrowind\Data Files\ncg.omwscripts` into exactly one menu, one global, and one player definition without executing NCG.
- Container instances validate a table return and register `interfaceName`, `interface`, `engineHandlers`, and `eventHandlers`. The retained live report shows three unique generation-2 environment identities with `mwseGlobalVisible: false`, interfaces `MenuFixture`, `GlobalFixture`, and `PlayerFixture`, engine order `MENU`, `GLOBAL`, `PLAYER`, reverse event order `PLAYER`, `GLOBAL`, `MENU`, and delayed delivery `1:Milestone3Event`. The intentional GLOBAL failure includes content, container, script, handler, and Lua location; the MENU handler still ran afterward.
- Explicit reload destroyed generation 1, recreated generation 2, reran all three isolated scripts, and redelivered the deterministic event. The final named shutdown probe recorded lifecycle state 6 (`Stopped`) and `cleanShutdown: true` before Morrowind detach. MWSE Lua reported `Lua 5.1-DW` and build 65535 before reload, after reload, and after OpenMW host shutdown.
- The standalone x86 native runner covers ABI and size negotiation, missing callbacks, parser success/rejections, the real NCG parser count, sandbox isolation, deterministic source-module loading, DLL/bytecode rejection, returned-table validation, handler order, failure isolation, delayed delivery, reload, and shutdown. The live suite retains `native-tests.json`, build log, MWSE log, host JSONL, bridge/runtime report, parsed-container report, handler-order report, reload/shutdown report, protocol events, result, save fixture, and the exact synthetic VFS inputs.
- Restoration evidence records matching pre/post hashes for pre-existing MWSE, Lua, support-DLL, and license files; removes the temporary harness mod/config; retains the synthetic fixture only inside the unique evidence directory; and reports zero remaining Morrowind processes. The completed addon loader, cache, source-immutability, load-order, and CSSE protocols were not redesigned.
- Milestone 3 deliberately does not execute NCG or implement the NCG-required `openmw.*` packages, persistence, UI, storage, stats, gameplay bindings, local/reference containers, or unrestricted Lua evaluation. Those surfaces remain Milestone 4 and later work.

## Milestone 4: NCG boot compatibility

Goal: all three NCG scripts load and reach their initialization handlers without missing-package or syntax errors.

### M4.1 Foundation packages

- [ ] Implement the NCG-required subset of `openmw.util`.
- [ ] Implement `openmw.interfaces` and mod-defined interface registration/lookup.
- [ ] Implement simulation and game-time `openmw.async` timers plus `async.callback`.
- [ ] Implement global and player `openmw.storage` sections and subscriptions.
- [ ] Implement the NCG-required subset of `openmw.core`.
- [ ] Implement `openmw.self` for the player container.

### M4.2 Player and record/stat bindings

- [ ] Implement stable player `GameObject` userdata.
- [ ] Implement cells and the player cell properties needed by NCG.
- [ ] Implement `openmw.types.Actor`, `NPC`, and `Player` detection.
- [ ] Implement attributes, skills, level, health, spell, class, race, and birth-sign access used by NCG.
- [ ] Ensure mutable stat proxy writes update native values correctly.
- [ ] Implement record collections and lowercase ID lookup behavior used by NCG.
- [ ] Validate handles on every call.

### M4.3 Input and engine handlers

- [ ] Implement action registration and action handlers used by NCG.
- [ ] Map `onFrame`, `onUpdate`, `onInit`, `onActive`, `onLoad`, and `onSave`.
- [ ] Map NCG-relevant `UiModeChanged`, `Died`, and skill-level events.
- [ ] Implement global events and player-local events with OpenMW ordering and one-frame delay.
- [ ] Confirm optional third-party interfaces remain safely detectable as absent.

### M4.4 NCG settings and UI baseline

- [ ] Provide the built-in `Settings` interface subset used by NCG.
- [ ] Provide `Controls` feature detection expected by NCG.
- [ ] Implement `ui.showMessage`.
- [ ] Implement the NCG-required `ui.create`, content, element update/destroy, text, image, container, and flex subset.
- [ ] Provide the MWUI/template constants NCG needs, or a compatibility adapter with equivalent results.
- [ ] Implement localization lookup sufficient for NCG's `core.l10n` calls.
- [ ] Warn once for unsupported cosmetic layout properties.

Milestone gate:

```text
NCG's menu, global, and player scripts all load; their onInit/onActive handlers run;
the settings surface is registered; and no required module or built-in interface is missing.
```

## Milestone 5: NCG functional behavior and persistence

Goal: NCG performs its core leveling behavior and survives save/load.

### M5.1 Serializer

- [ ] Serialize nil, booleans, numbers, strings, supported value userdata, object handles, and tables.
- [ ] Detect cycles and shared table references.
- [ ] Report the data path to unsupported values.
- [ ] Version serialized data and support migrations.
- [ ] Fuzz or property-test malformed serialized input.

### M5.2 Script and storage persistence

- [ ] Call each script's `onSave` and restore through `onLoad`.
- [ ] Persist reliable timers and registered callback names.
- [ ] Persist global and player storage sections.
- [ ] Prevent state from leaking between savegames or new games.
- [ ] Handle mid-game installation where no prior script data exists.
- [ ] Handle removal or renaming of a script between save and load.

### M5.3 NCG behavioral assertions

- [ ] NCG initializes a player profile.
- [ ] Attribute and skill state can be read through OpenMW-compatible proxies.
- [ ] Raising a skill triggers NCG's progression path.
- [ ] NCG updates attribute growth and level progress.
- [ ] Health recalculation produces native Morrowind health changes.
- [ ] Player death updates the expected storage value.
- [ ] NCG messages and logs appear through the compatibility UI.
- [ ] Save, exit, relaunch, and load restore NCG state.
- [ ] Existing MWSE Lua mods and persistent timers still work in the same run.

Milestone gate:

```text
The harness performs a deterministic NCG progression scenario, saves, restarts
Morrowind, reloads, and verifies the same NCG state and native player statistics.
```

## Milestone 6: General gameplay-mod compatibility

Goal: extend from the NCG vertical slice to common OpenMW gameplay patterns.

### M6.1 Local script containers

- [ ] Implement documented record-type autostart flags.
- [ ] Create local containers lazily for active references.
- [ ] Implement `onActive`, `onInactive`, `onActivated`, `onConsume`, and `onTeleported`.
- [ ] Implement dynamic `CUSTOM` attach, query, and detach.
- [ ] Persist local state by stable reference identity.
- [ ] Handle reference deletion and invalidation.
- [ ] Document inventory-stack identity limitations with tests.

### M6.2 World and nearby access

- [ ] Implement object lookup by form/reference identity.
- [ ] Implement active-cell object lists and filters.
- [ ] Implement cell lookup and basic world mutation.
- [ ] Implement raycasts through native collision where meaningful.
- [ ] Classify navmesh/pathfinding APIs as partial or not applicable unless a native equivalent is added.

### M6.3 Broader packages

- [ ] Implement common audio APIs.
- [ ] Implement common animation APIs.
- [ ] Implement camera mappings, using MGE only where required.
- [ ] Implement VFS file access and archive behavior.
- [ ] Expand declarative UI coverage based on real mod fixtures.
- [ ] Map postprocessing only where MGE provides a defensible equivalent.

### M6.4 Compatibility corpus

- [ ] Establish a redistributable or locally configured corpus of representative OpenMW mods.
- [ ] Add a static scanner that reports required packages, members, handlers, interfaces, and Lua syntax features.
- [ ] Generate a per-mod capability report before launch.
- [ ] Add one deterministic harness scenario per supported mod.
- [ ] Use corpus failures to prioritize API work rather than implementing the appendix in arbitrary order.

Milestone gate:

```text
The compatibility corpus runs through the automated harness with a published
per-mod status and no silent unsupported-API fallbacks.
```

## Milestone 7: Hardening and release requirements

- [ ] Crash containment for Lua panics and C++ exceptions.
- [ ] Per-script instruction and memory budgets with useful diagnostics.
- [ ] Deterministic cleanup on new game, load, return to menu, reload, and process shutdown.
- [ ] Stable bridge ABI validation.
- [ ] Malformed addon, script, VFS, and save-data tests.
- [ ] Load-order and dependency diagnostics.
- [ ] User-facing compatibility report and log locations.
- [ ] Documentation for installing `.omwaddon` and `.omwscripts` content.
- [ ] Documentation for API status and known semantic differences.
- [ ] Release packaging includes the support DLL and no test harness unless explicitly enabled.
- [ ] License review before incorporating any OpenMW implementation or auxiliary Lua source; prefer clean-room implementation from documented behavior.

## Global success conditions

1. MWSE Lua behavior and ABI remain unchanged for existing mods.
2. Failure to load the OpenMW support DLL does not prevent normal MWSE startup.
3. Unsupported OpenMW APIs fail explicitly and identify script, context, and member.
4. No OpenMW Lua value crosses into the MWSE Lua state or vice versa.
5. All native object handles are validated before use.
6. `.omwaddon` sources are never overwritten by runtime transformation.
7. Automated Morrowind integration tests are reproducible from one command.
8. NCG content changes and Lua behavior are verified from game state rather than log text alone.
9. Save/load tests include a full process restart.
10. Every checked API member has an associated test or an explicit not-applicable rationale.

## API tracking inventory

Source of truth: `E:\Projects\Morrowind\openmw\files\lua_api\openmw\*.lua` as inspected for this design. Re-run the inventory when updating the targeted OpenMW revision.

The inspected source contains 569 function declarations representing 567 unique callable names, 192 type declarations representing 187 unique type names, and 1,512 field declarations. The checklist below contains every unique callable name and every unique type name. Overloaded declarations share one callable checklist item and require coverage for every documented overload.

- [ ] Add `tools\openmw-compat\generate-api-inventory.ps1` to regenerate or validate this appendix from an explicitly configured OpenMW checkout.
- [ ] Make inventory validation report added, removed, and changed declarations without editing the document silently.

Checkbox meaning in this appendix:

- `[ ]` unstarted or not yet verified;
- `[x]` implemented or deliberately classified, with tests and any limitation recorded beside it.

Types and public fields are tracked after the callable-member list. A type is not complete until its documented fields, constants, mutability, indexing behavior, and callable members are covered or individually classified.

### `openmw.ambient`

- [ ] `ambient.playSound`
- [ ] `ambient.playSoundFile`
- [ ] `ambient.stopSound`
- [ ] `ambient.stopSoundFile`
- [ ] `ambient.isSoundPlaying`
- [ ] `ambient.isSoundFilePlaying`
- [ ] `ambient.streamMusic`
- [ ] `ambient.stopMusic`
- [ ] `ambient.isMusicPlaying`
- [ ] `ambient.say`
- [ ] `ambient.stopSay`
- [ ] `Sound.isSayActive`

### `openmw.animation`

- [ ] `animation.hasAnimation`
- [ ] `animation.skipAnimationThisFrame`
- [ ] `animation.getTextKeyTime`
- [ ] `animation.isPlaying`
- [ ] `animation.getCurrentTime`
- [ ] `animation.isLoopingAnimation`
- [ ] `animation.cancel`
- [ ] `animation.setLoopingEnabled`
- [ ] `animation.getCompletion`
- [ ] `animation.getLoopCount`
- [ ] `animation.getSpeed`
- [ ] `animation.setSpeed`
- [ ] `animation.clearAnimationQueue`
- [ ] `animation.playQueued`
- [ ] `animation.playBlended`
- [ ] `animation.hasGroup`
- [ ] `animation.hasBone`
- [ ] `animation.getActiveGroup`
- [ ] `animation.addVfx`
- [ ] `animation.removeVfx`
- [ ] `animation.removeAllVfx`

### `openmw.async`

- [ ] `async.registerTimerCallback`
- [ ] `async.newSimulationTimer`
- [ ] `async.newGameTimer`
- [ ] `async.newUnsavableSimulationTimer`
- [ ] `async.newUnsavableGameTimer`
- [ ] `async.callback`

### `openmw.camera`

- [ ] `camera.getMode`
- [ ] `camera.getQueuedMode`
- [ ] `camera.setMode`
- [ ] `camera.allowCharacterDeferredRotation`
- [ ] `camera.showCrosshair`
- [ ] `camera.getTrackedPosition`
- [ ] `camera.getPosition`
- [ ] `camera.getPitch`
- [ ] `camera.setPitch`
- [ ] `camera.getYaw`
- [ ] `camera.setYaw`
- [ ] `camera.getRoll`
- [ ] `camera.setRoll`
- [ ] `camera.getExtraPitch`
- [ ] `camera.setExtraPitch`
- [ ] `camera.getExtraYaw`
- [ ] `camera.setExtraYaw`
- [ ] `camera.getExtraRoll`
- [ ] `camera.setExtraRoll`
- [ ] `camera.setProjectionOffset`
- [ ] `camera.getProjectionOffset`
- [ ] `camera.setStaticPosition`
- [ ] `camera.getFirstPersonOffset`
- [ ] `camera.setFirstPersonOffset`
- [ ] `camera.getFocalPreferredOffset`
- [ ] `camera.setFocalPreferredOffset`
- [ ] `camera.getThirdPersonDistance`
- [ ] `camera.setPreferredThirdPersonDistance`
- [ ] `camera.getFocalTransitionSpeed`
- [ ] `camera.setFocalTransitionSpeed`
- [ ] `camera.instantTransition`
- [ ] `camera.getCollisionType`
- [ ] `camera.setCollisionType`
- [ ] `camera.getBaseFieldOfView`
- [ ] `camera.getFieldOfView`
- [ ] `camera.setFieldOfView`
- [ ] `camera.getBaseViewDistance`
- [ ] `camera.getViewDistance`
- [ ] `camera.setViewDistance`
- [ ] `camera.getViewTransform`
- [ ] `camera.viewportToWorldVector`
- [ ] `camera.worldToViewportVector`

Types: `MODE`.

### `openmw.content`

- [ ] `GMSTContent.getFallbacks`

### `openmw.core`

- [ ] `core.quit`
- [ ] `core.sendGlobalEvent`
- [ ] `core.getSimulationTime`
- [ ] `core.getSimulationTimeScale`
- [ ] `core.getGameTime`
- [ ] `core.getGameTimeScale`
- [ ] `core.isWorldPaused`
- [ ] `core.getRealTime`
- [ ] `core.getRealFrameDuration`
- [ ] `core.getGMST`
- [ ] `core.getGameDifficulty`
- [ ] `core.l10n`
- [ ] `ContentFiles.indexOf`
- [ ] `ContentFiles.has`
- [ ] `core.getFormId`
- [ ] `GameObject.isValid`
- [ ] `GameObject.sendEvent`
- [ ] `GameObject.activateBy`
- [ ] `GameObject.addScript`
- [ ] `GameObject.hasScript`
- [ ] `GameObject.removeScript`
- [ ] `GameObject.setScale`
- [ ] `GameObject.teleport`
- [ ] `GameObject.moveInto`
- [ ] `GameObject.remove`
- [ ] `GameObject.split`
- [ ] `GameObject.getBoundingBox`
- [ ] `Cell.hasTag`
- [ ] `Cell.isInSameSpace`
- [ ] `Cell.getAll`
- [ ] `PathGrid.getPoints`
- [ ] `Inventory.countOf`
- [ ] `Inventory.getAll`
- [ ] `Inventory.find`
- [ ] `Inventory.resolve`
- [ ] `Inventory.isResolved`
- [ ] `Inventory.findAll`
- [ ] `Land.getHeightAt`
- [ ] `Land.getTextureAt`
- [ ] `Spells.createRecordDraft`
- [ ] `Enchantments.createRecordDraft`
- [ ] `Sound.isEnabled`
- [ ] `Sound.playSound3d`
- [ ] `Sound.playSoundFile3d`
- [ ] `Sound.stopSound3d`
- [ ] `Sound.stopSoundFile3d`
- [ ] `Sound.isSoundPlaying`
- [ ] `Sound.isSoundFilePlaying`
- [ ] `Sound.say`
- [ ] `Sound.stopSay`
- [ ] `Sound.isSayActive`
- [ ] `Attribute.record`
- [ ] `Skill.record`
- [ ] `RegionRecord.setProbability`
- [ ] `RegionRecord.resetProbability`
- [ ] `Weather.getCurrent`
- [ ] `Weather.getNext`
- [ ] `Weather.getTransition`
- [ ] `Weather.changeWeather`
- [ ] `Weather.getCurrentSunLightDirection`
- [ ] `Weather.getCurrentSunVisibility`
- [ ] `Weather.getCurrentSunPercentage`
- [ ] `Weather.getCurrentWindSpeed`
- [ ] `Weather.getCurrentStormDirection`

Types: `ActiveEffect`, `ActiveSpell`, `ActiveSpellEffect`, `Attribute`, `AttributeRecord`, `Cell`, `ContentFiles`, `DialogueConditionOperator`, `DialogueConditionType`, `DialogueInfoCondition`, `DialogueRecord`, `DialogueRecordInfo`, `Enchantment`, `EnchantmentType`, `FactionRank`, `FactionRecord`, `GameObject`, `Inventory`, `MagicEffect`, `MagicEffectId`, `MagicEffectWithParams`, `MagicSchoolData`, `MWScriptRecord`, `ObjectList`, `ObjectOwner`, `PathGrid`, `PathGridPoint`, `RegionRecord`, `RegionSoundRef`, `Skill`, `SkillRecord`, `SoundRecord`, `Spell`, `SpellRange`, `SpellType`, `TeleportOptions`, `TimeOfDayInterpolatorColor`, `TimeOfDayInterpolatorFloat`, `WeatherRecord`.

### `openmw.debug`

- [ ] `Debug.toggleRenderMode`
- [ ] `Debug.toggleGodMode`
- [ ] `Debug.isGodMode`
- [ ] `Debug.toggleAI`
- [ ] `Debug.isAIEnabled`
- [ ] `Debug.toggleCollision`
- [ ] `Debug.isCollisionEnabled`
- [ ] `Debug.toggleMWScript`
- [ ] `Debug.isMWScriptEnabled`
- [ ] `Debug.reloadLua`
- [ ] `Debug.setNavMeshRenderMode`
- [ ] `Debug.setShaderHotReloadEnabled`
- [ ] `Debug.triggerShaderReload`

Types: `NAV_MESH_RENDER_MODE`, `RENDER_MODE`.

### `openmw.input`

- [ ] `input.isIdle`
- [ ] `input.isActionPressed`
- [ ] `input.isKeyPressed`
- [ ] `input.isControllerButtonPressed`
- [ ] `input.isShiftPressed`
- [ ] `input.isCtrlPressed`
- [ ] `input.isAltPressed`
- [ ] `input.isSuperPressed`
- [ ] `input.isMouseButtonPressed`
- [ ] `input.getMouseMoveX`
- [ ] `input.getMouseMoveY`
- [ ] `input.getAxisValue`
- [ ] `input.getKeyName`
- [ ] `input.getControlSwitch`
- [ ] `input.setControlSwitch`
- [ ] `input.registerAction`
- [ ] `input.bindAction`
- [ ] `input.registerActionHandler`
- [ ] `input.getBooleanActionValue`
- [ ] `input.getNumberActionValue`
- [ ] `input.getRangeActionValue`
- [ ] `input.registerTrigger`
- [ ] `input.registerTriggerHandler`
- [ ] `input.activateTrigger`

Types: `ACTION`, `ACTION_TYPE`, `ActionInfo`, `ActionType`, `CONTROL_SWITCH`, `CONTROLLER_AXIS`, `CONTROLLER_BUTTON`, `ControlSwitch`, `KEY`, `KeyboardEvent`, `KeyCode`, `TouchEvent`, `TriggerInfo`.

### `openmw.interfaces`

- [ ] `interfaces.__index`

### `openmw.markup`

- [ ] `markup.decodeYaml`
- [ ] `markup.loadYaml`

### `openmw.menu`

- [ ] `menu.getState`
- [ ] `menu.newGame`
- [ ] `menu.loadGame`
- [ ] `menu.deleteGame`
- [ ] `menu.getCurrentSaveDir`
- [ ] `menu.saveGame`
- [ ] `menu.getSaves`
- [ ] `menu.getAllSaves`
- [ ] `menu.quit`

Types: `SaveInfo`, `STATE`.

### `openmw.nearby`

- [ ] `nearby.getObjectByFormId`
- [ ] `nearby.castRay`
- [ ] `nearby.castRenderingRay`
- [ ] `nearby.asyncCastRenderingRay`
- [ ] `nearby.findPath`
- [ ] `nearby.findRandomPointAroundCircle`
- [ ] `nearby.castNavigationRay`
- [ ] `nearby.findNearestNavMeshPosition`

Types: `AgentBounds`, `AreaCosts`, `CastRayOptions`, `CastRenderingRayOptions`, `COLLISION_SHAPE_TYPE`, `COLLISION_TYPE`, `FIND_PATH_STATUS`, `FindNearestNavMeshPositionOptions`, `FindPathOptions`, `NAVIGATOR_FLAGS`, `NavMeshOptions`, `RayCastingResult`.

### `openmw.postprocessing`

- [ ] `postprocessing.load`
- [ ] `postprocessing.getChain`
- [ ] `Shader.enable`
- [ ] `Shader.disable`
- [ ] `Shader.isEnabled`
- [ ] `Shader.setBool`
- [ ] `Shader.setInt`
- [ ] `Shader.setFloat`
- [ ] `Shader.setVector2`
- [ ] `Shader.setVector3`
- [ ] `Shader.setVector4`
- [ ] `Shader.setIntArray`
- [ ] `Shader.setFloatArray`
- [ ] `Shader.setVector2Array`
- [ ] `Shader.setVector3Array`
- [ ] `Shader.setVector4Array`

Types: `Shader`.

### `openmw.self`

- [ ] `Self.isActive`
- [ ] `Self.enableAI`

Types: `ActorControls`, `ATTACK_TYPE`.

### `openmw.storage`

- [ ] `storage.globalSection`
- [ ] `storage.playerSection`
- [ ] `storage.allGlobalSections`
- [ ] `storage.allPlayerSections`
- [ ] `StorageSection.get`
- [ ] `StorageSection.getCopy`
- [ ] `StorageSection.subscribe`
- [ ] `StorageSection.asTable`
- [ ] `StorageSection.reset`
- [ ] `StorageSection.removeOnExit`
- [ ] `StorageSection.setLifeTime`
- [ ] `StorageSection.set`

Types: `LifeTime`, `StorageSection`.

### `openmw.types`

- [ ] `Actor.getEncumbrance`
- [ ] `Actor.getCapacity`
- [ ] `Actor.getBarterGold`
- [ ] `Actor.setBarterGold`
- [ ] `Actor.isDead`
- [ ] `Actor.isDeathFinished`
- [ ] `Actor.getPathfindingAgentBounds`
- [ ] `Actor.isInActorsProcessingRange`
- [ ] `Actor.objectIsInstance`
- [ ] `Actor.inventory`
- [ ] `Actor.canMove`
- [ ] `Actor.getRunSpeed`
- [ ] `Actor.getWalkSpeed`
- [ ] `Actor.getCurrentSpeed`
- [ ] `Actor.isOnGround`
- [ ] `Actor.isSwimming`
- [ ] `Actor.getStance`
- [ ] `Actor.setStance`
- [ ] `Actor.setKnockedDown`
- [ ] `Actor.getKnockedDown`
- [ ] `Actor.setHitRecovery`
- [ ] `Actor.getHitRecovery`
- [ ] `Actor.hasEquipped`
- [ ] `Actor.getEquipment`
- [ ] `Actor.setEquipment`
- [ ] `Actor.getSelectedSpell`
- [ ] `Actor.setSelectedSpell`
- [ ] `Actor.clearSelectedCastable`
- [ ] `Actor.getSelectedEnchantedItem`
- [ ] `Actor.setSelectedEnchantedItem`
- [ ] `Actor.activeEffects`
- [ ] `ActorActiveEffects.getEffect`
- [ ] `ActorActiveEffects.remove`
- [ ] `ActorActiveEffects.set`
- [ ] `ActorActiveEffects.modify`
- [ ] `Actor.activeSpells`
- [ ] `ActorActiveSpells.isSpellActive`
- [ ] `ActorActiveSpells.remove`
- [ ] `ActorActiveSpells.add`
- [ ] `Actor.spells`
- [ ] `ActorSpells.add`
- [ ] `ActorSpells.remove`
- [ ] `ActorSpells.clear`
- [ ] `ActorSpells.canUsePower`
- [ ] `DynamicStats.health`
- [ ] `DynamicStats.magicka`
- [ ] `DynamicStats.fatigue`
- [ ] `AIStats.alarm`
- [ ] `AIStats.fight`
- [ ] `AIStats.flee`
- [ ] `AIStats.hello`
- [ ] `AttributeStats.strength`
- [ ] `AttributeStats.intelligence`
- [ ] `AttributeStats.willpower`
- [ ] `AttributeStats.agility`
- [ ] `AttributeStats.speed`
- [ ] `AttributeStats.endurance`
- [ ] `AttributeStats.personality`
- [ ] `AttributeStats.luck`
- [ ] `SkillStats.block`
- [ ] `SkillStats.armorer`
- [ ] `SkillStats.mediumarmor`
- [ ] `SkillStats.heavyarmor`
- [ ] `SkillStats.bluntweapon`
- [ ] `SkillStats.longblade`
- [ ] `SkillStats.axe`
- [ ] `SkillStats.spear`
- [ ] `SkillStats.athletics`
- [ ] `SkillStats.enchant`
- [ ] `SkillStats.destruction`
- [ ] `SkillStats.alteration`
- [ ] `SkillStats.illusion`
- [ ] `SkillStats.conjuration`
- [ ] `SkillStats.mysticism`
- [ ] `SkillStats.restoration`
- [ ] `SkillStats.alchemy`
- [ ] `SkillStats.unarmored`
- [ ] `SkillStats.security`
- [ ] `SkillStats.sneak`
- [ ] `SkillStats.acrobatics`
- [ ] `SkillStats.lightarmor`
- [ ] `SkillStats.shortblade`
- [ ] `SkillStats.marksman`
- [ ] `SkillStats.mercantile`
- [ ] `SkillStats.speechcraft`
- [ ] `SkillStats.handtohand`
- [ ] `ActorStats.level`
- [ ] `Item.objectIsInstance`
- [ ] `Item.getEnchantmentCharge`
- [ ] `Item.isRestocking`
- [ ] `Item.setEnchantmentCharge`
- [ ] `Item.isCarriable`
- [ ] `Item.itemData`
- [ ] `Creature.createRecordDraft`
- [ ] `Creature.objectIsInstance`
- [ ] `Creature.record`
- [ ] `NPC.createRecordDraft`
- [ ] `NPC.objectIsInstance`
- [ ] `NPC.getFactions`
- [ ] `NPC.getFactionRank`
- [ ] `NPC.setFactionRank`
- [ ] `NPC.modifyFactionRank`
- [ ] `NPC.joinFaction`
- [ ] `NPC.leaveFaction`
- [ ] `NPC.getFactionReputation`
- [ ] `NPC.setFactionReputation`
- [ ] `NPC.modifyFactionReputation`
- [ ] `NPC.expel`
- [ ] `NPC.clearExpelled`
- [ ] `NPC.isExpelled`
- [ ] `NPC.getDisposition`
- [ ] `NPC.getBaseDisposition`
- [ ] `NPC.setBaseDisposition`
- [ ] `NPC.modifyBaseDisposition`
- [ ] `Classes.record`
- [ ] `NPC.isWerewolf`
- [ ] `NPC.setWerewolf`
- [ ] `NPC.record`
- [ ] `Races.record`
- [ ] `PLAYER.objectIsInstance`
- [ ] `PLAYER.getCrimeLevel`
- [ ] `PLAYER.setCrimeLevel`
- [ ] `PLAYER.isCharGenFinished`
- [ ] `PLAYER.isTeleportingEnabled`
- [ ] `PLAYER.setTeleportingEnabled`
- [ ] `PLAYER.quests`
- [ ] `PLAYER.addTopic`
- [ ] `PLAYER.journal`
- [ ] `PLAYERQuest.addJournalEntry`
- [ ] `PLAYER.getControlSwitch`
- [ ] `PLAYER.setControlSwitch`
- [ ] `PLAYER.getBirthSign`
- [ ] `PLAYER.setBirthSign`
- [ ] `BirthSigns.record`
- [ ] `PLAYER.sendMenuEvent`
- [ ] `Armor.objectIsInstance`
- [ ] `Armor.record`
- [ ] `Armor.createRecordDraft`
- [ ] `BodyPart.objectIsInstance`
- [ ] `Book.objectIsInstance`
- [ ] `Book.record`
- [ ] `Book.createRecordDraft`
- [ ] `Clothing.objectIsInstance`
- [ ] `Clothing.record`
- [ ] `Clothing.createRecordDraft`
- [ ] `Ingredient.createRecordDraft`
- [ ] `Ingredient.objectIsInstance`
- [ ] `Ingredient.record`
- [ ] `LOCKABLE.objectIsInstance`
- [ ] `LOCKABLE.getKeyRecord`
- [ ] `LOCKABLE.setKeyRecord`
- [ ] `LOCKABLE.getTrapSpell`
- [ ] `LOCKABLE.setTrapSpell`
- [ ] `LOCKABLE.getLockLevel`
- [ ] `LOCKABLE.isLocked`
- [ ] `LOCKABLE.lock`
- [ ] `LOCKABLE.unlock`
- [ ] `Light.objectIsInstance`
- [ ] `Light.createRecordDraft`
- [ ] `Light.record`
- [ ] `Miscellaneous.objectIsInstance`
- [ ] `Miscellaneous.record`
- [ ] `Miscellaneous.getSoul`
- [ ] `Miscellaneous.createRecordDraft`
- [ ] `Miscellaneous.setSoul`
- [ ] `Potion.objectIsInstance`
- [ ] `Potion.record`
- [ ] `Potion.createRecordDraft`
- [ ] `Weapon.objectIsInstance`
- [ ] `Weapon.record`
- [ ] `Weapon.createRecordDraft`
- [ ] `Apparatus.objectIsInstance`
- [ ] `Apparatus.record`
- [ ] `Lockpick.objectIsInstance`
- [ ] `Lockpick.record`
- [ ] `Probe.createRecordDraft`
- [ ] `Probe.objectIsInstance`
- [ ] `Probe.record`
- [ ] `Repair.objectIsInstance`
- [ ] `Repair.record`
- [ ] `Activator.objectIsInstance`
- [ ] `Activator.record`
- [ ] `Activator.createRecordDraft`
- [ ] `Container.content`
- [ ] `Container.createRecordDraft`
- [ ] `Container.inventory`
- [ ] `Container.objectIsInstance`
- [ ] `Container.getEncumbrance`
- [ ] `Container.getCapacity`
- [ ] `Container.record`
- [ ] `Door.createRecordDraft`
- [ ] `Door.objectIsInstance`
- [ ] `Door.isTeleport`
- [ ] `Door.destPosition`
- [ ] `Door.destRotation`
- [ ] `Door.destCell`
- [ ] `Door.record`
- [ ] `Door.getDoorState`
- [ ] `Door.isOpen`
- [ ] `Door.isClosed`
- [ ] `Door.activateDoor`
- [ ] `Static.createRecordDraft`
- [ ] `Static.objectIsInstance`
- [ ] `Static.record`
- [ ] `CreatureLevelledList.objectIsInstance`
- [ ] `CreatureLevelledList.record`
- [ ] `CreatureLevelledListRecord.getRandomId`
- [ ] `ESM4Terminal.objectIsInstance`
- [ ] `ESM4Terminal.record`
- [ ] `ESM4Door.objectIsInstance`
- [ ] `ESM4Door.isTeleport`
- [ ] `ESM4Door.destPosition`
- [ ] `ESM4Door.destRotation`
- [ ] `ESM4Door.destCell`
- [ ] `ESM4Door.record`

Types: `Activator`, `ActivatorRecord`, `Actor`, `ActorActiveEffects`, `ActorActiveSpells`, `ActorSpells`, `ActorStats`, `AIStat`, `AIStats`, `Apparatus`, `ApparatusRecord`, `ApparatusTYPE`, `Armor`, `ArmorRecord`, `ArmorTYPE`, `AttributeStat`, `AttributeStats`, `BirthSignRecord`, `BodyPart`, `BodyPartRecord`, `Book`, `BookRecord`, `BookSKILL`, `ClassRecord`, `Clothing`, `ClothingRecord`, `ClothingTYPE`, `Container`, `ContainerRecord`, `CONTROL_SWITCH`, `ControlSwitch`, `Creature`, `CreatureAttack`, `CreatureLevelledList`, `CreatureLevelledListRecord`, `CreatureRecord`, `CreatureTYPE`, `Door`, `DoorRecord`, `DoorSTATE`, `DynamicStat`, `DynamicStats`, `EQUIPMENT_SLOT`, `EquipmentTable`, `ESM4Door`, `ESM4DoorRecord`, `ESM4Terminal`, `ESM4TerminalRecord`, `GenderedNumber`, `Ingredient`, `IngredientRecord`, `Item`, `ItemData`, `LevelledListItem`, `LevelStat`, `Light`, `LightRecord`, `Lockpick`, `LockpickRecord`, `Miscellaneous`, `MiscellaneousRecord`, `NPC`, `NpcRecord`, `NpcStats`, `OFFENSE_TYPE_IDS`, `PLAYER`, `PlayerJournalTextEntry`, `PlayerJournalTopic`, `PlayerJournalTopicEntry`, `PLAYERQuest`, `Potion`, `PotionRecord`, `Probe`, `ProbeRecord`, `RaceRecord`, `Repair`, `RepairRecord`, `ReputationStat`, `SkillIncreasesForAttributeStats`, `SkillIncreasesForSpecializationStats`, `SkillStat`, `SkillStats`, `STANCE`, `Static`, `StaticRecord`, `TravelDestination`, `Weapon`, `WeaponRecord`, `WeaponTYPE`.

### `openmw.ui`

- [ ] `ui.showMessage`
- [ ] `ui.printToConsole`
- [ ] `ui.setConsoleMode`
- [ ] `ui.setConsoleSelectedObject`
- [ ] `ui.screenSize`
- [ ] `ui.content`
- [ ] `ui.create`
- [ ] `ui.registerSettingsPage`
- [ ] `ui.removeSettingsPage`
- [ ] `ui.updateAll`
- [ ] `Layers.indexOf`
- [ ] `Layers.insertAfter`
- [ ] `Layers.insertBefore`
- [ ] `Content.__index`
- [ ] `Content.insert`
- [ ] `Content.add`
- [ ] `Content.indexOf`
- [ ] `Element.update`
- [ ] `Element.destroy`
- [ ] `ui.texture`

Types: `ALIGNMENT`, `CONSOLE_COLOR`, `Content`, `Element`, `Layer`, `Layers`, `Layout`, `MouseEvent`, `SettingsPageOptions`, `Template`, `TextureResource`, `TextureResourceOptions`, `TYPE`.

### `openmw.util`

- [ ] `util.round`
- [ ] `util.remap`
- [ ] `util.clamp`
- [ ] `util.normalizeAngle`
- [ ] `util.makeReadOnly`
- [ ] `util.makeStrictReadOnly`
- [ ] `util.loadCode`
- [ ] `util.bitAnd`
- [ ] `util.bitOr`
- [ ] `util.bitXor`
- [ ] `util.bitNot`
- [ ] `util.vector2`
- [ ] `Vector2.__add`
- [ ] `Vector2.__sub`
- [ ] `Vector2.__mul`
- [ ] `Vector2.__div`
- [ ] `Vector2.length`
- [ ] `Vector2.length2`
- [ ] `Vector2.normalize`
- [ ] `Vector2.rotate`
- [ ] `Vector2.dot`
- [ ] `Vector2.emul`
- [ ] `Vector2.ediv`
- [ ] `util.vector3`
- [ ] `Vector3.__add`
- [ ] `Vector3.__sub`
- [ ] `Vector3.__mul`
- [ ] `Vector3.__div`
- [ ] `Vector3.__tostring`
- [ ] `Vector3.length`
- [ ] `Vector3.length2`
- [ ] `Vector3.normalize`
- [ ] `Vector3.dot`
- [ ] `Vector3.cross`
- [ ] `Vector3.emul`
- [ ] `Vector3.ediv`
- [ ] `util.vector4`
- [ ] `Vector4.__add`
- [ ] `Vector4.__sub`
- [ ] `Vector4.__mul`
- [ ] `Vector4.__div`
- [ ] `Vector4.__tostring`
- [ ] `Vector4.length`
- [ ] `Vector4.length2`
- [ ] `Vector4.normalize`
- [ ] `Vector4.dot`
- [ ] `Vector4.emul`
- [ ] `Vector4.ediv`
- [ ] `util.box` overloads
- [ ] `Color.asRgba`
- [ ] `Color.asRgb`
- [ ] `Color.asHex`
- [ ] `COLOR.rgba`
- [ ] `COLOR.commaString`
- [ ] `COLOR.rgb`
- [ ] `COLOR.hex`
- [ ] `Transform.__mul`
- [ ] `Transform.inverse`
- [ ] `Transform.apply`
- [ ] `Transform.getYaw`
- [ ] `Transform.getPitch`
- [ ] `Transform.getAnglesXZ`
- [ ] `Transform.getAnglesZYX`
- [ ] `TRANSFORM.move`
- [ ] `TRANSFORM.scale`
- [ ] `TRANSFORM.rotate`
- [ ] `TRANSFORM.rotateX`
- [ ] `TRANSFORM.rotateY`
- [ ] `TRANSFORM.rotateZ`

Types: `Box`, `COLOR`, `Transform`, `Vector2`, `Vector3`, `Vector4`.

### `openmw.vfs`

- [ ] `FileHandle.close`
- [ ] `FileHandle.lines`
- [ ] `FileHandle.seek`
- [ ] `FileHandle.read`
- [ ] `vfs.fileExists`
- [ ] `vfs.open`
- [ ] `vfs.lines`
- [ ] `vfs.pathsWithPrefix`
- [ ] `vfs.type`

Types: `FileHandle`.

### `openmw.world`

- [ ] `MWScriptFunctions.getLocalScript`
- [ ] `MWScriptFunctions.getGlobalVariables`
- [ ] `MWScriptFunctions.getGlobalScript`
- [ ] `world.getCellByName`
- [ ] `world.getCellById`
- [ ] `world.getExteriorCell`
- [ ] `world.getSimulationTime`
- [ ] `world.getSimulationTimeScale`
- [ ] `world.setSimulationTimeScale`
- [ ] `world.getGameTime`
- [ ] `world.getGameTimeScale`
- [ ] `world.setGameTimeScale`
- [ ] `world.isWorldPaused`
- [ ] `world.pause`
- [ ] `world.unpause`
- [ ] `world.getPausedTags`
- [ ] `world.getObjectByFormId`
- [ ] `world.createObject`
- [ ] `world.createRecord`
- [ ] `VFX.spawn`
- [ ] `VFX.remove`
- [ ] `world.advanceTime`

Types: `MWScript`, `MWScriptFunctions`, `MWScriptVariables`.

### Package/type completion checklist

This checklist covers non-callable package fields, constants, enum values, record collections, type properties, mutability, and index behavior. The source contains approximately 1,512 documented field declarations; listing each field separately here would make the project plan unusable. Agents implement and test those fields as part of the owning type, and check the type only after all Morrowind-relevant documented fields are implemented or explicitly classified.

- [ ] `openmw.ambient` package fields and constants
- [ ] `openmw.animation`: `BlendMask`, `BoneGroup`, `Priority`
- [ ] `openmw.async` package fields and constants
- [ ] `openmw.camera`: `MODE`
- [ ] `openmw.content` package fields and constants
- [ ] `openmw.core` types and fields listed above
- [ ] `openmw.debug`: `NAV_MESH_RENDER_MODE`, `RENDER_MODE`
- [ ] `openmw.input` types and fields listed above
- [ ] `openmw.interfaces` lookup and read-only behavior
- [ ] `openmw.markup` package fields and constants
- [ ] `openmw.menu`: `SaveInfo`, `STATE`
- [ ] `openmw.nearby` types and fields listed above
- [ ] `openmw.postprocessing`: `Shader`
- [ ] `openmw.self`: `ActorControls`, `ATTACK_TYPE`, contextual `self`
- [ ] `openmw.storage`: `LifeTime`, `StorageSection`
- [ ] `openmw.types` types and fields listed above
- [ ] `openmw.ui` types and fields listed above
- [ ] `openmw.util`: `Box`, `COLOR`, `Transform`, `Vector2`, `Vector3`, `Vector4`
- [ ] `openmw.vfs`: `FileHandle`
- [ ] `openmw.world`: `MWScript`, `MWScriptFunctions`, `MWScriptVariables`

### Built-in interface tracking

These are not all part of the native `openmw.*` package surface, but third-party mods commonly obtain them through `openmw.interfaces`. Implement only the portions justified by the compatibility corpus, with absence preserved for optional third-party interfaces.

- [ ] `Activation`
- [ ] `AI`
- [ ] `AnimationController`
- [ ] `Camera`
- [ ] `Combat`
- [ ] `Constants`
- [ ] `Controls`
- [ ] `MWUI`
- [ ] `Settings`
- [ ] `SkillProgression`
- [ ] `StatsWindow`
- [ ] `Templates`
- [ ] `TooltipBuilders`
- [ ] NCG helper methods `addLineToSection`, `getLine`, and `modifyLine`

Optional external interfaces observed in NCG and expected to remain absent unless their providing mods are loaded:

- `MarksmansEye`
- `SkillFramework`

## Open questions requiring harness evidence

1. Can native `GameFile` loading accept an `.omwaddon` filename directly once it is present in the active-file list, or is a cached `.esp` alias required?
2. Which loader paths assume a four-character extension or explicit master/plugin suffix?
3. What stable identity is sufficient for dynamic and inventory references across a full save/restart/load cycle?
4. Is reference Lua data large and early enough for all compatibility state, or should a dedicated save record be introduced?
5. At what lifecycle point can menu scripts safely initialize without delaying normal MWSE startup?
6. Which NCG UI elements can map directly to `tes3ui`, and which require a small retained/declarative compatibility layer?
7. Which OpenMW API revision should be the first declared baseline? NCG requires at least revision 70.
8. Can LuaJIT be packaged inside the support DLL without dependency or allocator conflicts in the 32-bit process?

Resolve these questions with recorded harness probes. Do not settle them by assumption alone.
