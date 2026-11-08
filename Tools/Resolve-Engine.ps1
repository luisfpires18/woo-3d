<#
.SYNOPSIS
  Resolves the Unreal Engine root for WeaponsOfOrder.uproject. Dot-source it, then call Resolve-UEEngineDir.

  Order (first hit wins):
    1. -EngineDir argument (explicit override).
    2. HKLM:\SOFTWARE\EpicGames\Unreal Engine\<EngineAssociation>\InstalledDirectory  (Epic Launcher install).
    3. %ProgramData%\Epic\UnrealEngineLauncher\LauncherInstalled.dat entry AppName "UE_<EngineAssociation>".
    4. HKCU:\SOFTWARE\Epic Games\Unreal Engine\Builds: a value equal to the association (GUID-registered
       builds), else any registered build whose Engine/Build/Build.version Major.Minor equals the association.
  Fails with a clear message if none match. Never writes the registry or environment.
#>
function Resolve-UEEngineDir {
    param([string]$ProjectFile, [string]$EngineDir)

    if ($EngineDir) { $cands = @($EngineDir) } else {
        $assoc = (Get-Content -Raw $ProjectFile | ConvertFrom-Json).EngineAssociation
        $cands = @()
        $hklm = Get-ItemProperty "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$assoc" -ErrorAction SilentlyContinue
        if ($hklm) { $cands += $hklm.InstalledDirectory }
        $dat = Join-Path $env:ProgramData 'Epic\UnrealEngineLauncher\LauncherInstalled.dat'
        if (Test-Path $dat) {
            $cands += (Get-Content -Raw $dat | ConvertFrom-Json).InstallationList |
                Where-Object { $_.AppName -eq "UE_$assoc" } | ForEach-Object { $_.InstallLocation }
        }
        $builds = Get-ItemProperty 'HKCU:\SOFTWARE\Epic Games\Unreal Engine\Builds' -ErrorAction SilentlyContinue
        if ($builds) {
            $vals = $builds.PSObject.Properties | Where-Object { $_.Name -notlike 'PS*' }
            $cands += $vals | Where-Object { $_.Name -eq $assoc } | ForEach-Object { $_.Value }
            foreach ($v in $vals) {
                $bv = Join-Path $v.Value 'Engine/Build/Build.version'
                if (Test-Path $bv) {
                    $j = Get-Content -Raw $bv | ConvertFrom-Json
                    if ("$($j.MajorVersion).$($j.MinorVersion)" -eq $assoc) { $cands += $v.Value }
                }
            }
        }
    }
    foreach ($c in $cands) {
        if ($c -and (Test-Path (Join-Path $c 'Engine/Build/BatchFiles/Build.bat'))) { return (Resolve-Path $c).Path }
    }
    throw "Unreal Engine not found for '$ProjectFile' (association '$assoc'). Pass -EngineDir <root containing Engine\>. See docs/BUILD_RUNBOOK.md."
}
