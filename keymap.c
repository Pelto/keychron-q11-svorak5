// Keychron Q11 ISO Nordic — Custom keymap
//
// Layer stack (higher index = higher priority):
//   0: MAC_SVORAK   — Swedish Dvorak (default when Mac switch)
//   1: MAC_QWERTY   — QWERTY overlay (toggle via MC_1)
//   2: MAC_SPECIAL   — Programming symbols via Unicode (hold RCmd)
//   3: WIN_SVORAK   — Swedish Dvorak (default when Win switch)
//   4: WIN_QWERTY   — QWERTY overlay (toggle via MC_1)
//   5: WIN_SPECIAL   — Programming symbols via Unicode (hold RAlt)
//   6: NUMPAD        — Right-hand numpad (hold MC_5)
//   7: FN            — Arrow keys on IJKL (hold FN)
//
// Mac setup required:
//   System Preferences → Keyboard → Input Sources → add "Unicode Hex Input"
//
// Windows setup required:
//   Install WinCompose: https://github.com/samhocevar/wincompose

#include QMK_KEYBOARD_H

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
    _FN,
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
// Unicode codepoints for SPECIAL layers
// ──────────────────────────────────────────────
#define U_LCBR UC(0x007B)  // {
#define U_RCBR UC(0x007D)  // }
#define U_LBRC UC(0x005B)  // [
#define U_RBRC UC(0x005D)  // ]
#define U_DLR  UC(0x0024)  // $
#define U_DQUO UC(0x0022)  // "
#define U_QUES UC(0x003F)  // ?
#define U_AMPR UC(0x0026)  // &
#define U_LABK UC(0x003C)  // <
#define U_RABK UC(0x003E)  // >
#define U_SEMI UC(0x003B)  // ;
#define U_SLSH UC(0x002F)  // /
#define U_LPRN UC(0x0028)  // (
#define U_RPRN UC(0x0029)  // )
#define U_PIPE UC(0x007C)  // |
#define U_CIRC UC(0x005E)  // ^
#define U_HASH UC(0x0023)  // #
#define U_TILD UC(0x007E)  // ~
#define U_DOT  UC(0x002E)  // .
#define U_AT   UC(0x0040)  // @
#define U_BSLS UC(0x005C)  // backslash
#define U_PERC UC(0x0025)  // %
#define U_COLN UC(0x003A)  // :
#define U_EQL  UC(0x003D)  // =
#define U_EXLM UC(0x0021)  // !

