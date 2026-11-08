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
| C-10 | Machines: **LP-VICTUS** (Radeon RX 5500M 4 GB) is the initial development machine, including M0 and early prototype work. **UFO-LP** (RTX 2070 8 GB) is the later development and 1080p/60 FPS packaged-build benchmark machine. Results are attributed to the machine actually used; Unreal, automation and media tooling are revalidated on UFO-LP before relying on them there. | [Owner update on PR #21](https://github.com/luisfpires18/woo-3d/pull/21#issuecomment-6714273439) |
| C-11 | Roadmap and plan structure: Roadmap Issue #2, milestone tracker Issues #3–#9, phase Issues #10–#20 and #22–#33 (#21 is the setup PR), M0 task Issues #34–#41. Issue-based Markdown trackers; no native GitHub Milestones or sub-issues. | [Roadmap #2](https://github.com/luisfpires18/woo-3d/issues/2); [review on PR #21](https://github.com/luisfpires18/woo-3d/pull/21#issuecomment-6714203616) |
| C-12 | Evidence storage: draft GitHub release assets per [STORAGE.md](STORAGE.md) (was P-4). Owner approved 8 Nov 2026. Verified in #41: upload, authenticated download with matching hashes, and 404 for anonymous access. ChatGPT reviewer retrieval is not verified. | [Owner decision on #41](https://github.com/luisfpires18/woo-3d/issues/41); [PR #47](https://github.com/luisfpires18/woo-3d/pull/47) |

## Completed setup actions

| ID | Action | Record |
|---|---|---|
| H-1 | Initial `master` bootstrap: commit `0a050c7` with `.gitignore` and `.gitattributes` only, pushed 7 Nov 2026 under the owner's start instruction for Issue #1. | [Issue #1 report](https://github.com/luisfpires18/woo-3d/issues/1#issuecomment-6714130674) |
| H-2 | Roadmap publication: Roadmap #2 and its tracker, phase and M0 task Issues published by the owner-approved roadmap step. | [Roadmap #2](https://github.com/luisfpires18/woo-3d/issues/2) |

## Proposed — owner decision needed

| ID | Question | Options / notes |
|---|---|---|
| P-1 | Repository visibility. Repo is **public**; earlier planning preferred private unless Actions usage justified public. | Keep public (third-party assets must allow redistribution) or make private (release-asset evidence stays private). Claude does not change visibility. |
| P-5 | Feature branch naming `woo-<id>-<slug>`. | Convention only; change freely. |

## Deferred

| ID | Item | Revisit when |
|---|---|---|
| D-1 | Custom models (characters, buildings). | After prototype with free assets. |
| D-2 | Full RPG systems (inventory, quests, progression, saving). | Roadmap planning. |
| D-3 | GitHub Actions / CI. | When local-only builds become a bottleneck. |
