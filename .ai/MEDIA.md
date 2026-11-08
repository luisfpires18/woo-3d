# Media evidence contract (Unreal)

## Capability status

The routes marked Verified below are the ones tested. woo-rts capture results (Unity Game view, FFmpeg `ddagrab`, helper scripts) are **not** evidence that any route works for Unreal; revalidate each one here.

| Route | Status |
|---|---|
| Editor viewport PNG through EngineWire `control_editor.screenshot` (`mode: editor_viewport`, `returnBase64: false`) | **Verified** 8 Nov 2026, LP-VICTUS, UE 5.8.4 editor, Radeon RX 5500M, tested commit `21e834f`, MCPSandbox only (#40). 1523x808 PNG written to `Saved/Screenshots`. Editor viewport only, not realtime. Not evidence for PIE, `-game` or packaged captures. |
| In-engine screenshot: `HighResShot 1` through `-ExecCmds` in `-game` | **Verified** 8 Nov 2026, LP-VICTUS, UE 5.8.4, D3D12 SM6, Radeon RX 5500M, tested commit `e87bf02`, MCPSandbox only (#41). 1280x720 PNG with engine pixels only. Run by [`Tools/Capture-GameEvidence.ps1`](../Tools/Capture-GameEvidence.ps1). `Shot` and `FScreenshotRequest` are untested. Also verified in WeaponsOfOrder on 8 Nov 2026 (#39, tested commit `3e9bb2d`). |
| Automation screenshot comparison (Functional Testing / Screenshot Comparison) | Unverified |
| Video: Movie Render Queue (editor, deterministic, offline) | Unverified |
| Video: FFmpeg 8.1.1 `ddagrab` of a `-game` window | **Verified** 8 Nov 2026, same machine, commit and project (#41). `Tools/Capture-GameEvidence.ps1` crops to the DPI-aware client area inset 8 px and keeps the window topmost. Two 10 s takes at 1584x884, about 29.7 FPS in the container. `-game` on the editor binary, not packaged; not evidence for PIE. Also verified in WeaponsOfOrder on 8 Nov 2026 (#39, tested commit `3e9bb2d`). From `fc04b91` the script fails and deletes the take on focus loss during recording or an FFmpeg error (both observed in #41). libx264 ran out of memory once with the editor and Project Browser also open, so close them first. |
| Video: FFmpeg `gdigrab` `title=` | **Rejected** 8 Nov 2026. At 125% display scale it recorded the wrong region and neighbouring window text. Do not use for evidence. |
| Upload to draft GitHub release + `gh release download` + hash match | **Verified** by Claude Code authenticated as the owner, 8 Nov 2026 (#41, `evidence-issue41-capture-smoke-20261108`). See [STORAGE.md](STORAGE.md). |
| Implementer download and pixel inspection | **Verified** by Claude Code, 8 Nov 2026 (#41): PNGs and sampled video frames read from the downloaded files. |
| ChatGPT reviewer retrieval and pixel inspection | Unverified. Only the reviewer can test it. A draft needs an authenticated client with repository write access. |
| EngineWire viewport capture in a session started before the editor | **Verified** 8 Nov 2026 (#41) after the owner pressed Reconnect on `unreal-engine` in the app with the editor open. The assistant cannot reconnect it. The Unreal Project Browser alone does not start the server; MCPSandbox must be open. |

When a task first needs a route, verify it within that task, record the result here (date, machine, engine, commit, command, outcome), and only then rely on it.

## Required fields per capture

Every screenshot or video in an evidence index states:

- **Scenario** — what is shown and how to reproduce it (map, spawn, actions, console commands).
- **Tested commit** — full SHA; distinguish from later docs-only commits.
- **Machine and OS**, GPU, driver.
- **Engine** — version and build configuration (Editor PIE, `-game`, Development/Shipping packaged).
- **Render settings** — resolution, scalability level, RHI (DX12/DX11/Vulkan), VSync, frame-rate cap.
- **Measured FPS / frame time** where relevant, with the measuring method (`stat unit`, `stat fps`, CSV profiler, Unreal Insights).
- **Timestamps** — capture time; for videos, duration and the relevant timestamps.
- **Original file** — unmodified; derivatives (trims, frames, compressions) are labelled as such with the exact transformation.
- **SHA-256** of every file.
- **Retrieval route** — release tag and asset name, plus the command to download it.
- **Limitations** — e.g. editor viewport vs packaged build, missing HUD, no audio, sampled-only review.

## Rules

- A screenshot is visual evidence of one scenario only. It does not prove performance, correctness elsewhere, or packaged behaviour.
- PIE captures are not packaged-build captures. Say which.
- Performance claims need a packaged build and named hardware (see [PROJECT.md](PROJECT.md#performance-objective)).
- Check the target window is visible and unobstructed before desktop capture; stop if obscured.
- Never trim or edit a capture silently. Keep the original.
- Video review by sampled frames is reported as sampled, with the sampling rate and unexamined intervals.
- Audio is out of scope unless a task requires it and a route is verified.
- Local paths or file listings are not proof a remote reviewer can access the media.
