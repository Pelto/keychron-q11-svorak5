# Design: Caps Lock → Enter + Shift Tap Dance + Caps Word

**Date:** 2026-03-12
**Status:** Approved

---

## Overview

Two related features:

1. **Caps Lock → Enter** — The physical Caps Lock key is remapped to Enter. Plain substitution, no tap dance.
2. **Shift Tap Dance + Caps Word** — Both Shift keys become tap dance keys with three behaviors: tap/hold = Shift, double-tap = Caps Lock toggle, triple-tap = Caps Word toggle. RGB lights both Shift keys red when either state is active.

---

## Feature 1: Caps Lock → Enter

### Change

Replace `KC_CAPS` with `KC_ENT` in the base Svorak layers:

- `keymap.c` MAC_SVORAK row 3 (line 125)
- `keymap.c` WIN_SVORAK row 3 (line 156)

QWERTY overlays use `KC_TRNS` on that position and inherit automatically.

### RGB

Remove the existing caps lock indicator on LED 23 (the physical Caps Lock key). That key is now Enter; showing red on Enter when caps is active would be misleading. The Shift keys (Section 2) become the sole caps indicators.

---

## Feature 2: Shift Tap Dance + Caps Word

### Build system changes

`rules.mk`:
```mk
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
```

No `config.h` changes. Default `TAPPING_TERM` of 200 ms is used initially and can be tuned post-flash.

### Tap dance behavior

| Input | Behavior |
|-------|----------|
| Tap once (any duration) or hold | Shift modifier |
| Double-tap | Toggle Caps Lock |
| Triple-tap | Toggle Caps Word |

Both Left and Right Shift get independent tap dance entries (`TD_LSFT`, `TD_RSFT`) that share the same logic but track state separately.

### Implementation pattern

Enum added to `keymap.c`:
```c
enum tap_dance_codes {
    TD_LSFT,
    TD_RSFT,
};
```

State tracking struct (one instance per key):
```c
typedef struct { uint8_t count; bool pressed; } shift_td_state_t;
```

`finished` callback logic:
- `count == 1` → `register_code(KC_LSFT / KC_RSFT)`
- `count == 2` → `tap_code(KC_CAPS)`
- `count >= 3` → `caps_word_toggle()`

`reset` callback logic:
- If `count == 1` was active → `unregister_code(KC_LSFT / KC_RSFT)`
- Otherwise nothing (Caps Lock and Caps Word are toggle actions)

### Keymap changes

Replace in MAC_SVORAK and WIN_SVORAK row 4:
- `KC_LSFT` → `TD(TD_LSFT)`
- `KC_RSFT` → `TD(TD_RSFT)`

QWERTY overlays use `KC_TRNS` and inherit automatically.

### Caps Word + Swedish letter fix

`tap_unicode_letter()` currently checks only `host_keyboard_led_state().caps_lock`. Caps Word does not set the host caps lock state, so Swedish letters (å/ä/ö) would remain lowercase during Caps Word. Fix:

```c
bool caps = host_keyboard_led_state().caps_lock || is_caps_word_on();
```

New `caps_word_press_user()` callback to keep Caps Word alive on Swedish characters and standard keep-alive keys:

```c
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        case SE_ARNG: case SE_ADIA: case SE_ODIA:
            return true;  // uppercase handled by tap_unicode_letter via is_caps_word_on()
        case KC_BSPC: case KC_DEL: case KC_MINS: case KC_UNDS:
            return true;
        default:
            return false;
    }
}
```

### RGB indicators

In `rgb_matrix_indicators_advanced_user()`:

- **Remove**: caps lock indicator on LED 23
- **Add**: when `host_keyboard_led_state().caps_lock || is_caps_word_on()`, set LEDs 30 (LSft) and 81 (RSft) to red

Both Caps Lock and Caps Word share the same visual indicator. The behavioral difference (manual exit vs auto-exit on non-alphanumeric) needs no separate color.

---

## Documentation changes

### README.md

1. Svorak base layer diagram: `⇪` → `↵`
2. Add "Key behaviors" section describing:
   - Physical Caps Lock key → Enter
   - Shift tap dance (tap/hold = Shift, ×2 = Caps Lock, ×3 = Caps Word)
   - RGB: both Shift keys red when Caps Lock or Caps Word is active

### docs/layout.json

1. MAC_SVORAK row3: `"KC_CAPS": "KC_ENT"`
2. WIN_SVORAK row3: `"KC_CAPS": "KC_ENT"`
3. MAC_SVORAK row4: `"KC_LSFT": "TD(TD_LSFT)"`, `"KC_RSFT": "TD(TD_RSFT)"`
4. WIN_SVORAK row4: `"KC_LSFT": "TD(TD_LSFT)"`, `"KC_RSFT": "TD(TD_RSFT)"`
5. Layers with `"_______"` on those positions unchanged (inherit via `KC_TRNS`)

---

## Files changed

| File | Change |
|------|--------|
| `rules.mk` | Add `TAP_DANCE_ENABLE`, `CAPS_WORD_ENABLE` |
| `keymap.c` | Caps remap, tap dance enum + callbacks + actions, keymap replacements, `tap_unicode_letter` fix, `caps_word_press_user`, RGB update |
| `README.md` | Layer diagram + key behaviors section |
| `docs/layout.json` | Caps and Shift key values in base SVORAK layers |
