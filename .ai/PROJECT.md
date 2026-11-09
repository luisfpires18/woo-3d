# Project

- **Name:** Weapons of Order (WOO)
- **Genre:** single-player RPG
- **Repository:** https://github.com/luisfpires18/woo-3d (public — see [DECISIONS.md](DECISIONS.md) C-13)
- **Integration branch:** `master`. One feature branch per approved Issue. No `main` or `dev`.
- **CI:** none. No GitHub Actions. Claude builds and tests locally.

## Engine and machines

| Item | Value | Status |
|---|---|---|
| Engine | Unreal Engine 5.8.4 (`Build.version` 5.8.4, CL 58902052, `++UE5+Release-5.8`) | Verified installed on LP-VICTUS at `D:/Apps/Epic Games/UE_5.8`, 7 Nov 2026 |
| Current / initial development machine | LP-VICTUS — Windows 11 Pro, AMD Radeon RX 5500M 4 GB + integrated Radeon | Confirmed ([DECISIONS.md](DECISIONS.md) C-10); observed 7 Nov 2026 |
| Later development and performance-benchmark machine | UFO-LP (hostname `LP-UFOROG`) — Windows 11 Pro 10.0.26200, Ryzen 7 5800X, 32 GB RAM, RTX 2070 8 GB (driver 610.88) | Becoming the primary development machine (owner, 9 Nov 2026). UE 5.8.4 CL 58902052 at `F:/Apps/Epic Games/UE_5.8` (not registered in HKCU/HKLM; pass `-EngineDir`). Editor build and launch verified 9 Nov 2026 (VS 2026 18.11.0, MSVC 14.50.35739, SDK 10.0.22621.0). |
| Python | Engine-bundled `Engine/Binaries/ThirdParty/Python3/Win64/python.exe`; `PythonScriptPlugin` present under `Plugins/Experimental` | Files present; editor scripting **not run** |
| Automation tool | `Engine/Build/BatchFiles/RunUAT.bat` | Present; **not run** |

Engine paths are machine-specific. Re-check them on each machine (`HKCU\SOFTWARE\Epic Games\Unreal Engine\Builds` lists source/installed builds); never copy them blindly. No `.uproject` exists yet, so no build has been attempted.

## Performance objective

Reference target: **1080p, 60 FPS in a packaged Windows build** on UFO-LP (RTX 2070 8 GB). This is an objective subject to benchmarking, not a verified result. LP-VICTUS measurements are reported as LP-VICTUS results and never as evidence for the UFO-LP target.

## Prototype scope (from Issue #1)

- 500 × 500 m freely explorable **Sylvara jungle**.
- **Venari Village** with 8–12 buildings, dirt trails, a Sylvaran outpost.
- One predefined **Warrior** with a two-handed sword.
- Tab targeting, auto-attack, an immediate cooldown-based **Heavy Attack**, **Rage** resource.
- Free animated wildlife.

**Deferred** (not initial requirements): custom models, full RPG systems (inventory, quests, progression, etc.). See [DECISIONS.md](DECISIONS.md).

## Assets and licensing

- Only assets that are free **and** legally usable in a commercial game. Record source, licence and acquisition date for every third-party asset (a `THIRD_PARTY_ASSETS.md` will be added with the first asset import, not before).
- Account-gated Fab acquisitions: the **owner** acquires them. Claude may automate permitted direct downloads only.
- The repository is public. Committing a third-party asset publishes it; check that its licence allows redistribution in source form before committing. If not, keep it out of Git and record how to obtain it.
- Required Unreal binary assets (`.uasset`, `.umap`, source art) are tracked with **Git LFS**. See [`.gitattributes`](../.gitattributes).
- Evidence media is **not** committed to LFS by default; see [STORAGE.md](STORAGE.md).
- Bulk asset imports need their own approved Issue.

## Documentation index

| File | Purpose |
|---|---|
| [`../CLAUDE.md`](../CLAUDE.md) | Claude Code entry point |
| [`../README.md`](../README.md) | Project summary and local startup |
| [WORKFLOW.md](WORKFLOW.md) | Lifecycle, authorization, review, handoff |
| [SESSION_START.md](SESSION_START.md) | New-session procedure (ChatGPT and Claude) |
| [STATE.md](STATE.md) | Current pointers and blockers only |
| [DECISIONS.md](DECISIONS.md) | Confirmed / proposed / deferred decisions |
| [MEDIA.md](MEDIA.md) | Unreal screenshot/video evidence contract |
| [STORAGE.md](STORAGE.md) | Evidence storage, manifest, retention |
| [`../docs/BUILD_RUNBOOK.md`](../docs/BUILD_RUNBOOK.md) | Editor build / launch-check scripts and fresh-checkout troubleshooting |

A game design document is not in this repository yet; do not reconstruct one from chat history.

## Related projects

`luisfpires18/woo-rts` (Unity RTS) shares the workflow structure only. Its milestones, tests, tools, paths and evidence do not apply here.
