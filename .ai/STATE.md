# Current state

Pointers only. Detailed history lives in Issues and PRs; replace lines, do not append a chronology.

- **Roadmap:** https://github.com/luisfpires18/woo-3d/issues/2 (M0–M6; issue-based trackers, not native GitHub Milestones).
- **Current milestone:** M0 Technical Foundation — in progress; tracker https://github.com/luisfpires18/woo-3d/issues/3.
- **Active implementation Issue:** #36 WOO-003 (owner start 8 Nov 2026); #35 WOO-002 runs in parallel (PR #45).
- **Active implementation PR:** #36 → https://github.com/luisfpires18/woo-3d/pull/44 (draft, in review).
- **Last completed task:** WOO-001 https://github.com/luisfpires18/woo-3d/issues/34 — review ACCEPTED, owner directed merge; PR https://github.com/luisfpires18/woo-3d/pull/42 squash-merged to `master` as `58f84b97a7d4e9aa1112d91aa30e4d087a8592d0`.
- **Research:** #38 WOO-005 owner-accepted and closed 8 Nov 2026. Asset selection approval for #39 and P-1 (visibility) are still pending.
- **Next proposed task:** none started beyond #35/#36. #40 WOO-007 needs WOO-002 too; #37 needs #36; #39 needs #38 selection approval and P-1; #41 needs #40.
- **Development machine:** LP-VICTUS initially. UFO-LP is the later performance-benchmark machine; Unreal, automation and media tooling must be revalidated there when development moves.

## Blockers / owner decisions open

- Repository visibility: public vs private — [DECISIONS.md](DECISIONS.md) P-1. Do not commit source assets lacking public redistribution rights.
- Evidence storage route untested — [DECISIONS.md](DECISIONS.md) P-4.
- Editor build and launch verified on LP-VICTUS (#34, PR #42). Automation, capture routes and packaging are still **unverified** — [MEDIA.md](MEDIA.md).