// ──────────────────────────────────────────────
// Helpers — send Unicode with shift / caps-lock awareness
// ──────────────────────────────────────────────
// Letters: Shift XOR CapsLock selects upper-case.
static void tap_unicode_letter(uint32_t lower, uint32_t upper, keyrecord_t *record) {
    if (record->event.pressed) {
        uint8_t mods    = get_mods() | get_weak_mods() | get_oneshot_mods();
        bool    shifted = (mods & MOD_MASK_SHIFT) != 0;
        bool    caps    = host_keyboard_led_state().caps_lock;
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
    TG(MAC_QWERTY), SE_SECT, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,           KC_PGUP,
    LSG(KC_4),      KC_TAB,  SE_ARNG, SE_ADIA, SE_ODIA, KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    SE_COMM, KC_RBRC,                    KC_PGDN,
    LCG(KC_Q),      KC_CAPS, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_MINS, SE_APOS, KC_ENT,            KC_HOME,
    KC_CALC,        KC_LSFT, SE_LESS, SE_DOT,  KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,             KC_RSFT, KC_UP,
    MO(NUMPAD),     KC_LCTL, KC_LOPT, KC_LCMD, MO(_FN),          KC_SPC,                    KC_SPC,           MO(MAC_SPECIAL), MO(_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
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
// Programming symbols via Unicode hex input (macOS)
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
    LGUI(KC_L),     KC_CAPS, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH, KC_NUHS, KC_ENT,            KC_HOME,
    KC_CALC,        KC_LSFT, KC_NUBS, KC_DOT,  KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,             KC_RSFT, KC_UP,
    MO(NUMPAD),     KC_LCTL, KC_LWIN, KC_LALT, MO(_FN),          KC_SPC,                    KC_SPC,           MO(WIN_SPECIAL), MO(_FN), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
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
    KC_TRNS, KC_TRNS, U_LCBR,  U_RCBR,  U_LBRC,  U_RBRC,  U_DLR,   U_DQUO,  U_QUES,  U_AMPR,  U_LABK,  U_RABK,  KC_TRNS, U_TILD,                   KC_TRNS,
    KC_TRNS, KC_TRNS, U_SEMI,  U_SLSH,  U_LPRN,  U_RPRN,  U_PIPE,  KC_TRNS, U_CIRC,  U_HASH,  U_DQUO,  U_TILD,  KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, U_COLN,  U_EQL,   U_AT,    U_EXLM,  U_BSLS,  U_PERC,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
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

// ── FN LAYER ────────────────────────────────────────────────────────────────────
// Right: IJKL arrow keys.  Left: WER/SDF navigation.
[_FN] = LAYOUT_92_iso(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_INS,  KC_HOME, KC_PGUP, KC_TRNS, KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL,  KC_END,  KC_PGDN, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                    KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
),

};
// clang-format on

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
        // ── Backtick / acute — custom handling ──
        // ` (unshifted) exists on the US layout → send KC_GRV directly.
        // ´ (shifted) needs Unicode; clear mods first to avoid Option+B
        // being interpreted as "word backward" by macOS.
        case SE_ACUT:
            if (record->event.pressed) {
                uint8_t mods    = get_mods() | get_weak_mods() | get_oneshot_mods();
                bool    shifted = (mods & MOD_MASK_SHIFT) != 0;
                uint8_t saved   = get_mods();
                clear_mods();
                clear_weak_mods();
                if (shifted) {
                    register_unicode(0x00B4);  // ´
                } else {
                    tap_code(KC_GRV);  // `
                }
                set_mods(saved);
            }
            return false;
        case SE_APOS: tap_unicode_symbol(0x0027, 0x002A, record); return false; // ' / *
        case SE_LESS: tap_unicode_symbol(0x003C, 0x003E, record); return false; // < / >

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
    if (IS_LAYER_ON(_FN)) {
        // Base color with yellow highlights on active keys
        for (uint8_t i = led_min; i < led_max; i++) {
            if (IS_LAYER_ON(WIN_QWERTY) || IS_LAYER_ON(MAC_QWERTY)) {
                rgb_matrix_set_color(i, RGB_CYAN);
            } else {
                rgb_matrix_set_color(i, RGB_BLUE);
            }
        }
        // Left: WER/SDF navigation
        rgb_set(led_min, led_max, 18, RGB_YELLOW);  // W → Ins
        rgb_set(led_min, led_max, 19, RGB_YELLOW);  // E → Home
        rgb_set(led_min, led_max, 20, RGB_YELLOW);  // R → PgUp
        rgb_set(led_min, led_max, 25, RGB_YELLOW);  // S → Del
        rgb_set(led_min, led_max, 26, RGB_YELLOW);  // D → End
        rgb_set(led_min, led_max, 27, RGB_YELLOW);  // F → PgDn
        // Right: IJKL arrows
        rgb_set(led_min, led_max, 61, RGB_YELLOW);  // I → Up
        rgb_set(led_min, led_max, 68, RGB_YELLOW);  // J → Left
        rgb_set(led_min, led_max, 69, RGB_YELLOW);  // K → Down
        rgb_set(led_min, led_max, 70, RGB_YELLOW);  // L → Right
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

    // ── Caps Lock indicator (red when active) ──
    if (host_keyboard_led_state().caps_lock) {
        rgb_set(led_min, led_max, 23, RGB_RED);  // CapsLock key
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
