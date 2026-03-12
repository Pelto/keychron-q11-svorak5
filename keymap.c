// Keychron Q11 ISO Nordic — Custom keymap
//
// Layer stack (higher index = higher priority):
//   0: MAC_SVORAK   — Swedish Dvorak (default when Mac switch)
//   1: MAC_QWERTY   — QWERTY overlay (toggle via MC_1)
//   2: MAC_SPECIAL   — Programming symbols — plain US keycodes (hold RCmd)
//   3: WIN_SVORAK   — Swedish Dvorak (default when Win switch)
//   4: WIN_QWERTY   — QWERTY overlay (toggle via MC_1)
//   5: WIN_SPECIAL   — Programming symbols via Unicode (hold RAlt)
//   6: NUMPAD        — Right-hand numpad (hold MC_5)
//   7: MAC_FN        — Nav, edit, shortcuts (hold FN, Mac mode)
//   8: WIN_FN        — Nav, edit, shortcuts (hold FN, Win mode)
//
// Mac setup required:
//   System Preferences → Keyboard → Input Sources → add "Unicode Hex Input"
//
// Windows setup required:
//   Install WinCompose: https://github.com/samhocevar/wincompose

#include QMK_KEYBOARD_H
#include "transactions.h"

// ──────────────────────────────────────────────
// Layer definitions
// ──────────────────────────────────────────────
enum layers {
    MAC_SVORAK,
    MAC_QWERTY,
    MAC_SPECIAL,
    WIN_SVORAK,
    WIN_QWERTY,
    WIN_SPECIAL,
    NUMPAD,
    MAC_FN,
    WIN_FN,
};

// ──────────────────────────────────────────────
// Custom keycodes — Swedish characters via Unicode
// ──────────────────────────────────────────────
// On Mac the active input source must be "Unicode Hex Input" for the SPECIAL
// layer to work.  That input source uses a US keyboard mapping, so all keys
// that differ between Swedish and US need to send their characters via
// register_unicode() instead of relying on the OS layout.
enum custom_keycodes {
    SE_SECT = SAFE_RANGE,  // § / °
    SE_PLUS,               // + / ?
    SE_ACUT,               // ` / ´
    SE_ARNG,               // å / Å
    SE_ADIA,               // ä / Ä
    SE_ODIA,               // ö / Ö
    SE_APOS,               // ' / *
    SE_LESS,               // < / >
    SE_DOT,                // . / :  (dot unshifted, colon shifted)
    SE_COMM,               // , / ;  (comma unshifted, semicolon shifted)
};

// ──────────────────────────────────────────────
// Tap dance codes
// ──────────────────────────────────────────────
enum tap_dance_codes {
    TD_LSFT,
    TD_RSFT,
};

