<#
.SYNOPSIS
  Builds WeaponsOfOrderEditor Win64 Development with UBT (Build.bat). Exit code = UBT exit code.
.PARAMETER Clean
  Deletes only <project>\Binaries and <project>\Intermediate first (regenerable project outputs).
.EXAMPLE
  powershell -ExecutionPolicy Bypass -File Tools\Build-Editor.ps1 -Clean
#>
param([switch]$Clean, [string]$EngineDir, [string]$Configuration = 'Development')
$ErrorActionPreference = 'Stop'
. "$PSScriptRoot\Resolve-Engine.ps1"

$root = (Resolve-Path "$PSScriptRoot\..").Path
$proj = Join-Path $root 'WeaponsOfOrder.uproject'
$engine = Resolve-UEEngineDir -ProjectFile $proj -EngineDir $EngineDir
Write-Host "Engine:  $engine"
Write-Host "Project: $proj"

if ($Clean) {
    foreach ($d in 'Binaries', 'Intermediate') {
        $p = Join-Path $root $d
        if (Test-Path $p) { Write-Host "Clean: removing $p"; Remove-Item -Recurse -Force $p }
    }
}

$logDir = Join-Path $root 'Saved\BuildLogs'
New-Item -ItemType Directory -Force $logDir | Out-Null
$log = Join-Path $logDir ("build-{0}-{1}.log" -f ($(if ($Clean) { 'clean' } else { 'incr' })), (Get-Date -Format 'yyyyMMdd-HHmmss'))

$sw = [Diagnostics.Stopwatch]::StartNew()
& (Join-Path $engine 'Engine\Build\BatchFiles\Build.bat') WeaponsOfOrderEditor Win64 $Configuration "-project=$proj" -waitmutex -NoHotReload 2>&1 | Tee-Object -FilePath $log
$code = $LASTEXITCODE
$sw.Stop()
Write-Host ("RESULT exit={0} elapsed={1:N2}s log={2}" -f $code, $sw.Elapsed.TotalSeconds, $log)
exit $code
