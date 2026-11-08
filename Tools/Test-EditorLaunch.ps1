<#
.SYNOPSIS
  Editor-launch smoke check, separate from the build. Starts UnrealEditor.exe (GUI) on the project,
  waits for "Engine Initialization", asks the main window to close (WM_CLOSE, same as the X button),
  waits for exit, then scans the log for Error/Fatal lines.
  Exit 0 = initialized, closed cleanly, no error lines. Does not touch registry or environment.
.EXAMPLE
  powershell -ExecutionPolicy Bypass -File Tools\Test-EditorLaunch.ps1
#>
param([string]$EngineDir, [int]$InitTimeoutSec = 900, [int]$ExitTimeoutSec = 180)
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\Resolve-Engine.ps1"

$root = (Resolve-Path "$PSScriptRoot\..").Path
$proj = Join-Path $root 'WeaponsOfOrder.uproject'
$engine = Resolve-UEEngineDir -ProjectFile $proj -EngineDir $EngineDir
$exe = Join-Path $engine 'Engine\Binaries\Win64\UnrealEditor.exe'
$logDir = Join-Path $root 'Saved\BuildLogs'
New-Item -ItemType Directory -Force $logDir | Out-Null
$log = Join-Path $logDir ("editor-launch-{0}.log" -f (Get-Date -Format 'yyyyMMdd-HHmmss'))

$sw = [Diagnostics.Stopwatch]::StartNew()
$p = Start-Process $exe -ArgumentList "`"$proj`"", '-log', "-abslog=`"$log`"", '-nosplash', '-NoSound' -PassThru

$init = $null
while (-not $init -and $sw.Elapsed.TotalSeconds -lt $InitTimeoutSec -and -not $p.HasExited) {
    Start-Sleep 5
    if (Test-Path $log) { $init = Select-String -Path $log -Pattern 'Engine Initialization\) Total time' | Select-Object -First 1 }
}
if (-not $init) {
    Write-Host "FAIL: no engine initialization within ${InitTimeoutSec}s (exited=$($p.HasExited)). Log: $log"
    if (-not $p.HasExited) { $p.CloseMainWindow() | Out-Null }
    exit 2
}
Write-Host "Initialized after $([int]$sw.Elapsed.TotalSeconds)s: $($init.Line.Trim())"
Start-Sleep 10   # let the editor settle (asset registry, first frame)

$p.Refresh(); [void]$p.CloseMainWindow()
if (-not $p.WaitForExit($ExitTimeoutSec * 1000)) {
    Write-Host "FAIL: editor did not exit within ${ExitTimeoutSec}s after WM_CLOSE; left running for inspection (PID $($p.Id))."
    exit 3
}
$errs = Select-String -Path $log -Pattern '(^|\s)(Error|Fatal):' -CaseSensitive
$warns = (Select-String -Path $log -Pattern 'Warning:' -CaseSensitive).Count
Write-Host "Editor exit code: $($p.ExitCode); errors: $($errs.Count); warnings: $warns; log: $log"
$errs | ForEach-Object { Write-Host "  $($_.Line.Trim())" }
if ($p.ExitCode -ne 0 -or $errs) { exit 1 }
exit 0
