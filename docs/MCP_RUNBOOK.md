# Unreal MCP runbook (EngineWire)

Added by WOO-007 (#40). Observed on LP-VICTUS, UE 5.8.4, 8 Nov 2026.

| Item | Value |
|---|---|
| Server | EngineWire 0.6.0 (`unreal-mcp`), https://github.com/ChiR24/Unreal_mcp, beta |
| Location | Project plugin in `D:/Workspace/unrealengine/MCPSandbox/Plugins/EngineWire`. Not enabled in `WeaponsOfOrder.uproject`. |
| Transport | Native Streamable HTTP, `http://127.0.0.1:3000/mcp`, loopback only. WebSocket bridge on `127.0.0.1:8090,8091`. |
| Tool | One gateway tool, `unreal`, with operations `search`, `describe`, `execute`, `configure` |
| Claude Code name | `unreal-engine`, local scope, keyed to `D:/Workspace/unrealengine/woo` in `~/.claude.json` |

## Start and connect

1. Open `MCPSandbox.uproject` in UE 5.8. The port listens about 25 s after launch.
2. Start a **new** Claude Code session in this repository. MCP servers are read at session start; a session already running does not gain the tool.
3. Confirm the connection. `Connected` is expected:

```bash
claude mcp get unreal-engine
```

## Register again

The token changes if `Saved/MCP/capability-token` is deleted. Registering again needs it copied into the header. Never print or commit it.

```bash
claude mcp remove unreal-engine -s local
```

```bash
claude mcp add --transport http -s local unreal-engine http://127.0.0.1:3000/mcp --header "X-MCP-Capability-Token: $(tr -d '\r\n' < D:/Workspace/unrealengine/MCPSandbox/Saved/MCP/capability-token)"
```

## Recover from a disconnect

| Symptom | Fix |
|---|---|
| `ECONNREFUSED` | The editor is closed or still loading. Open it and retry. |
| 401 | The token is stale. Register again. |
| Tool missing but `claude mcp get` is `Connected` | The session started before registration or while the editor was closed. Start a new session. |

## Call pattern

Search, then describe, then execute, as the server instructions say. Skipping describe is how a call goes wrong: `inspect.get_editor_state` without its required `editorState` returned `MISSING_REQUIRED_PARAMETER`.

Capabilities verified through Claude Code on 8 Nov 2026: `manage_level.get_summary`, `control_actor.list`, `inspect.get_editor_state`, `build_environment.create_lighting_setup`, `control_actor.spawn`, `control_actor.set_transform`, `manage_level.save`, `control_editor.screenshot`, `system_control.run_build` (`kind: tests`), `system_control.execute_python`. The other capabilities in the 453-entry catalog are untested.

## Reruns without duplicates

`control_actor.spawn` is declared non-idempotent. Guard every spawn:

- `options.require: ["not <Label> exists"]` refuses a second spawn with `PRECONDITION_FAILED`, and nothing runs. Use this one. It reads the live level.
- `options.idempotencyKey` with the same params returns the recorded receipt with `"replayed": true`. The receipt repeats the old message `Actor spawned`, so check `replayed` and the actor list. Whether receipts survive an editor restart is untested.
- `control_actor.set_transform` is safe to repeat. A repeat with the same values reports `NOTHING_CHANGED` in its undo block.

## Independent check of saved state

Run a separate process against the saved map. `-DisablePlugins=EngineWire` keeps it off the editor's ports. A script in `-script` can call `unreal.EditorLoadingAndSavingUtils.load_map` and print the actors.

```bash
"D:/Apps/Epic Games/UE_5.8/Engine/Binaries/Win64/UnrealEditor-Cmd.exe" "D:/Workspace/unrealengine/MCPSandbox/MCPSandbox.uproject" -run=pythonscript -script="D:/tmp/verify.py" -DisablePlugins=EngineWire -unattended -nullrhi -nosplash -nop4 -stdout -abslog=D:/tmp/verify.log
```

## What cannot be automated, and fallbacks

| Limit | Safe fallback |
|---|---|
| A running Claude Code session cannot gain the tool. | Start a new session. Publish a blocker if that is not possible. Python or CLI does not count as MCP success. |
| The MCP needs an open editor with the plugin loaded. `NOT_CONNECTED` or a refused connection means no editor operation ran. | Open `MCPSandbox.uproject`. For read-only checks of saved content, use the commandlet above. |
| `headersHelper` needs the workspace trust dialog accepted interactively. | The header stored in `~/.claude.json`, outside the repository. |
| `execute_python` and destructive capabilities need a single-use consent nonce from describe. | Run describe again for each call. A nonce cannot be reused. |
| Viewport captures are not realtime unless enabled, so ticking effects are missing. | `control_editor.configure_viewport` with `realtime`, or capture a running game. Not tested. |
| Blueprint graph edits, PIE, packaging, and the plugin in `WeaponsOfOrder.uproject` are unverified through this MCP. | Verify in the Issue that first needs them. Enabling the plugin in the production project needs owner direction. |

## Security

- Keep **Listen Host** at `127.0.0.1` and **Allow Non Loopback** off.
- The token is stored in plain text in `~/.claude.json`, which is outside the repository. `headersHelper` would avoid that, but Claude Code does not run it until the workspace trust dialog has been accepted interactively.
- The plugin logs a warning that the all-or-nothing capability token is deprecated in favour of scoped tokens. Scoped tokens are not configured.
