# Build and editor-launch runbook

How to build `WeaponsOfOrderEditor` and smoke-test the editor from a fresh checkout on Windows. Added by WOO-002 (#35).

## Tested environment

| Item | Value |
|---|---|
| Machine | LP-VICTUS, Windows 11 Pro 10.0.26200, Ryzen 7 5800H, Radeon RX 5500M 4 GB |
| Engine | UE 5.8.4 installed build (CL 58902052, `++UE5+Release-5.8`) at `D:/Apps/Epic Games/UE_5.8` |
| Toolchain | Visual Studio 2026 Community 18.10, MSVC 14.51.36231 (compiler 14.51.36260), Windows SDK 10.0.26100.0 |
| Target / config | `WeaponsOfOrderEditor Win64 Development` |
| DDC | project-local `DerivedDataCache/` (see `Config/DefaultEngine.ini`) |

UFO-LP is not verified yet. Re-run both scripts there after migration.

## Commands

From the repository root, in PowerShell or cmd:

```bash
powershell -NoProfile -ExecutionPolicy Bypass -File Tools/Build-Editor.ps1 -Clean
```

```bash
powershell -NoProfile -ExecutionPolicy Bypass -File Tools/Build-Editor.ps1
```

```bash
powershell -NoProfile -ExecutionPolicy Bypass -File Tools/Test-EditorLaunch.ps1
```

- `Build-Editor.ps1` runs `Engine\Build\BatchFiles\Build.bat WeaponsOfOrderEditor Win64 Development -project=<uproject> -waitmutex -NoHotReload`. `-Clean` first deletes only `Binaries/` and `Intermediate/` in the project. The script exits with UBT's exit code and prints `RESULT exit=… elapsed=…`.
- `Test-EditorLaunch.ps1` is a separate step. It opens the GUI editor, waits for `(Engine Initialization) Total time`, sends WM_CLOSE (the same as clicking X), waits for the process to exit, then lists `Error`, `Fatal` and `Warning` log lines. It exits 0 only for a clean exit with no errors. Exit 2 means the editor did not initialise. Exit 3 means it did not close, and the script leaves it running so you can inspect it.
- Logs go to `Saved/BuildLogs/`, which Git ignores.
- Both scripts take `-EngineDir <root>` to override engine discovery.

## How the engine is found

`Tools/Resolve-Engine.ps1` reads `EngineAssociation` (currently `5.8`) from `WeaponsOfOrder.uproject` and tries these in order:

1. `-EngineDir`.
2. `HKLM\SOFTWARE\EpicGames\Unreal Engine\<assoc>\InstalledDirectory` (Epic Launcher installs).
3. `%ProgramData%\Epic\UnrealEngineLauncher\LauncherInstalled.dat`, entry `UE_<assoc>`.
4. `HKCU\SOFTWARE\Epic Games\Unreal Engine\Builds`. It takes a value named after the association, or any registered build whose `Engine/Build/Build.version` major.minor matches.

On LP-VICTUS only step 4 matches. The engine is registered there under a GUID. The HKLM key is absent and `LauncherInstalled.dat` is empty. The scripts only read the registry and environment and never write to them.

## Troubleshooting a fresh checkout

| Symptom | Cause / fix |
|---|---|
| `Unreal Engine not found …` | The engine is not installed or not registered. Pass `-EngineDir "D:/Apps/Epic Games/UE_5.8"`. To register it permanently, run `Engine/Binaries/Win64/UnrealVersionSelector.exe -register` from the engine root. That command writes HKCU, so it is your choice and the scripts never run it. |
| `running scripts is disabled on this system` | Keep `-ExecutionPolicy Bypass` on the command line. Do not change the machine policy. |
| UBT: `Visual Studio … not found` / no MSVC toolset | Install VS 2022/2026 with the **Game development with C++** workload, MSVC v14.5x, and a Windows 10/11 SDK. |
| `Warning: Visual Studio compiler 14.51.36260 is not a preferred version` | Expected on LP-VICTUS. UE 5.8 prefers 14.50.35717. The build still succeeds. Installing 14.50 side by side is an optional owner decision. |
| UBT waits on `-waitmutex` | Another UBT or editor build is running. Wait for it, or close Visual Studio or the editor. |
| `Unable to build while Live Coding is active` | Close the editor before building. |
| Link fails: `cannot open file …UnrealEditor-WeaponsOfOrder.dll` | The editor still has the DLL loaded. Close it and rebuild. |
| Editor shows "modules are missing or built with a different engine version" | Run `Build-Editor.ps1` first. Don't let the editor rebuild for you, because the scripts are the recorded route. |
| Strange UBT or intermediate errors after an engine update | Run `Build-Editor.ps1 -Clean`. |
| Log: `LogDirectoryWatcher: Warning: Failed to begin reading directory changes for …/Content/` | Expected while the project has no `Content/` folder. |
| Log: `LogLayoutService: Warning: … UnrealEd_Layout_v1.5 …` | Comes from per-user engine layout settings and is harmless. |
| Log: `LogHttp: Warning: … Unbinding delegates for 1 outstanding Http Requests` | An Epic analytics upload was still pending at shutdown. Harmless. |
| Sluggish first launch (~60–70 s) | First-time shader and DDC warm-up. Later launches take about 15 s on LP-VICTUS. |
| C: disk filling up | The DDC is project-local on D:. Check that the log shows `Local: Using data cache path …/DerivedDataCache: Writable`. |

## Side effects to know about

Running the GUI editor writes normal per-user state outside the repository: `%LOCALAPPDATA%\UnrealEngine\5.8\Saved\Config\WindowsEditor\*.ini` (layout and editor settings), `%LOCALAPPDATA%\UnrealEngine\Editor\*.json` (recent projects, layouts), analytics, and UnrealTrace logs. The editor writes these itself on every launch. The scripts set no environment variables and change no registry keys.

Build products go only to `Binaries/`, `Intermediate/` and `Saved/`, which Git ignores. Generating project files (not part of these scripts) also creates `*.slnx` and `.vsconfig`. #36 covers ignoring those.
