# Current state

Pointers only. Detailed history lives in Issues and PRs; replace lines, do not append a chronology.

- **Roadmap:** https://github.com/luisfpires18/woo-3d/issues/2 (M0–M6; issue-based trackers, not native GitHub Milestones).
- **Current milestone:** M0 Technical Foundation — in progress; tracker https://github.com/luisfpires18/woo-3d/issues/3.
- **Active implementation Issue:** none.
- **Active implementation PR:** none. Draft https://github.com/luisfpires18/woo-3d/pull/43 (#38 STATE pointer only) is still open.
- **Last completed tasks:** WOO-002 https://github.com/luisfpires18/woo-3d/issues/35 (PR https://github.com/luisfpires18/woo-3d/pull/45, squash-merged as `4ae9cbb`) and WOO-003 https://github.com/luisfpires18/woo-3d/issues/36 (PR https://github.com/luisfpires18/woo-3d/pull/44) — reviews ACCEPTED, owner directed merge 8 Nov 2026.
- **Research:** #38 WOO-005 owner-accepted and closed 8 Nov 2026. Asset selection approval for #39 and P-1 (visibility) are still pending.
- **Next proposed task:** eligible, not started (needs an explicit owner start): #37 WOO-004. #40 WOO-007 also needs the owner to confirm the MCP selection and approve Claude Code MCP registration; #39 needs #38 selection approval and P-1; #41 needs #40.
- **Development machine:** LP-VICTUS initially. UFO-LP is the later performance-benchmark machine; Unreal, automation and media tooling must be revalidated there when development moves.

## Blockers / owner decisions open

- Repository visibility: public vs private — [DECISIONS.md](DECISIONS.md) P-1. Do not commit source assets lacking public redistribution rights.
- Evidence storage route untested — [DECISIONS.md](DECISIONS.md) P-4.
- Editor build (clean + incremental) and launch check scripted and verified on LP-VICTUS (#35, `docs/BUILD_RUNBOOK.md`). Automation, capture routes and packaging are still **unverified** — [MEDIA.md](MEDIA.md).
