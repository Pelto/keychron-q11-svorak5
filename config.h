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

// ── Tap-hold behaviour ────────────────────────────────────────────────────────
// Hold is triggered when another key is pressed AND fully released while the
// LT() key is still held. This is more forgiving than HOLD_ON_OTHER_KEY_PRESS
// for fast typing: a natural space-to-next-letter roll (next letter not yet
// released when space lifts) registers as a tap. Intentional modifier use
// (hold space → press shortcut → release shortcut → release space) still
// triggers hold correctly because the shortcut completes its cycle first.
#define PERMISSIVE_HOLD

// ── Unicode input modes ───────────────────────────────────────────────────────
// Mac uses native Swedish keystrokes — no Unicode input mode needed.
// Win uses WinCompose for the SPECIAL layer symbols.
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE

// Persist the last used Unicode mode across power cycles (uses EEPROM).
// Set to false if you want the switch to always be the source of truth.
#define UNICODE_CYCLE_PERSIST false
