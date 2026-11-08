# Current state

Pointers only. Detailed history lives in Issues and PRs; replace lines, do not append a chronology.

- **Roadmap:** https://github.com/luisfpires18/woo-3d/issues/2 (M0–M6; issue-based trackers, not native GitHub Milestones).
- **Current milestone:** M0 Technical Foundation — in progress; tracker https://github.com/luisfpires18/woo-3d/issues/3.
- **Active implementation Issue:** #41 WOO-008 https://github.com/luisfpires18/woo-3d/issues/41 (draft PR awaiting review).
- **Active implementation PR:** https://github.com/luisfpires18/woo-3d/pull/47 (draft).
- **Last completed tasks:** WOO-002 https://github.com/luisfpires18/woo-3d/issues/35 (PR https://github.com/luisfpires18/woo-3d/pull/45, squash-merged as `4ae9cbb`), WOO-003 https://github.com/luisfpires18/woo-3d/issues/36 (PR https://github.com/luisfpires18/woo-3d/pull/44), and WOO-004 https://github.com/luisfpires18/woo-3d/issues/37 (closed; audit report https://github.com/luisfpires18/woo-3d/issues/37#issuecomment-6720442373). WOO-007 https://github.com/luisfpires18/woo-3d/issues/40 (PR https://github.com/luisfpires18/woo-3d/pull/46) — review ACCEPTED, owner directed merge 8 Nov 2026.
- **Research:** #38 WOO-005 owner-accepted and closed 8 Nov 2026. Asset selection approval for #39 and P-1 (visibility) are still pending.
- **Next proposed task:** none eligible without an owner decision. #39 WOO-006 needs #38 selection approval, P-1 and Fab steps.
- **Development machine:** LP-VICTUS initially. UFO-LP is the later performance-benchmark machine; Unreal, automation and media tooling must be revalidated there when development moves.

## Blockers / owner decisions open

- Repository visibility: public vs private — [DECISIONS.md](DECISIONS.md) P-1. Do not commit source assets lacking public redistribution rights.
- Editor build (clean + incremental) and launch check scripted and verified on LP-VICTUS (#35, `docs/BUILD_RUNBOOK.md`). EngineWire MCP, the editor viewport capture route and one engine automation test are verified in MCPSandbox only (#40, `docs/MCP_RUNBOOK.md`). `-game` screenshot and FFmpeg `ddagrab` video are verified in MCPSandbox only (#41). Other capture routes and packaging are still **unverified** — [MEDIA.md](MEDIA.md).
