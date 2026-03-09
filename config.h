// config.h — Keychron Q11 custom keymap

#pragma once

// ── Unicode input modes ───────────────────────────────────────────────────────
// Both modes are compiled in. The firmware auto-selects based on the
// Mac/Win switch at boot (see keyboard_post_init_user in keymap.c).
// You can also cycle manually with UC_NEXT (mapped to FN layer).
#define UNICODE_SELECTED_MODES UNICODE_MODE_MACOS, UNICODE_MODE_WINCOMPOSE

// Persist the last used Unicode mode across power cycles (uses EEPROM).
// Set to false if you want the switch to always be the source of truth.
#define UNICODE_CYCLE_PERSIST false

// ── Unicode key for Mac ───────────────────────────────────────────────────────
// By default QMK uses KC_LALT (left Option) to enter Unicode on Mac.
// We temporarily release our custom Alt before calling register_unicode(),
// so this default is fine and does not need changing.
// #define UNICODE_KEY_MAC KC_LALT  // (this is already the default)
