# Current state

Pointers only. Detailed history lives in Issues and PRs; replace lines, do not append a chronology.

- **Roadmap:** https://github.com/luisfpires18/woo-3d/issues/2 (M0–M6; issue-based trackers, not native GitHub Milestones).
- **Current milestone:** M0 Technical Foundation — in progress; tracker https://github.com/luisfpires18/woo-3d/issues/3.
- **Active implementation Issue:** #39 WOO-006 https://github.com/luisfpires18/woo-3d/issues/39 — sample merged on owner direction; left open for the owner decisions below.
- **Active implementation PR:** none. Last: https://github.com/luisfpires18/woo-3d/pull/48 (squash-merged on owner direction, 8 Nov 2026; about 67.8 MB of LFS content).
- **Last completed tasks:** WOO-002 https://github.com/luisfpires18/woo-3d/issues/35 (PR https://github.com/luisfpires18/woo-3d/pull/45, squash-merged as `4ae9cbb`), WOO-003 https://github.com/luisfpires18/woo-3d/issues/36 (PR https://github.com/luisfpires18/woo-3d/pull/44), and WOO-004 https://github.com/luisfpires18/woo-3d/issues/37 (closed; audit report https://github.com/luisfpires18/woo-3d/issues/37#issuecomment-6720442373). WOO-007 https://github.com/luisfpires18/woo-3d/issues/40 (PR https://github.com/luisfpires18/woo-3d/pull/46) — review ACCEPTED, owner directed merge 8 Nov 2026. WOO-008 https://github.com/luisfpires18/woo-3d/issues/41 (PR https://github.com/luisfpires18/woo-3d/pull/47, squash-merged as `caee4896`) — review ACCEPTED, owner directed merge 8 Nov 2026.
- **Research:** #38 WOO-005 owner-accepted and closed 8 Nov 2026. Provisional asset selection approved for the #39 spike under gates, 8 Nov 2026 (https://github.com/luisfpires18/woo-3d/issues/39#issuecomment-6726799348).
- **Next proposed task:** none; #39 WOO-006 is the last open M0 task.
- **Development machine:** moving to UFO-LP (`LP-UFOROG`, checkout `F:/Workspace/Projects/Unity/woo-3d`). Setup audit 9 Nov 2026: Git LFS content complete; native Unreal MCP verified in a sandbox only; editor build (clean + incremental), launch check, `-game` HighResShot and FFmpeg `ddagrab` video of `L_ImportSample` pass there (capture must be launched from an interactive terminal, not a background shell). Evidence upload, MCP in WeaponsOfOrder and packaging not run. LP-VICTUS results stay LP-VICTUS results.

## Blockers / owner decisions open

- #39 open items for the owner: hostile types are Wolf and Husky by owner decision (Velociraptor clips failed in this importer); the free Warrior has no head mesh; the Ranger outfit is a blockout placeholder. Repository stays public ([DECISIONS.md](DECISIONS.md) C-13): do not commit source assets lacking public redistribution rights.
- Editor build (clean + incremental) and launch check scripted and verified on LP-VICTUS (#35, `docs/BUILD_RUNBOOK.md`). EngineWire MCP, the editor viewport capture route and one engine automation test are verified in MCPSandbox only (#40, `docs/MCP_RUNBOOK.md`). `-game` screenshot and FFmpeg `ddagrab` video are verified in MCPSandbox only (#41). Other capture routes and packaging are still **unverified** — [MEDIA.md](MEDIA.md).
