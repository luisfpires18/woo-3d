# Weapons of Order

Single-player RPG built with **Unreal Engine 5.8.4**.

The first prototype is a 500 × 500 m Sylvara jungle with Venari Village, dirt trails and a Sylvaran outpost. The player controls one Warrior with a two-handed sword, using tab targeting, auto-attack, a cooldown Heavy Attack and Rage. Free animated wildlife populates the jungle. Full scope and status: [`.ai/PROJECT.md`](.ai/PROJECT.md).

**Status:** M0 Technical Foundation. A minimal C++ project (`WeaponsOfOrder.uproject`, `Source/WeaponsOfOrder`) exists; no gameplay or assets yet.

## Requirements

- Windows 10/11
- Unreal Engine 5.8.4 (Epic Games Launcher)
- Visual Studio 2026 (18.0+) or 2022 (17.8+). Open the committed [`.vsconfig`](.vsconfig) in the Visual Studio Installer (*More > Import configuration*) to add the required workloads and components.
- MSVC: UE 5.8.4 prefers 14.50.35717-14.50.x (VS 2026) or 14.44.35207-14.44.x (VS 2022), minimum 14.38.33130; 14.50.0-14.50.35722 and 14.39-14.43 are banned. Windows SDK 10.0.22621.0. Source: `Engine/Config/Windows/Windows_SDK.json`.
- Git and [Git LFS](https://git-lfs.com/)

## Getting started

```bash
git lfs install
git clone https://github.com/luisfpires18/woo-3d.git
```

After cloning, check that LFS content is real data, not pointer files:

```bash
git lfs pull
git lfs ls-files
git lfs track
```

Build and launch steps: pending (WOO-002, #35).

## Source-control rules

- `.uasset`, `.umap` and source art (`.fbx`, `.obj`, `.blend`, `.psd`, `.tga`, `.exr`, `.hdr`, `.wav`, `.ogg`) go to Git LFS anywhere in the tree.
- `.png`, `.jpg` and `.mp4` go to LFS only under `Content/` or `SourceArt/`. Evidence screenshots and video are not committed; they go to release assets ([`.ai/STORAGE.md`](.ai/STORAGE.md)).
- `.vsconfig` is committed. Unreal regenerates it from the engine's `Windows_SDK.json`, so it only changes with an engine upgrade. With VS 2022 it lists 14.44 components instead of 14.50; keep the VS 2026 version.
- Ignored: `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, generated solutions (`*.sln`, `*.slnx`), IDE folders, build outputs and local credential files.

Check a path before committing it:

```bash
git check-attr -a -- Content/Maps/L_Test.umap
git check-ignore -v -- Saved/Logs/WeaponsOfOrder.log
```

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