// ── Mac Special — plain US keycodes, bypasses Unicode Hex Input ──
#define U_LCBR LSFT(KC_LBRC)  // {
#define U_RCBR LSFT(KC_RBRC)  // }
#define U_LBRC KC_LBRC         // [
#define U_RBRC KC_RBRC         // ]
#define U_DLR  LSFT(KC_4)     // $
#define U_DQUO LSFT(KC_QUOT)  // "
#define U_QUES LSFT(KC_SLSH)  // ?
#define U_AMPR LSFT(KC_7)     // &
#define U_LABK LSFT(KC_COMM)  // <
#define U_RABK LSFT(KC_DOT)   // >
#define U_SEMI KC_SCLN         // ;
#define U_SLSH KC_SLSH         // /
#define U_LPRN LSFT(KC_9)     // (
#define U_RPRN LSFT(KC_0)     // )
#define U_PIPE LSFT(KC_BSLS)  // |
#define U_CIRC LSFT(KC_6)     // ^
#define U_HASH LSFT(KC_3)     // #
#define U_TILD UC(0x007E)      // ~ (KC_GRV gives ± on Mac ISO/Swedish layout)
#define U_AT   LSFT(KC_2)     // @
#define U_BSLS KC_BSLS         // backslash
#define U_PERC LSFT(KC_5)     // %
#define U_COLN LSFT(KC_SCLN)  // :
#define U_EQL  KC_EQL          // =
#define U_EXLM LSFT(KC_1)     // !
#define U_GRV  KC_GRV          // `
// ── Win Special — UC() via WinCompose, unchanged ──
#define UW_LCBR UC(0x007B)  // {
#define UW_RCBR UC(0x007D)  // }
#define UW_LBRC UC(0x005B)  // [
#define UW_RBRC UC(0x005D)  // ]
#define UW_DLR  UC(0x0024)  // $
#define UW_DQUO UC(0x0022)  // "
#define UW_QUES UC(0x003F)  // ?
#define UW_AMPR UC(0x0026)  // &
#define UW_LABK UC(0x003C)  // <
#define UW_RABK UC(0x003E)  // >
#define UW_SEMI UC(0x003B)  // ;
#define UW_SLSH UC(0x002F)  // /
#define UW_LPRN UC(0x0028)  // (
#define UW_RPRN UC(0x0029)  // )
#define UW_PIPE UC(0x007C)  // |
#define UW_CIRC UC(0x005E)  // ^
#define UW_HASH UC(0x0023)  // #
#define UW_TILD UC(0x007E)  // ~
#define UW_AT   UC(0x0040)  // @
#define UW_BSLS UC(0x005C)  // backslash
#define UW_PERC UC(0x0025)  // %
#define UW_COLN UC(0x003A)  // :
#define UW_EQL  UC(0x003D)  // =
#define UW_EXLM UC(0x0021)  // !
#define UW_GRV  UC(0x0060)  // `

// ──────────────────────────────────────────────
// Helpers — send Unicode with shift / caps-lock awareness
// ──────────────────────────────────────────────
// Letters: Shift XOR CapsLock selects upper-case.
static void tap_unicode_letter(uint32_t lower, uint32_t upper, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t mods    = get_mods() | get_weak_mods() | get_oneshot_mods();
        bool    shifted = (mods & MOD_MASK_SHIFT) != 0;
        bool    caps    = host_keyboard_led_state().caps_lock || is_caps_word_on();
        register_unicode((shifted ^ caps) ? upper : lower);
    }
}

// Symbols: only Shift selects the alternate character.
static void tap_unicode_symbol(uint32_t lower, uint32_t upper, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t mods    = get_mods() | get_weak_mods() | get_oneshot_mods();
        bool    shifted = (mods & MOD_MASK_SHIFT) != 0;
        register_unicode(shifted ? upper : lower);
    }
}

