<#
.SYNOPSIS
  Runs a map with -game, takes a HighResShot and records only the game window with FFmpeg.
  Holds A during the recording so the frames show camera motion. WOO-008 (#41).
  -Keys and -Mouse replace that with a scripted timeline of real OS input (WOO-014, #14):
    -Keys  'start-end:VK;...'   e.g. '1-3.5:0x57;3.6-3.8:0x20'  (VK codes, seconds from record start)
    -Mouse 'start-end:dx,dy;...' relative mouse move per 100 ms tick while active
.NOTES
  ddagrab is cropped to the DPI-aware client rectangle, so only game pixels enter the file.
  Do not use gdigrab title=: at 125% DPI it pulled neighbouring window text into the frame.
  Exits non-zero and deletes the take if the game loses foreground at any point in the
  recording, because the window could then be covered, or if FFmpeg exits non-zero.
#>
param(
    [Parameter(Mandatory)][string]$OutDir,
    [string]$Project = 'D:\Workspace\unrealengine\MCPSandbox\MCPSandbox.uproject',
    [string]$Map = '/Game/WOO007/L_WOO007_McpTest',
    [string]$Editor = 'D:\Apps\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe',
    [int]$Seconds = 10,
    [int]$WarmupSeconds = 30,
    [int]$Fps = 30,
    [string]$Keys = '2-5:0x41',
    [string]$Mouse = ''
)
$ErrorActionPreference = 'Stop'
function Parse-Timeline([string]$Spec) {
    foreach ($e in ($Spec -split ';' | Where-Object { $_.Trim() })) {
        $range, $val = $e.Split(':'); $a, $b = $range.Split('-')
        [pscustomobject]@{ From = [double]$a; To = [double]$b; Val = $val; Down = $false }
    }
}
$keyPlan = @(Parse-Timeline $Keys)
$mousePlan = @(Parse-Timeline $Mouse)
Add-Type -Namespace W -Name U -MemberDefinition @'
[DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
[DllImport("user32.dll")] public static extern IntPtr GetForegroundWindow();
[DllImport("user32.dll")] public static extern bool SetProcessDPIAware();
[DllImport("user32.dll")] public static extern bool GetClientRect(IntPtr h, out RECT r);
[DllImport("user32.dll")] public static extern bool ClientToScreen(IntPtr h, ref POINT p);
[DllImport("user32.dll")] public static extern bool SetWindowPos(IntPtr h, IntPtr a, int x, int y, int cx, int cy, uint f);
[StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
[StructLayout(LayoutKind.Sequential)] public struct POINT { public int X, Y; }
[DllImport("user32.dll")] public static extern void keybd_event(byte k, byte s, uint f, UIntPtr e);
[DllImport("user32.dll")] public static extern uint MapVirtualKey(uint code, uint mapType);
[DllImport("user32.dll")] public static extern void mouse_event(uint f, int dx, int dy, uint d, UIntPtr e);
'@
[W.U]::SetProcessDPIAware() | Out-Null
New-Item -ItemType Directory -Force $OutDir | Out-Null
$shots = Join-Path (Split-Path $Project) 'Saved\Screenshots'
$started = Get-Date

$game = Start-Process $Editor -PassThru -ArgumentList @(
    "`"$Project`"", $Map, '-game', '-windowed', '-ResX=1280', '-ResY=720', '-WinX=100', '-WinY=100',
    '-nosplash', '-nosound', '-DisablePlugins=EngineWire,ModelContextProtocol,AllToolsets,EditorToolset,AnimationAssistantToolset,AutomationTestToolset,ToolsetRegistry', "-abslog=`"$OutDir\game.log`"",
    '-ExecCmds="t.MaxFPS 60,HighResShot 1"')
try {
    $deadline = (Get-Date).AddSeconds(120)
    do { Start-Sleep 2; $game.Refresh() } until ($game.MainWindowHandle -ne 0 -or (Get-Date) -gt $deadline)
    if ($game.MainWindowHandle -eq 0) { throw 'Game window did not appear.' }
    Start-Sleep $WarmupSeconds
    $game.Refresh()
    $title = $game.MainWindowTitle
    $hwnd = $game.MainWindowHandle
    for ($i = 0; $i -lt 30 -and [W.U]::GetForegroundWindow() -ne $hwnd; $i++) {
        [W.U]::SetForegroundWindow($hwnd) | Out-Null   # Windows can refuse this while the user is typing
        Start-Sleep 1
    }
    if ([W.U]::GetForegroundWindow() -ne $hwnd) { throw 'Game window is not foreground; not recording.' }

    $video = Join-Path $OutDir 'game-window.mp4'
    [W.U]::SetWindowPos($hwnd, [IntPtr](-1), 0, 0, 0, 0, 3) | Out-Null   # topmost, keep size and position
    $rect = New-Object W.U+RECT; [W.U]::GetClientRect($hwnd, [ref]$rect) | Out-Null
    $pt = New-Object W.U+POINT; [W.U]::ClientToScreen($hwnd, [ref]$pt) | Out-Null
    # Inset 8 px: the window's rounded corners and accent border are not game pixels and can show the desktop.
    $w = ($rect.R - $rect.L - 16) -band -2; $h = ($rect.B - $rect.T - 16) -band -2     # even sizes for yuv420p
    $pt.X += 8; $pt.Y += 8
    $grab = "ddagrab=framerate=${Fps}:draw_mouse=0:offset_x=$($pt.X):offset_y=$($pt.Y):video_size=${w}x${h},hwdownload,format=bgra"
    $ff = Start-Process ffmpeg -PassThru -NoNewWindow -RedirectStandardError (Join-Path $OutDir 'ffmpeg.log') -ArgumentList @(
        '-y', '-hide_banner', '-f', 'lavfi', '-i', $grab, '-t', $Seconds, '-c:v', 'libx264', '-preset', 'veryfast',
        '-crf', '18', '-pix_fmt', 'yuv420p', "`"$video`"")
    $null = $ff.Handle   # keeps ExitCode readable under PowerShell 5.1
    $lost = $false
    $clock = [Diagnostics.Stopwatch]::StartNew()
    try {
        # Watch focus for the whole recording. Polled every 100 ms, so a shorter loss can pass unseen.
        while (-not $ff.HasExited) {
            if ([W.U]::GetForegroundWindow() -ne $hwnd) { $lost = $true; break }
            $t = $clock.Elapsed.TotalSeconds
            foreach ($k in $keyPlan) {
                $hold = $t -ge $k.From -and $t -lt $k.To
                if ($hold -ne $k.Down) {
                    $vk = [Convert]::ToInt32($k.Val, 16); [W.U]::keybd_event([byte]$vk, [byte][W.U]::MapVirtualKey($vk, 0), [uint32]$(if ($hold) { 0 } else { 2 }), [UIntPtr]::Zero)   # real scan code: UE needs it to tell L/R Shift
                    $k.Down = $hold
                }
            }
            foreach ($m in $mousePlan) {
                if ($t -ge $m.From -and $t -lt $m.To) {
                    $dx, $dy = $m.Val.Split(','); [W.U]::mouse_event(1, [int]$dx, [int]$dy, 0, [UIntPtr]::Zero)   # 1 = MOVE
                }
            }
            Start-Sleep -Milliseconds 100
        }
    } finally {
        foreach ($k in $keyPlan) { if ($k.Down) { $vk = [Convert]::ToInt32($k.Val, 16); [W.U]::keybd_event([byte]$vk, [byte][W.U]::MapVirtualKey($vk, 0), 2, [UIntPtr]::Zero) } }  # release, always
    }
    if ($lost) {
        if (-not $ff.HasExited) { Stop-Process -Id $ff.Id; $ff.WaitForExit() }
        Remove-Item -LiteralPath $video -ErrorAction SilentlyContinue
        throw 'Game lost foreground during recording; take discarded.'
    }
    $ff.WaitForExit()
    if ($ff.ExitCode -ne 0) {
        Remove-Item -LiteralPath $video -ErrorAction SilentlyContinue
        throw "FFmpeg exited with code $($ff.ExitCode); take discarded. See ffmpeg.log."
    }
    Get-ChildItem $shots -Recurse -Filter *.png | Where-Object LastWriteTime -gt $started |
        Copy-Item -Destination $OutDir
    "title=$title"
    "client=${w}x$h at $($pt.X),$($pt.Y)"
    "ffmpeg_exit=$($ff.ExitCode)"
} finally {
    if (-not $game.HasExited) { Stop-Process -Id $game.Id }
}
Get-ChildItem $OutDir | Select-Object Name, Length
