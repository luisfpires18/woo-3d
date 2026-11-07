# Weapons of Order

Single-player RPG built with **Unreal Engine 5.8.4**.

The first prototype is a 500 × 500 m Sylvara jungle with Venari Village, dirt trails and a Sylvaran outpost. The player controls one Warrior with a two-handed sword, using tab targeting, auto-attack, a cooldown Heavy Attack and Rage. Free animated wildlife populates the jungle. Full scope and status: [`.ai/PROJECT.md`](.ai/PROJECT.md).

**Status:** workflow setup only. There is no Unreal project (`.uproject`) yet.

## Requirements

- Windows 10/11
- Unreal Engine 5.8.4 (Epic Games Launcher)
- Visual Studio with the "Game development with C++" workload (once C++ code exists)
- Git and [Git LFS](https://git-lfs.com/)

## Getting started

```bash
git lfs install
git clone https://github.com/luisfpires18/woo-3d.git
```

Open the `.uproject` with UE 5.8.4 once one exists.

## Workflow

Development follows an issue-driven workflow: planning, implementation on a feature branch, independent review, and owner approval. Integration branch: `master`.

| Document | Purpose |
|---|---|
| [CLAUDE.md](CLAUDE.md) | Claude Code entry point |
| [.ai/WORKFLOW.md](.ai/WORKFLOW.md) | Lifecycle, authorization and review |
| [.ai/SESSION_START.md](.ai/SESSION_START.md) | New-session checklist |
| [.ai/STATE.md](.ai/STATE.md) | Current task pointers |
| [.ai/DECISIONS.md](.ai/DECISIONS.md) | Confirmed, proposed and deferred decisions |
| [.ai/MEDIA.md](.ai/MEDIA.md) | Screenshot and video evidence |
| [.ai/STORAGE.md](.ai/STORAGE.md) | Evidence storage and retention |
