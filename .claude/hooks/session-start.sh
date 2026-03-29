#!/bin/bash
set -euo pipefail

# Only run in Claude Code on the web (remote environment)
if [ "${CLAUDE_CODE_REMOTE:-}" != "true" ]; then
  exit 0
fi

# ── ARM cross-compiler ──────────────────────────────────────────────────────
if ! command -v arm-none-eabi-gcc &>/dev/null; then
  apt-get update -qq
  apt-get install -y -qq gcc-arm-none-eabi libnewlib-arm-none-eabi
fi

# ── QMK CLI (Python) ────────────────────────────────────────────────────────
if ! command -v qmk &>/dev/null; then
  pip3 install --quiet setuptools qmk
fi

# ── Clone QMK firmware + modules (make handles this, but pre-cloning
#    avoids the 2-min wait on first build) ────────────────────────────────────
cd "$CLAUDE_PROJECT_DIR"
if [ ! -d .build/qmk_firmware ]; then
  make link
fi
