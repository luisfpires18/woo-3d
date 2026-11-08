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

## Security

- Keep **Listen Host** at `127.0.0.1` and **Allow Non Loopback** off.
- The token is stored in plain text in `~/.claude.json`, which is outside the repository. `headersHelper` would avoid that, but Claude Code does not run it until the workspace trust dialog has been accepted interactively.
- The plugin logs a warning that the all-or-nothing capability token is deprecated in favour of scoped tokens. Scoped tokens are not configured.
