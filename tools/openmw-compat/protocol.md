# Morrowind harness protocol version 1

The Milestone 1 harness uses UTF-8 JSON commands and append-only UTF-8 JSON Lines events. Every envelope contains `protocolVersion: 1`, a unique `runId`, and a UTC `timestamp` where it is emitted by the game. Requests also contain a unique `requestId`, `type: "command"`, a command name, optional arguments, and `timeoutMs`.

The launcher writes `command.json` to a uniquely named temporary file in the same directory and renames it into place. The game consumes and removes that file. There is at most one unconsumed command file, though multiple requests may be outstanding while `waitForEvent` is pending.

The game appends one complete JSON object plus a newline to `events.jsonl`. Readers must ignore an unterminated final line until it is completed. Sequence numbers are monotonically increasing within a run.

Message types are:

- `ready`: MWSE and the opt-in harness initialized. Also written atomically to `ready.json`.
- `heartbeat`: emitted every two seconds with current game state.
- `response`: completion of a request; `ok` selects either `result` or structured `error`.
- `assertion`: named probe assertion with expected, actual, and pass/fail values.
- `log`: structured harness diagnostic.
- `fatal`: a protocol-level failure that prevents normal request completion.
- `shutdown`: acknowledgement that graceful process shutdown was requested after output was flushed.

Relevant messages carry `gameState`, whose stable fields are `name`, `mainMenu`, `loading`, `inGame`, `paused`, `currentCell`, and `playerValid`. `paused` maps to native Morrowind menu mode; this is the closest stable engine state exposed by MWSE and includes ordinary menus.

Supported commands are `ping`, `getState`, `evalNamedProbe`, `loadGame`, `newGame`, `teleport`, `waitForEvent`, `screenshot`, and `shutdown`. `screenshot` queues MGE's capture into Morrowind's `screenshots` directory and responds only after the file appears; some D3D8/MGE configurations do not expose framebuffer capture, in which case it returns a bounded `screenshot_timeout` error. Normal tests cannot evaluate arbitrary Lua. Version 1 named probes are `mwseInitialization`, `playerAndCell`, `writeReferencePersistentValue`, `readReferencePersistentValue`, and `saveGame`.

Every launcher wait is bounded. A timeout writes `timeout-diagnostic.json` containing the last heartbeat, recent protocol messages, process state, and the tail of `MWSE.log`. An in-game `waitForEvent` timeout returns the same game-state and recent-log diagnostic in its error.

On shutdown the Lua mod flushes `response` and `shutdown` first, then attempts an in-process `WM_CLOSE`. MGE can own the top-level HWND, so the launcher also calls `CloseMainWindow` on the exact process it started. Failure to exit within 15 seconds fails the gate and cleanup force-terminates only that process.