// ──────────────────────────────────────────────
// Keymaps
// ──────────────────────────────────────────────
// LAYOUT_92_iso row structure (92 keys):
//   Row 0 (17): knob_l  Esc  F1..F12  Ins  Del  knob_r
//   Row 1 (16): MC_1    `    1..0  -  =  Bspc       PgUp
//   Row 2 (15): MC_2    Tab  Q..P  [  ]              PgDn
//   Row 3 (16): MC_3    Caps A..L  ;  '  #   Enter   Home
//   Row 4 (15): MC_4    LSft NUBS Z../    RSft        Up
//   Row 5 (13): MC_5    LCtl LOpt LCmd FN Spc Spc RCmd ROpt RCtl Left Down Right
//
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// ── MAC SVORAK ──────────────────────────────────────────────────────────────────
[MAC_SVORAK] = LAYOUT_92_iso(
    KC_MUTE,        KC_ESC,  KC_BRID, KC_BRIU, KC_MCTL, KC_LPAD, RM_VALD, RM_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, KC_INS,  KC_DEL,  KC_MUTE,
    TG(MAC_QWERTY), SE_SECT, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    SE_PLUS, SE_ACUT, KC_BSPC,           KC_PGUP,
    LSG(KC_4),      KC_TAB,  SE_ARNG, SE_ADIA, SE_ODIA, KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    SE_COMM, KC_RBRC,                    KC_PGDN,
    LCG(KC_Q),      KC_ENT,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS, SE_APOS, KC_ENT,            KC_HOME,
    KC_CALC,        TD(TD_LSFT), SE_LESS, SE_DOT,  KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,             TD(TD_RSFT), KC_UP,
    MO(NUMPAD),     KC_LCTL, KC_LOPT, KC_LCMD, MO(MAC_FN),       KC_SPC,                    KC_SPC,           MO(MAC_SPECIAL), MO(MAC_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
),

// ── MAC QWERTY ──────────────────────────────────────────────────────────────────
[MAC_QWERTY] = LAYOUT_92_iso(
    KC_TRNS,        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    TG(MAC_QWERTY), KC_GRV,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    SE_ARNG, KC_RBRC,                     KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    SE_ODIA, SE_ADIA, SE_APOS, KC_TRNS,           KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_MINS,          KC_TRNS, KC_TRNS,
    MO(NUMPAD),     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── MAC SPECIAL ─────────────────────────────────────────────────────────────────
// Programming symbols — plain US keycodes (macOS)
[MAC_SPECIAL] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, U_LCBR,  U_RCBR,  U_LBRC,  U_RBRC,  U_DLR,   U_DQUO,  U_QUES,  U_AMPR,  U_LABK,  U_RABK,  KC_TRNS, U_TILD,                   KC_TRNS,
    KC_TRNS, KC_TRNS, U_SEMI,  U_SLSH,  U_LPRN,  U_RPRN,  U_PIPE,  KC_TRNS, U_CIRC,  U_HASH,  U_DQUO,  U_TILD,  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, U_COLN,  KC_EQL,  U_AT,    U_EXLM,  U_BSLS,  U_PERC,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── WIN SVORAK ──────────────────────────────────────────────────────────────────
[WIN_SVORAK] = LAYOUT_92_iso(
    KC_MUTE,        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL,  KC_MUTE,
    TG(WIN_QWERTY), KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,           KC_PGUP,
    KC_PSCR,        KC_TAB,  KC_LBRC, KC_QUOT, KC_SCLN, KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_COMM, KC_RBRC,                    KC_PGDN,
    LGUI(KC_L),     KC_ENT,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH, KC_NUHS, KC_ENT,            KC_HOME,
    KC_CALC,        TD(TD_LSFT), KC_NUBS, KC_DOT,  KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,             TD(TD_RSFT), KC_UP,
    MO(NUMPAD),     KC_LCTL, KC_LWIN, KC_LALT, MO(WIN_FN),       KC_SPC,                    KC_SPC,           MO(WIN_SPECIAL), MO(WIN_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
),

// ── WIN QWERTY ──────────────────────────────────────────────────────────────────
[WIN_QWERTY] = LAYOUT_92_iso(
    KC_TRNS,        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    TG(WIN_QWERTY), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                     KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_TRNS,           KC_TRNS,
    KC_TRNS,        KC_TRNS, KC_TRNS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_TRNS, KC_TRNS,
    MO(NUMPAD),     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── WIN SPECIAL ─────────────────────────────────────────────────────────────────
// Programming symbols via WinCompose
[WIN_SPECIAL] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, UW_LCBR, UW_RCBR, UW_LBRC, UW_RBRC, UW_DLR,  UW_DQUO, UW_QUES, UW_AMPR, UW_LABK, UW_RABK, KC_TRNS, UW_TILD,                  KC_TRNS,
    KC_TRNS, KC_TRNS, UW_SEMI, UW_SLSH, UW_LPRN, UW_RPRN, UW_PIPE, KC_TRNS, UW_CIRC, UW_HASH, UW_DQUO, UW_TILD, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, UW_COLN, UW_EQL,  UW_AT,   UW_EXLM, UW_BSLS, UW_PERC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── NUMPAD ──────────────────────────────────────────────────────────────────────
// Right-hand numpad overlay. Hold MC_5 to activate.
[NUMPAD] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSLS, KC_PAST, KC_PMNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_PAST, KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P4,   KC_P5,   KC_P6,   KC_PMNS, KC_PPLS, KC_TRNS, KC_TRNS, KC_PENT,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_P1,   KC_P2,   KC_P3,   KC_PPLS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_PDOT,                    KC_P0,            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── MAC FN LAYER ─────────────────────────────────────────────────────────────────
// Left:  Q/W/E=Ins/Home/PgUp  A/S/D=Del/End/PgDn  F=SelAll  Z/X=Undo/Redo  C/V/B=Copy/Paste/Cut
// Right: U=Bspc  I=Up  O=Del  J=Left  K=Down  L=Right
[MAC_FN] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_INS,  KC_HOME, KC_PGUP, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSPC, KC_UP,   KC_DEL,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_DEL,  KC_END,  KC_PGDN, LGUI(KC_A), KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,       KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, LGUI(KC_Z), SGUI(KC_Z), LGUI(KC_C), LGUI(KC_V), LGUI(KC_X), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

// ── WIN FN LAYER ─────────────────────────────────────────────────────────────────
// Left:  Q/W/E=Ins/Home/PgUp  A/S/D=Del/End/PgDn  F=SelAll  Z/X=Undo/Redo  C/V/B=Copy/Paste/Cut
// Right: U=Bspc  I=Up  O=Del  J=Left  K=Down  L=Right
[WIN_FN] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_INS,  KC_HOME, KC_PGUP, KC_TRNS, KC_TRNS, KC_TRNS, KC_BSPC, KC_UP,   KC_DEL,  KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_DEL,  KC_END,  KC_PGDN, LCTL(KC_A), KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,       KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, LCTL(KC_Z), LCTL(KC_Y), LCTL(KC_C), LCTL(KC_V), LCTL(KC_X), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

};
// clang-format on

// ──────────────────────────────────────────────
// Tap dance — Shift keys (tap/hold=Shift, ×2=Caps Lock, ×3=Caps Word)
// ──────────────────────────────────────────────
typedef struct { uint8_t count; bool registered; } shift_td_state_t;

static shift_td_state_t ltap_state = {0, false};
static shift_td_state_t rtap_state = {0, false};

static void sft_finished(tap_dance_state_t *state, void *user_data, bool is_left) {
    shift_td_state_t *s = is_left ? &ltap_state : &rtap_state;
    s->count = state->count;
    if (s->count == 1) {
        register_code(is_left ? KC_LSFT : KC_RSFT);
        s->registered = true;
    } else if (s->count == 2) {
        tap_code(KC_CAPS);
    } else {
        caps_word_toggle();
    }
}

static void sft_reset(tap_dance_state_t *state, void *user_data, bool is_left) {
    shift_td_state_t *s = is_left ? &ltap_state : &rtap_state;
    if (s->registered) {
        unregister_code(is_left ? KC_LSFT : KC_RSFT);
        s->registered = false;
    }
    s->count = 0;
}

static void lsft_finished(tap_dance_state_t *state, void *user_data) { sft_finished(state, user_data, true); }
static void lsft_reset(tap_dance_state_t *state, void *user_data)    { sft_reset(state, user_data, true); }
static void rsft_finished(tap_dance_state_t *state, void *user_data) { sft_finished(state, user_data, false); }
static void rsft_reset(tap_dance_state_t *state, void *user_data)    { sft_reset(state, user_data, false); }

tap_dance_action_t tap_dance_actions[] = {
    [TD_LSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lsft_finished, lsft_reset),
    [TD_RSFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rsft_finished, rsft_reset),
};

// ──────────────────────────────────────────────
// Custom keycode handling — Swedish chars via Unicode
// ──────────────────────────────────────────────
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // ── Swedish letters (shift XOR caps-lock) ──
        case SE_ARNG: tap_unicode_letter(0x00E5, 0x00C5, record); return false; // å / Å
        case SE_ADIA: tap_unicode_letter(0x00E4, 0x00C4, record); return false; // ä / Ä
        case SE_ODIA: tap_unicode_letter(0x00F6, 0x00D6, record); return false; // ö / Ö

        // ── Swedish symbols (shift only) ──
        case SE_SECT: tap_unicode_symbol(0x00A7, 0x00B0, record); return false; // § / °
        case SE_PLUS: tap_unicode_symbol(0x002B, 0x003F, record); return false; // + / ?
        case SE_ACUT: tap_unicode_symbol(0x0060, 0x00B4, record); return false; // ` / ´
        case SE_APOS: tap_unicode_symbol(0x0027, 0x002A, record); return false; // ' / *
        case SE_LESS: tap_unicode_symbol(0x003C, 0x003E, record); return false; // < / >

        // ── Minus / underscore (caps lock or caps word: - → _) ──
        case KC_MINS:
            if (record->event.pressed) {
                if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
                    register_code16(KC_UNDS);
                } else {
                    register_code(KC_MINS);
                }
            } else {
                unregister_code16(KC_UNDS);
                unregister_code(KC_MINS);
            }
            return false;

        // ── Dot / colon (unshifted = normal KC_DOT, shifted = Unicode colon) ──
        case SE_DOT:
            if (record->event.pressed) {
                uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
                if (mods & MOD_MASK_SHIFT) {
                    register_unicode(0x003A); // :
                } else {
                    register_code(KC_DOT);
                }
            } else {
                unregister_code(KC_DOT);
            }
            return false;

        // ── Comma / semicolon (unshifted = normal KC_COMM, shifted = Unicode ;) ──
        case SE_COMM:
            if (record->event.pressed) {
                uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
                if (mods & MOD_MASK_SHIFT) {
                    register_unicode(0x003B); // ;
                } else {
                    register_code(KC_COMM);
                }
            } else {
                unregister_code(KC_COMM);
            }
            return false;
    }
    return true;
}

