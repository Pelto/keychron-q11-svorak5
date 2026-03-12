// config.h — Keychron Q11 custom keymap

#pragma once

// ── Split sync ───────────────────────────────────────────────────────────────
// Sync layer state to the slave half so RGB indicators work on both sides.
#define SPLIT_LAYER_STATE_ENABLE
// Sync host LED state (Caps Lock, Num Lock, etc.) to the slave half so the
// right-side Shift key RGB indicator also lights up when Caps Lock is active.
#define SPLIT_LED_STATE_ENABLE
// Custom RPC transaction ID for syncing Caps Word state to the slave half.
// QMK does not sync caps_word natively; we push it via housekeeping_task_user.
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_CAPS_WORD

// ── Unicode input modes ───────────────────────────────────────────────────────
// Both modes are compiled in. The firmware auto-selects based on the
// Mac/Win switch at boot (see dip_switch_update_user in keymap.c).
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
