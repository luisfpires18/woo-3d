#!/usr/bin/env bash
# Run a bpy script in headless Blender and exit with its status.
#   blender.sh [file.blend] <script.py> [script args...]
# Script args arrive after "--" in sys.argv. Binary: $BLENDER_BIN, then blender on PATH, then the default install paths.
set -euo pipefail

usage="usage: blender.sh [file.blend] <script.py> [args...]"
blend=()
if [[ ${1:-} == *.blend ]]; then blend=("$1"); shift; fi
script=${1:?$usage}; shift
[ -f "$script" ] || { echo "no such script: $script" >&2; exit 2; }

bin=${BLENDER_BIN:-}
if [ -z "$bin" ]; then
  bin=$(command -v blender || true)
fi
if [ -z "$bin" ]; then
  for c in /c/Program\ Files/Blender\ Foundation/Blender*/blender.exe /Applications/Blender.app/Contents/MacOS/Blender; do
    [ -x "$c" ] && bin=$c
  done
fi
[ -n "$bin" ] || { echo "Blender not found: set BLENDER_BIN" >&2; exit 2; }

exec "$bin" --background ${blend[@]+"${blend[@]}"} --python-exit-code 1 --python "$script" -- "$@"
