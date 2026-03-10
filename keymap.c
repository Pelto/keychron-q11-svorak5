// Keychron Q11 ISO Nordic — Custom keymap with Unicode output
//
// Special characters are sent as Unicode codepoints, not keycode combos.
// This means they arrive as characters at the application level, bypassing
// any application shortcut hijacking (e.g. Google Sheets, browser shortcuts).
//
// Mac setup required:
//   System Preferences → Keyboard → Input Sources → add "Unicode Hex Input"
//   and select it as active input source.
//
// Windows setup required:
//   Install WinCompose: https://github.com/samhocevar/wincompose
//   QMK will use it automatically in WINCOMPOSE mode.
//
// The physical Mac/Win switch on the Q11 sets the default layer at boot,
// which we use to auto-switch Unicode input mode.

#include QMK_KEYBOARD_H

// ──────────────────────────────────────────────
// Layer definitions
// ──────────────────────────────────────────────
enum layers {
    MAC_BASE,
    WIN_BASE,
    GAME,    // QWERTY layer for gaming — toggle with FN+H
    _FN,
};

// ──────────────────────────────────────────────
// Custom keycodes
// ──────────────────────────────────────────────
enum custom_keycodes {
    CK_LOPT = SAFE_RANGE,  // Custom left Option/Alt — activates special chars
};

// ──────────────────────────────────────────────
// Unicode codepoints for all special characters.
// Prefixed CP_ (codepoint) to avoid collision with QMK's UC_* keycode defines.
// ──────────────────────────────────────────────

// Row 1: å ä ö p y f g c r l , "
#define CP_LCBR 0x007B  // {
#define CP_RCBR 0x007D  // }
#define CP_LBRC 0x005B  // [
#define CP_RBRC 0x005D  // ]
#define CP_DLR  0x0024  // $
#define CP_DQUO 0x0022  // "
#define CP_QUES 0x003F  // ?
#define CP_AMPR 0x0026  // &
#define CP_LT   0x003C  // <
#define CP_GT   0x003E  // >
#define CP_SPC  0x0020  // (space)
#define CP_TILD 0x007E  // ~

// Row 2: a o e u i d h t n s - '
#define CP_SEMI 0x003B  // ;
#define CP_SLSH 0x002F  // /
#define CP_LPRN 0x0028  // (
#define CP_RPRN 0x0029  // )
#define CP_PIPE 0x007C  // |
#define CP_HASH 0x0023  // #

// Row 3: (unused) q j k x b m w v z
#define CP_COLN 0x003A  // :
#define CP_EQL  0x003D  // =
#define CP_AT   0x0040  // @
#define CP_EXCL 0x0021  // !
#define CP_BSLS 0x005C  // backslash
#define CP_PERC 0x0025  // %
#define CP_DOT  0x002E  // .

// ──────────────────────────────────────────────
// Keymaps
// ──────────────────────────────────────────────
// LAYOUT_92_iso row structure (92 keys):
//   Row 0 (17): encoder_click  Esc F1..F12  Ins Del  Mute
//   Row 1 (16): extra  ` 1..0 - =  Bspc         PgUp
//   Row 2 (15): extra  Tab Q..P [ ]              PgDn
//   Row 3 (16): extra  Caps A..L ; ' #  Enter    Home
//   Row 4 (15): extra  LSft NUBS Z../ RSft        Up
//   Row 5 (13): extra  LCtl LOpt LCmd FN  Spc Spc  RCmd ROpt RCtl  Left Down Right
//
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// ── MAC BASE ──────────────────────────────────────────────────────────────────
// Dvorak layout with Swedish characters.
// ISO Nordic HID positions:
//   KC_LBRC = å   KC_QUOT = ä   KC_SCLN = ö
//   KC_SLSH = ,   KC_EQL  = "
//   KC_NUBS = unused bottom-left ISO key
[MAC_BASE] = LAYOUT_92_iso(
    KC_MUTE,  KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_TRNS, KC_MUTE,
    KC_TRNS,  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_PGUP,
    KC_TRNS,  KC_TAB,  KC_LBRC, KC_QUOT, KC_SCLN, KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_SLSH, KC_EQL,                   KC_PGDN,
    KC_TRNS,  KC_CAPS, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS, KC_TRNS, KC_ENT,           KC_HOME,
    KC_TRNS,  KC_LSFT, KC_NUBS, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_TRNS,          KC_RSFT, KC_UP,
    KC_TRNS,  KC_LCTL, CK_LOPT, KC_LGUI, MO(_FN),          KC_SPC,                    KC_SPC,           KC_RGUI, KC_ROPT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
),

