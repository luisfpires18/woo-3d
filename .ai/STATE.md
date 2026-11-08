# Current state

Pointers only. Detailed history lives in Issues and PRs; replace lines, do not append a chronology.

- **Roadmap:** https://github.com/luisfpires18/woo-3d/issues/2 (M0–M6; issue-based trackers, not native GitHub Milestones).
- **Current milestone:** M0 Technical Foundation — in progress; tracker https://github.com/luisfpires18/woo-3d/issues/3.
- **Active implementation Issue:** https://github.com/luisfpires18/woo-3d/issues/40 (WOO-007), blocked on a new Claude Code session.
- **Active implementation PR:** https://github.com/luisfpires18/woo-3d/pull/46 (draft).
- **Last completed tasks:** WOO-002 https://github.com/luisfpires18/woo-3d/issues/35 (PR https://github.com/luisfpires18/woo-3d/pull/45, squash-merged as `4ae9cbb`), WOO-003 https://github.com/luisfpires18/woo-3d/issues/36 (PR https://github.com/luisfpires18/woo-3d/pull/44), and WOO-004 https://github.com/luisfpires18/woo-3d/issues/37 (closed; audit report https://github.com/luisfpires18/woo-3d/issues/37#issuecomment-6720442373).
- **Research:** #38 WOO-005 owner-accepted and closed 8 Nov 2026. Asset selection approval for #39 and P-1 (visibility) are still pending.
- **Next proposed task:** finish #40. #39 needs #38 selection approval and P-1; #41 needs #40.
- **Development machine:** LP-VICTUS initially. UFO-LP is the later performance-benchmark machine; Unreal, automation and media tooling must be revalidated there when development moves.

## Blockers / owner decisions open

- Repository visibility: public vs private — [DECISIONS.md](DECISIONS.md) P-1. Do not commit source assets lacking public redistribution rights.
- Evidence storage route untested — [DECISIONS.md](DECISIONS.md) P-4.
- Editor build (clean + incremental) and launch check scripted and verified on LP-VICTUS (#35, `docs/BUILD_RUNBOOK.md`). Automation, capture routes and packaging are still **unverified** — [MEDIA.md](MEDIA.md).
