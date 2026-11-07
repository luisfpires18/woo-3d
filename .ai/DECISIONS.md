# Decisions

Three lists. Do not move an item to **Confirmed** without an owner decision and a link. Brainstorm options are not requirements.

## Confirmed

| ID | Decision | Source |
|---|---|---|
| C-1 | Engine is Unreal Engine 5.8.4. | [Issue #1](https://github.com/luisfpires18/woo-3d/issues/1) |
| C-2 | Workflow: ChatGPT plans → Claude Code implements → separate ChatGPT reviews → owner approves. Review statuses `ACCEPTED` / `NEED CHANGES` / `DECLINED`. | Issue #1; adapted from woo-rts `.ai/WORKFLOW.md` |
| C-3 | Integration branch `master`; one feature branch per approved Issue; no `main` or `dev`. | Issue #1 |
| C-4 | No GitHub Actions initially; Claude builds and tests locally. | Issue #1 |
| C-5 | Prototype scope: 500 × 500 m Sylvara jungle, Venari Village (8–12 buildings), dirt trails, Sylvaran outpost, one Warrior with two-handed sword, tab targeting, auto-attack, cooldown Heavy Attack, Rage, free animated wildlife. | Issue #1 |
| C-6 | Assets: free and commercially usable; owner acquires account-gated Fab assets; Claude may automate permitted direct downloads; Unreal binaries in Git LFS. | Issue #1 |
| C-7 | Performance objective: 1080p / 60 FPS packaged Windows build, subject to benchmarking. | Issue #1 |
| C-8 | LorEvolve is optional and never blocks work. | Issue #1 |
| C-9 | GitHub records (Issues, PRs, these files) are authoritative; assistant memory is not. | woo-rts DECISIONS (5 Nov 2026), restated in Issue #1 |

## Proposed — owner decision needed

| ID | Question | Options / notes |
|---|---|---|
| P-1 | Repository visibility. Repo is **public**; earlier planning preferred private unless Actions usage justified public. | Keep public (third-party assets must allow redistribution) or make private (release-asset evidence stays private). Claude does not change visibility. |
| P-2 | Primary machine. Issue #1 names UFO-LP (RTX 2070 8 GB); setup ran on LP-VICTUS (RX 5500M 4 GB). | Confirm which machine runs builds/benchmarks; record the other as secondary. Performance results must name hardware. |
| P-3 | Initial `master` bootstrap. Remote was empty; a PR needs a base commit. | Recorded in Issue #1 comment; owner directed setup start on 7 Nov 2026. |
| P-4 | Evidence storage route: draft GitHub release assets (as woo-rts). | Untested in this repo; see [STORAGE.md](STORAGE.md). |
| P-5 | Feature branch naming `woo-<id>-<slug>`. | Convention only; change freely. |

## Deferred

| ID | Item | Revisit when |
|---|---|---|
| D-1 | Custom models (characters, buildings). | After prototype with free assets. |
| D-2 | Full RPG systems (inventory, quests, progression, saving). | Roadmap planning. |
| D-3 | GitHub Actions / CI. | When local-only builds become a bottleneck. |
| D-4 | Roadmap Issue and milestones. | Separate owner-approved publication step. |
