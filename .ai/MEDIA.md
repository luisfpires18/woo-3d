# Media evidence contract (Unreal)

## Capability status

One route is verified, below. woo-rts capture results (Unity Game view, FFmpeg `ddagrab`, helper scripts) are **not** evidence that any route works for Unreal; revalidate each one here.

| Route | Status |
|---|---|
| Editor viewport PNG through EngineWire `control_editor.screenshot` (`mode: editor_viewport`, `returnBase64: false`) | **Verified** 8 Nov 2026, LP-VICTUS, UE 5.8.4 editor, Radeon RX 5500M, tested commit `21e834f`, MCPSandbox only (#40). 1523x808 PNG written to `Saved/Screenshots`. Editor viewport only, not realtime. Not evidence for PIE, `-game` or packaged captures. |
| In-engine screenshot (`HighResShot`, `Shot` console commands; `FScreenshotRequest`) | Unverified |
| Automation screenshot comparison (Functional Testing / Screenshot Comparison) | Unverified |
| Video: Movie Render Queue (editor, deterministic, offline) | Unverified |
| Video: desktop capture of a running `-game` window (e.g. FFmpeg `ddagrab`) | Unverified on this engine and this GPU |
| Upload to draft GitHub release + `gh release download` + hash match | Unverified in this repo (see [STORAGE.md](STORAGE.md)) |
| Reviewer retrieval and pixel inspection | Unverified |

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
