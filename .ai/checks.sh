#!/usr/bin/env bash
# Pre-push gate for this repository. scripts/gate.py runs the integration
# branch's copy of this file inside the task's worktree, so a branch cannot
# weaken its own checks. Fast, deterministic, non-interactive; no full build.
# Engine-free: no UBT build, no editor launch, no automation tests (those need
# Unreal; see docs/BUILD_RUNBOOK.md and the verification levels in .ai/PROJECT.md).
set -euo pipefail

py=$(command -v python || command -v python3 || command -v py || true)
[ -n "$py" ] || { echo "checks.sh: python not found on PATH" >&2; exit 1; }
command -v powershell >/dev/null || { echo "checks.sh: powershell not found on PATH" >&2; exit 1; }

echo "== .uproject JSON"
"$py" -c "import json,sys; json.load(open(sys.argv[1], encoding='utf-8'))" WeaponsOfOrder.uproject

echo "== Tools/*.py syntax"
"$py" -c "import ast,sys; [ast.parse(open(f, encoding='utf-8').read(), f) for f in sys.argv[1:]]" Tools/*.py

echo "== Tools/*.ps1 parse"
powershell -NoProfile -NonInteractive -Command '
$bad = 0
foreach ($f in Get-ChildItem Tools -Filter *.ps1) {
  $errs = $null
  [void][System.Management.Automation.Language.Parser]::ParseFile($f.FullName, [ref]$null, [ref]$errs)
  foreach ($e in $errs) { Write-Host "$($f.Name):$($e.Extent.StartLineNumber): $($e.Message)"; $bad = 1 }
}
exit $bad'

echo "== bash -n scripts/*.sh"
for f in scripts/*.sh; do bash -n "$f"; done

echo "checks.sh: OK"