// ── WIN BASE ──────────────────────────────────────────────────────────────────
[WIN_BASE] = LAYOUT_92_iso(
    KC_MUTE,  KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_TRNS, KC_MUTE,
    KC_TRNS,  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_PGUP,
    KC_TRNS,  KC_TAB,  KC_LBRC, KC_QUOT, KC_SCLN, KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_SLSH, KC_EQL,                   KC_PGDN,
    KC_TRNS,  KC_CAPS, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS, KC_TRNS, KC_ENT,           KC_HOME,
    KC_TRNS,  KC_LSFT, KC_NUBS, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_TRNS,          KC_RSFT, KC_UP,
    KC_TRNS,  KC_LCTL, KC_LWIN, CK_LOPT, MO(_FN),          KC_SPC,                    KC_SPC,           KC_RALT, KC_RWIN, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
),

// ── GAME LAYER ────────────────────────────────────────────────────────────────
// Standard QWERTY for gaming. Toggle on/off with FN+H.
// CK_LOPT is replaced with plain KC_LALT — no special character logic while gaming.
// All non-alpha keys fall through to the base layer (KC_TRNS).
[GAME] = LAYOUT_92_iso(
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_LALT, KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS,          KC_TRNS, KC_RALT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    //                 ^ plain Alt, not CK_LOPT — disables special char logic while gaming
),

// ── FN LAYER ──────────────────────────────────────────────────────────────────
// FN+H        → toggle GAME layer on/off
// FN+ROpt     → cycle Unicode input mode
[_FN] = LAYOUT_92_iso(
    RM_TOGG,  KC_TRNS, KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS, RM_TOGG,
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,  RM_TOGG, RM_NEXT, RM_VALU, RM_HUEU, RM_SATU, RM_SPDU, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS,  KC_TRNS, RM_PREV, RM_VALD, RM_HUED, RM_SATD, RM_SPDD, KC_TRNS, TG(GAME),KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    //                                                                          ^ FN+H toggles GAME layer
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS,           KC_TRNS, UC_NEXT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    //                                                                                                              ^ FN+ROpt cycles Unicode mode
),

};
// clang-format on

// ──────────────────────────────────────────────
// RGB indicator — solid red every frame while GAME layer is active
// so you always know you're in QWERTY mode
// ──────────────────────────────────────────────
bool rgb_matrix_indicators_user(void) {
    if (IS_LAYER_ON(GAME)) {
        rgb_matrix_set_color_all(RGB_RED);
        return false;
    }
    return true;
}

// ──────────────────────────────────────────────
// Auto-switch Unicode input mode based on Mac/Win switch.
// Called once at boot after the default layer is set.
// ──────────────────────────────────────────────
void keyboard_post_init_user(void) {
    if (get_highest_layer(default_layer_state) == MAC_BASE) {
        set_unicode_input_mode(UNICODE_MODE_MACOS);
    } else {
        set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
    }
}

// ──────────────────────────────────────────────
// Track left Opt hold state
// ──────────────────────────────────────────────
static bool lopt_held = false;