// ──────────────────────────────────────────────
// Caps Word — keep alive on Swedish characters and standard keys
// ──────────────────────────────────────────────
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case SE_ARNG: case SE_ADIA: case SE_ODIA:
            return true;  // uppercase handled by tap_unicode_letter via is_caps_word_on()
        case KC_BSPC: case KC_DEL: case KC_MINS: case KC_UNDS:
            return true;  // KC_MINS included to keep Caps Word alive for hyphenated-words
        default:
            return false;
    }
}

// ──────────────────────────────────────────────
// RGB indicators — per-layer colors
// LED indices from iso_encoder.c g_snled27351_leds:
//   Left half (0-42):
//     Row 0: Esc=0  F1=1 F2=2 F3=3 F4=4 F5=5 F6=6
//     Row 1: M1=7   `=8  1=9  2=10 3=11 4=12 5=13 6=14
//     Row 2: M2=15  Tab=16 Q=17 W=18 E=19 R=20 T=21
//     Row 3: M3=22  Caps=23 A=24 S=25 D=26 F=27 G=28
//     Row 4: M4=29  LSft=30 NUBS=31 Z=32 X=33 C=34 V=35 B=36
//     Row 5: M5=37  LCtl=38 LWin=39 LAlt=40 Fn=41 SpcL=42
//   Right half (43-89):
//     Row 0: F7=43 F8=44 F9=45 F10=46 F11=47 F12=48 Ins=49 Del=50
//     Row 1: 7=51  8=52  9=53  0=54   -=55   ==56   Bspc=57 PgUp=58
//     Row 2: Y=59  U=60  I=61  O=62   P=63   [=64   ]=65    PgDn=66
//     Row 3: H=67  J=68  K=69  L=70   ;=71   '=72   NUHS=73 Ent=74 Home=75
//     Row 4: N=76  M=77  ,=78  .=79   /=80   RSft=81 Up=82
//     Row 5: SpcR=83 RWin=84 Fn=85 RCtl=86 Left=87 Down=88 Right=89
// ──────────────────────────────────────────────
// Use the _advanced variant so each split half only writes LEDs within its own
// range [led_min, led_max).  The basic rgb_matrix_indicators_user() writes to
// global indices that share I2C register addresses between halves, causing the
// left-half SNLED27351 to light up wrong keys when right-half indices are set.
// ──────────────────────────────────────────────

