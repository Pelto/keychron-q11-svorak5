# Snap Tap (SOCD Cleaner) for QWERTY Gaming Layers

**Date:** 2026-03-23
**Status:** Approved

## Summary

Add Snap Tap (SOCD filtering) to WASD keys on both QWERTY layers using the `getreuer/socd_cleaner` community module. When holding one directional key and tapping the opposing key, the first key is released (last-input-priority). Auto-enables on QWERTY entry, toggleable via left encoder knob, auto-disables on QWERTY exit.

## Requirements

- SOCD filtering on W/S and A/D pairs using `SOCD_CLEANER_LAST` strategy
- Auto-enable when entering MAC_QWERTY or WIN_QWERTY
- Auto-disable when exiting QWERTY layers
- Left encoder knob press toggles SOCD on/off (only in QWERTY modes)
- WASD keys show RED RGB when SOCD is enabled (over cyan base)
- Re-entering QWERTY always resets SOCD to enabled

## Design

### 1. Module Installation (Makefile)

Extend the Makefile to clone `getreuer/qmk-modules` into `.build/qmk_firmware/modules/getreuer/` as a shallow clone. Add as a dependency of the `link` target.

Add `keymap.json` to `SRC_FILES` so it gets symlinked.

```makefile
MODULES_DIR := $(QMK_HOME)/modules/getreuer

$(MODULES_DIR):
	git clone --depth 1 https://github.com/getreuer/qmk-modules.git $@

link: | $(QMK_HOME) $(MODULES_DIR)
```

### 2. Module Declaration (keymap.json)

New file `keymap.json` in project root:

```json
{
    "modules": ["getreuer/socd_cleaner"]
}
```

This modules-only JSON alongside `keymap.c` is a supported QMK pattern (verified in `keyboards/handwired/onekey/keymaps/community_module/`). QMK includes the existing `keymap.c` as `OTHER_KEYMAP_C` when both files are present.

### 3. SOCD Configuration (keymap.c)

Define opposing pairs at file scope:

```c
socd_cleaner_t socd_opposing_pairs[] = {
    {{KC_W, KC_S}, SOCD_CLEANER_LAST},
    {{KC_A, KC_D}, SOCD_CLEANER_LAST},
};
```

The module header is auto-included by QMK's community module system. No manual `#include` or `extern` declaration is needed for `socd_cleaner_enabled` or `socd_cleaner_t`.

### 4. Layer-Based Auto-Enable/Disable (keymap.c)

New `layer_state_set_user` function. Tracks QWERTY transitions (not just state) so that momentary layers (Fn, Numpad, Special) don't reset the user's manual toggle:

```c
layer_state_t layer_state_set_user(layer_state_t state) {
    static bool was_in_qwerty = false;
    bool in_qwerty = IS_LAYER_ON_STATE(state, MAC_QWERTY)
                  || IS_LAYER_ON_STATE(state, WIN_QWERTY);
    if (in_qwerty && !was_in_qwerty) {
        socd_cleaner_enabled = true;   // entering QWERTY: enable
    } else if (!in_qwerty && was_in_qwerty) {
        socd_cleaner_enabled = false;  // leaving QWERTY: disable
    }
    was_in_qwerty = in_qwerty;
    return state;
}
```

### 5. Left Encoder Toggle (keymap layers)

Replace the left encoder knob press key (first key in row 0 of LAYOUT_92_iso) with `SOCDTOG` on:
- MAC_QWERTY layer
- WIN_QWERTY layer

`SOCDTOG` is provided by the module. No-op outside QWERTY since `layer_state_set_user` disables SOCD on non-QWERTY layers.

### 6. WASD RGB Indicators (keymap.c)

In `rgb_matrix_indicators_advanced_user`, within the QWERTY branch (line 517), after setting cyan base, add red WASD when enabled:

```c
if (socd_cleaner_enabled) {
    rgb_set(led_min, led_max, 18, RGB_RED);  // W
    rgb_set(led_min, led_max, 24, RGB_RED);  // A
    rgb_set(led_min, led_max, 25, RGB_RED);  // S
    rgb_set(led_min, led_max, 26, RGB_RED);  // D
}
```

## Files Changed

- `Makefile` — add module clone step, add `keymap.json` to `SRC_FILES`
- `keymap.json` — new file, module declaration
- `keymap.c` — SOCD pairs, `layer_state_set_user`, encoder keycodes, RGB indicators
- `README.md` — document Snap Tap feature
- `docs/layout.json` — update layout documentation
