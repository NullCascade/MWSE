# Automated Morrowind compatibility harness

Run the Milestone 1 gate from the repository root:

```powershell
.\tools\openmw-compat\Invoke-MorrowindHarness.ps1
```

The default test installation is `C:\Games\Morrowind`, the default build is x86 Debug, and the deterministic starting fixture is `Saves\TestMWSE0000.ess`. All are configurable:

```powershell
.\tools\openmw-compat\Invoke-MorrowindHarness.ps1 `
    -MorrowindDirectory D:\Games\Morrowind `
    -Configuration Release `
    -FixtureSave clean-fixture.ess
```

The launcher locates MSBuild with `vswhere`, builds `MWSE\MWSE.vcxproj`, temporarily stages the resulting MWSE files and the harness mod, writes an explicit opt-in config, launches Morrowind in its installation directory, runs the smoke suite, requests shutdown, and restores every staged file. It does not edit `Morrowind.ini` or the user's load order. It refuses to start if another Morrowind process is already running.

Each run is retained under `Data Files\MWSE\tmp\openmw-compat-harness\<run-id>`. Important files are `result.json`, `events.jsonl`, `ready.json`, `MWSE.log`, `build.log`, screenshots, and the smoke-suite `.ess`. The generated save is removed from the normal `Saves` directory after being copied into the run directory.

To validate protocol utilities without launching the game:

```powershell
.\tools\openmw-compat\tests\Test-HarnessProtocol.ps1
```

To extend the harness, add a narrowly named function to `probes` in `misc\package\Data Files\MWSE\mods\openmw_compat_harness\main.lua`, emit at least one `assertion`, then call it through `evalNamedProbe`. Add orchestration to the launcher or a future suite file. Do not add unrestricted evaluation: probe names and event names are explicit allowlists. See [protocol.md](protocol.md) for envelope and timeout rules.