// ──────────────────────────────────────────────
// Special character lookup — returns Unicode codepoint for a given base keycode.
// Returns 0 if no special character is mapped for that key.
// ──────────────────────────────────────────────
static uint32_t get_special_codepoint(uint16_t keycode) {
    switch (keycode) {
        // Row 1: å ä ö p y f g c r l , "
        case KC_LBRC: return CP_LCBR;  // å → {
        case KC_QUOT: return CP_RCBR;  // ä → }
        case KC_SCLN: return CP_LBRC;  // ö → [
        case KC_P:    return CP_RBRC;  // p → ]
        case KC_Y:    return CP_DLR;   // y → $
        case KC_F:    return CP_DQUO;  // f → "
        case KC_G:    return CP_QUES;  // g → ?
        case KC_C:    return CP_AMPR;  // c → &
        case KC_R:    return CP_LT;    // r → <
        case KC_L:    return CP_GT;    // l → >
        case KC_SLSH: return CP_SPC;   // , → space
        case KC_EQL:  return CP_TILD;  // " → ~

        // Row 2: a o e u i d h t n s - '
        case KC_A:    return CP_SEMI;  // a → ;
        case KC_O:    return CP_SLSH;  // o → /
        case KC_E:    return CP_LPRN;  // e → (
        case KC_U:    return CP_RPRN;  // u → )
        case KC_I:    return CP_PIPE;  // i → |
        case KC_T:    return CP_HASH;  // t → #
        case KC_N:    return CP_DQUO;  // n → "
        // KC_D, KC_H, KC_S, KC_MINS → no output (return 0)

        // Row 3: (unused) q j k x b m w v z
        case KC_Q:    return CP_COLN;  // q → :
        case KC_J:    return CP_EQL;   // j → =
        case KC_K:    return CP_AT;    // k → @
        case KC_X:    return CP_EXCL;  // x → !
        case KC_B:    return CP_BSLS;  // b → backslash
        case KC_M:    return CP_PERC;  // m → %
        case KC_W:    return CP_DOT;   // w → .
        // KC_V, KC_Z, KC_NUBS → no output (return 0)

        default: return 0;
    }
}

// ──────────────────────────────────────────────
// Main key processing
// ──────────────────────────────────────────────
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    const uint8_t mods = get_mods();

    // If any of these are held, Opt behaves normally — no special layer
    const bool bypass = (mods & MOD_BIT(KC_LGUI))  ||  // Cmd / Win
                        (mods & MOD_BIT(KC_RGUI))  ||
                        (mods & MOD_BIT(KC_LCTL))  ||  // Ctrl
                        (mods & MOD_BIT(KC_RCTL))  ||
                        (mods & MOD_BIT(KC_LSFT))  ||  // Shift
                        (mods & MOD_BIT(KC_RSFT));

    switch (keycode) {

        case CK_LOPT:
            if (record->event.pressed) {
                lopt_held = true;
                // Register Alt so Opt-alone shortcuts (e.g. word jump) still work
                register_code(KC_LALT);
            } else {
                lopt_held = false;
                unregister_code(KC_LALT);
            }
            return false;

        default:
            if (lopt_held && !bypass && record->event.pressed) {
                uint32_t codepoint = get_special_codepoint(keycode);
                if (codepoint != 0) {
                    // Release Alt before sending Unicode — the OS Unicode input
                    // sequence must not have Alt held during entry
                    unregister_code(KC_LALT);

                    // Send character as a Unicode codepoint.
                    // On Mac: uses Unicode Hex Input (Opt+hex sequence).
                    // On Win: uses WinCompose sequence.
                    // The application receives a plain character either way.
                    register_unicode(codepoint);

                    // Re-hold Alt for remainder of the Opt press
                    register_code(KC_LALT);

                    return false; // consumed — don't send the base keycode
                }
                // No mapping for this key: fall through.
                // The base key + Alt will be sent as-is, which is correct
                // for any Opt+key combos not in our table.
            }
            break;
    }

    return true;
}

// ──────────────────────────────────────────────
// Encoder (knob) — volume control
// ──────────────────────────────────────────────
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return false;
}
#endif