// Set a single LED, only if it belongs to this half.
static inline void rgb_set(uint8_t led_min, uint8_t led_max,
                           uint8_t i, uint8_t r, uint8_t g, uint8_t b) {
    if (i >= led_min && i < led_max) rgb_matrix_set_color(i, r, g, b);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    bool is_mac = (default_layer_state & (1UL << MAC_SVORAK)) != 0;

    // ── Layer colors ──
    if (IS_LAYER_ON(MAC_FN) || IS_LAYER_ON(WIN_FN)) {
        // All off, then per-function colors on active keys
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_OFF);
        }
        // Left nav cluster: Q/W/E=Ins/Home/PgUp  A/S/D=Del/End/PgDn → yellow
        rgb_set(led_min, led_max, 17, RGB_YELLOW);  // Q → Ins
        rgb_set(led_min, led_max, 18, RGB_YELLOW);  // W → Home
        rgb_set(led_min, led_max, 19, RGB_YELLOW);  // E → PgUp
        rgb_set(led_min, led_max, 24, RGB_YELLOW);  // A → Del
        rgb_set(led_min, led_max, 25, RGB_YELLOW);  // S → End
        rgb_set(led_min, led_max, 26, RGB_YELLOW);  // D → PgDn
        // Right arrows: IJKL → yellow
        rgb_set(led_min, led_max, 61, RGB_YELLOW);  // I → Up
        rgb_set(led_min, led_max, 68, RGB_YELLOW);  // J → Left
        rgb_set(led_min, led_max, 69, RGB_YELLOW);  // K → Down
        rgb_set(led_min, led_max, 70, RGB_YELLOW);  // L → Right
        // Edit shortcuts: F=SelAll  C=Copy  V=Paste  B=Cut → red
        rgb_set(led_min, led_max, 27, RGB_RED);     // F → Select All
        rgb_set(led_min, led_max, 34, RGB_RED);     // C → Copy
        rgb_set(led_min, led_max, 35, RGB_RED);     // V → Paste
        rgb_set(led_min, led_max, 36, RGB_RED);     // B → Cut
        // Edit keys: U=Bspc  O=Del → red
        rgb_set(led_min, led_max, 60, RGB_RED);     // U → Backspace
        rgb_set(led_min, led_max, 62, RGB_RED);     // O → Delete
        // Undo/Redo: Z/X → green
        rgb_set(led_min, led_max, 32, RGB_GREEN);   // Z → Undo
        rgb_set(led_min, led_max, 33, RGB_GREEN);   // X → Redo
    } else if (IS_LAYER_ON(NUMPAD)) {
        // All off, then yellow on numpad keys only
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_OFF);
        }
        rgb_set(led_min, led_max, 51, RGB_YELLOW);  // 7 → P/
        rgb_set(led_min, led_max, 52, RGB_YELLOW);  // 8 → P*
        rgb_set(led_min, led_max, 53, RGB_YELLOW);  // 9 → P-
        rgb_set(led_min, led_max, 59, RGB_YELLOW);  // Y → P7
        rgb_set(led_min, led_max, 60, RGB_YELLOW);  // U → P8
        rgb_set(led_min, led_max, 61, RGB_YELLOW);  // I → P9
        rgb_set(led_min, led_max, 62, RGB_YELLOW);  // O → P/
        rgb_set(led_min, led_max, 63, RGB_YELLOW);  // P → P*
        rgb_set(led_min, led_max, 67, RGB_YELLOW);  // H → P4
        rgb_set(led_min, led_max, 68, RGB_YELLOW);  // J → P5
        rgb_set(led_min, led_max, 69, RGB_YELLOW);  // K → P6
        rgb_set(led_min, led_max, 70, RGB_YELLOW);  // L → P-
        rgb_set(led_min, led_max, 71, RGB_YELLOW);  // ; → P+
        rgb_set(led_min, led_max, 74, RGB_YELLOW);  // Enter → PEnter
        rgb_set(led_min, led_max, 76, RGB_YELLOW);  // N → P1
        rgb_set(led_min, led_max, 77, RGB_YELLOW);  // M → P2
        rgb_set(led_min, led_max, 78, RGB_YELLOW);  // , → P3
        rgb_set(led_min, led_max, 79, RGB_YELLOW);  // . → P+
        rgb_set(led_min, led_max, 42, RGB_YELLOW);  // Space_L → P.
        rgb_set(led_min, led_max, 83, RGB_YELLOW);  // Space_R → P0
    } else if (IS_LAYER_ON(WIN_SPECIAL) || IS_LAYER_ON(MAC_SPECIAL)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_MAGENTA);
        }
    } else if (IS_LAYER_ON(WIN_QWERTY) || IS_LAYER_ON(MAC_QWERTY)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_CYAN);
        }
    } else {
        // Base Svorak layer
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_BLUE);
        }
    }

    // ── Caps Lock / Caps Word indicator (red on both Shift keys when active) ──
    if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
        rgb_set(led_min, led_max, 30, RGB_RED);  // LSft key
        rgb_set(led_min, led_max, 81, RGB_RED);  // RSft key
    }

    // ── OS indicator on physical arrow keys (always visible) ──
    if (is_mac) {
        rgb_set(led_min, led_max, 82, RGB_MAGENTA);  // Up
        rgb_set(led_min, led_max, 87, RGB_MAGENTA);  // Left
        rgb_set(led_min, led_max, 88, RGB_MAGENTA);  // Down
        rgb_set(led_min, led_max, 89, RGB_MAGENTA);  // Right
    } else {
        rgb_set(led_min, led_max, 82, RGB_RED);     // Up
        rgb_set(led_min, led_max, 87, RGB_RED);     // Left
        rgb_set(led_min, led_max, 88, RGB_RED);     // Down
        rgb_set(led_min, led_max, 89, RGB_RED);     // Right
    }

    return false;
}

