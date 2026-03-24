# Snap Tap (SOCD Cleaner) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add SOCD filtering (Snap Tap) to WASD keys on QWERTY gaming layers using the getreuer/socd_cleaner community module.

**Architecture:** Community module provides SOCD logic. We declare opposing pairs in keymap.c, auto-enable/disable via layer state transitions, toggle via left encoder press in QWERTY layers, and show WASD as red LEDs when active.

**Tech Stack:** QMK firmware, C, getreuer/socd_cleaner community module

**Spec:** `docs/superpowers/specs/2026-03-23-snap-tap-socd-design.md`

---

## File Map

- **Create:** `keymap.json` — module declaration (modules-only, no layers)
- **Modify:** `Makefile` — add module clone step, add keymap.json to SRC_FILES
- **Modify:** `keymap.c` — SOCD pairs, layer_state_set_user, SOCDTOG on QWERTY layers, RGB indicators
- **Modify:** `README.md` — document Snap Tap feature
- **Modify:** `docs/layout.json` — update layout documentation

---

### Task 1: Makefile — add community module clone step

**Files:**
- Modify: `Makefile`

This task adds the module download as a build dependency and includes keymap.json in the symlinked files. All edits use content-based matching (line numbers will shift as edits are applied).

- [ ] **Step 1: Add MODULES_DIR variable after QMK_HOME**

Find the line `QMK_HOME ?= .build/qmk_firmware` and add after it:

```makefile
# The getreuer/qmk-modules repo structure mirrors QMK's expected module path:
#   repo/socd_cleaner/ -> modules/getreuer/socd_cleaner/
MODULES_DIR ?= $(QMK_HOME)/modules/getreuer
```

- [ ] **Step 2: Add keymap.json to SRC_FILES**

Find:

```makefile
SRC_FILES  := keymap.c config.h rules.mk
```

Replace with:

```makefile
SRC_FILES  := keymap.c keymap.json config.h rules.mk
```

- [ ] **Step 3: Add module clone target**

Find the QMK clone target block:

```makefile
$(QMK_HOME):
	git clone --depth 1 https://github.com/qmk/qmk_firmware.git $@
```

Add after it:

```makefile

# Clone community modules (getreuer — includes socd_cleaner and other modules, only socd_cleaner is used)
$(MODULES_DIR):
	git clone --depth 1 https://github.com/getreuer/qmk-modules.git $@
```

- [ ] **Step 4: Add MODULES_DIR as dependency of link**

Find:

```makefile
link: | $(QMK_HOME)
```

Replace with:

```makefile
link: | $(QMK_HOME) $(MODULES_DIR)
```

- [ ] **Step 5: Verify Makefile syntax**

Run: `make -n compile 2>&1 | head -20`

Expected: no Makefile syntax errors. Should show the clone commands and compile command in dry-run output. If `.build/` doesn't exist yet, the clone commands will appear. If it does, just the compile command.

---

### Task 2: Create keymap.json

**Files:**
- Create: `keymap.json`

- [ ] **Step 1: Create keymap.json**

Create file `keymap.json` in the project root with this exact content:

```json
{
    "modules": ["getreuer/socd_cleaner"]
}
```

This is a modules-only JSON. QMK supports this alongside keymap.c — verified in `keyboards/handwired/onekey/keymaps/community_module/`.

---

### Task 3: Add SOCD pairs, init, and layer_state_set_user to keymap.c

**Files:**
- Modify: `keymap.c`

The module auto-includes its header via QMK's community module system. No manual `#include` or `extern` declaration is needed for `socd_cleaner_t`, `socd_cleaner_enabled`, or `SOCD_CLEANER_LAST`.

- [ ] **Step 1: Add SOCD opposing pairs after custom keycodes enum**

Find the closing `};` of `enum custom_keycodes` (the one containing `SE_ACUT = SAFE_RANGE`) and add after it:

```c

// ──────────────────────────────────────────────
// SOCD Cleaner — Snap Tap for WASD gaming keys
// ──────────────────────────────────────────────
socd_cleaner_t socd_opposing_pairs[] = {
    {{KC_W, KC_S}, SOCD_CLEANER_LAST},
    {{KC_A, KC_D}, SOCD_CLEANER_LAST},
};
```

- [ ] **Step 2: Disable SOCD at boot**

The module defaults `socd_cleaner_enabled = true`. Since the keyboard boots into Svorak (not QWERTY), we need to disable it at init. Find the existing `keyboard_post_init_user` function:

```c
void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_CAPS_WORD, caps_word_sync_handler);
}
```

Replace with:

