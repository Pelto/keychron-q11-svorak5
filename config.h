// config.h — Keychron Q11 custom keymap

#pragma once

// ── Split sync ───────────────────────────────────────────────────────────────
// Sync layer state to the slave half so RGB indicators work on both sides.
#define SPLIT_LAYER_STATE_ENABLE
// Sync host LED state (Caps Lock, Num Lock, etc.) to the slave half so the
// right-side Shift key RGB indicator also lights up when Caps Lock is active.
#define SPLIT_LED_STATE_ENABLE
// Custom RPC transaction IDs for syncing state to the slave half.
// QMK does not sync caps_word or keypress hit data natively; we push
// them via housekeeping_task_user.
#define SPLIT_TRANSACTION_IDS_USER USER_SYNC_CAPS_WORD, USER_SYNC_FADE
// Increase RPC buffer size to fit last_hit_t (41 bytes) for fade sync.
#define RPC_M2S_BUFFER_SIZE 64
#define RPC_S2M_BUFFER_SIZE 64

// ── RGB reactive fade ────────────────────────────────────────────────────────
// Enable keypress hit tracking so g_last_hit_tracker is populated.
// We read it directly in rgb_matrix_indicators_advanced_user to blend
// a white flash that fades back to the layer's base color.
#define RGB_MATRIX_KEYPRESSES

// ── Tap-hold behaviour ────────────────────────────────────────────────────────
// Hold is triggered when another key is pressed AND fully released while the
// LT() key is still held. This is more forgiving than HOLD_ON_OTHER_KEY_PRESS
// for fast typing: a natural space-to-next-letter roll (next letter not yet
// released when space lifts) registers as a tap. Intentional modifier use
// (hold space → press shortcut → release shortcut → release space) still
// triggers hold correctly because the shortcut completes its cycle first.
#define PERMISSIVE_HOLD