// ──────────────────────────────────────────────
// Caps Word split sync — push state to slave half via custom RPC
// ──────────────────────────────────────────────
static void caps_word_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    bool active = *(const bool *)in_data;
    if (active) {
        caps_word_on();
    } else {
        caps_word_off();
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_CAPS_WORD, caps_word_sync_handler);
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) return;
    static bool last_caps_word = false;
    bool current = is_caps_word_on();
    if (current != last_caps_word) {
        last_caps_word = current;
        transaction_rpc_send(USER_SYNC_CAPS_WORD, sizeof(bool), &current);
    }
}

// ──────────────────────────────────────────────
// DIP switch — Mac/Win hardware slider
// The board default maps to layers 0/2. We override to
// target our actual layer indices and set Unicode mode.
// Returning false prevents the board's dip_switch_update_kb
// from running its own default_layer_set.
// ──────────────────────────────────────────────
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        // Clear any stale QWERTY toggles so RGB stays in sync
        layer_off(MAC_QWERTY);
        layer_off(WIN_QWERTY);
        if (active) {
            default_layer_set(1UL << MAC_SVORAK);
            set_unicode_input_mode(UNICODE_MODE_MACOS);
        } else {
            default_layer_set(1UL << WIN_SVORAK);
            set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
        }
    }
    return false;
}

// ──────────────────────────────────────────────
// Encoder (knob) — volume control
// ──────────────────────────────────────────────
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        // Left knob: back & forward (Cmd+]/Cmd+[ on Mac, Alt+Right/Alt+Left on Win)
        bool is_mac = (default_layer_state & (1UL << MAC_SVORAK)) != 0;
        if (is_mac) {
            tap_code16(clockwise ? LGUI(KC_RBRC) : LGUI(KC_LBRC));
        } else {
            tap_code16(clockwise ? LALT(KC_RGHT) : LALT(KC_LEFT));
        }
    } else {
        // Right knob: volume
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    }
    return false;
}
#endif