```c
void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_CAPS_WORD, caps_word_sync_handler);
    socd_cleaner_enabled = false;
}
```

- [ ] **Step 3: Add layer_state_set_user function**

Find the closing `}` of the `dip_switch_update_user` function (the one containing `layer_off(MAC_QWERTY)` and `default_layer_set`). Add after it:

```c

// ──────────────────────────────────────────────
// Layer state — auto-enable SOCD in QWERTY modes
// ──────────────────────────────────────────────
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

---

### Task 4: Map SOCDTOG to left encoder on QWERTY layers

**Files:**
- Modify: `keymap.c`

The left encoder knob press is the first key in row 0 of each LAYOUT_92_iso. Use the layer comment + `LAYOUT_92_iso(` as context to find the unique replacement site.

- [ ] **Step 1: Update MAC_QWERTY left encoder**

Find the MAC_QWERTY layer's first row (the line immediately after `[MAC_QWERTY] = LAYOUT_92_iso(`). Change:

```c
[MAC_QWERTY] = LAYOUT_92_iso(
    KC_TRNS,        KC_TRNS, KC_TRNS,
```

to:

```c
[MAC_QWERTY] = LAYOUT_92_iso(
    SOCDTOG,        KC_TRNS, KC_TRNS,
```

(Only replace the first `KC_TRNS` on that line — the rest of the line stays unchanged.)

- [ ] **Step 2: Update WIN_QWERTY left encoder**

Find the WIN_QWERTY layer's first row (the line immediately after `[WIN_QWERTY] = LAYOUT_92_iso(`). Change:

```c
[WIN_QWERTY] = LAYOUT_92_iso(
    KC_TRNS,        KC_TRNS, KC_TRNS,
```

to:

```c
[WIN_QWERTY] = LAYOUT_92_iso(
    SOCDTOG,        KC_TRNS, KC_TRNS,
```

(Only replace the first `KC_TRNS` on that line — the rest of the line stays unchanged.)

---

### Task 5: Add WASD RGB indicators when SOCD is enabled

**Files:**
- Modify: `keymap.c:517-520`

- [ ] **Step 1: Add red WASD indicators in QWERTY RGB branch**

Replace the QWERTY RGB block (lines 517-520):

```c
    } else if (IS_LAYER_ON(WIN_QWERTY) || IS_LAYER_ON(MAC_QWERTY)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_CYAN);
        }
```

with:

```c
    } else if (IS_LAYER_ON(WIN_QWERTY) || IS_LAYER_ON(MAC_QWERTY)) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_CYAN);
        }
        if (socd_cleaner_enabled) {
            rgb_set(led_min, led_max, 18, RGB_RED);  // W
            rgb_set(led_min, led_max, 24, RGB_RED);  // A
            rgb_set(led_min, led_max, 25, RGB_RED);  // S
            rgb_set(led_min, led_max, 26, RGB_RED);  // D
        }
```

---

### Task 6: Build verification

- [ ] **Step 1: Compile firmware**

Run: `make compile`

Expected: successful build with no errors. The community module should be found and compiled in. Look for any warnings related to `socd_cleaner` or `SOCDTOG` in the output.

- [ ] **Step 2: Fix any build errors**

If there are errors, check:
1. `SOCDTOG` not found → the module may use a different keycode name, check `.build/qmk_firmware/modules/getreuer/socd_cleaner/socd_cleaner.h` for the actual keycode definition
2. `socd_cleaner_t` or `socd_cleaner_enabled` not found → the module header wasn't auto-included. Check that `keymap.json` was symlinked correctly into the keymap dir: `ls -la .build/qmk_firmware/keyboards/keychron/q11/iso_encoder/keymaps/custom/keymap.json`. If the symlink is missing, check that `keymap.json` is in `SRC_FILES` in the Makefile

---

### Task 7: Update README.md and docs/layout.json

**Files:**
- Modify: `README.md`
- Modify: `docs/layout.json`

- [ ] **Step 1: Read current README.md to find the right section**

Read `README.md` to identify where QWERTY layer features are documented and the "Full layer diagrams" section.

- [ ] **Step 2: Add Snap Tap documentation to README.md**

Add a section documenting:
- Snap Tap (SOCD filtering) is available on QWERTY layers
- Auto-enables when entering QWERTY mode
- Left encoder knob press toggles it on/off
- WASD keys glow red when active
- Auto-disables when leaving QWERTY

- [ ] **Step 3: Update docs/layout.json**

Update the MAC_QWERTY and WIN_QWERTY layer entries to reflect the SOCDTOG key on the left encoder position.

- [ ] **Step 4: Final compile check**

Run: `make compile`

Expected: clean build, no errors or warnings.
