# Project instructions

## Swedish keycodes reference

The Mac layers use native Swedish keyboard combos (Option+key, Shift+key).
Reference for keycode mappings:
https://github.com/qmk/qmk_firmware/blob/9fd72b352b8687bb8846c9977010bc137d91a5fc/quantum/keymap_extras/keymap_swedish_mac_ansi.h

Note: this is the ANSI version. The keyboard is ISO, so adjust for:
- `KC_GRV` = `§` (not `<` like ANSI)
- `KC_NUBS` = `<` (ISO-only key between left shift and Z)
- `KC_NUHS` = `'` (ISO-only key between quote and Enter)

Known discrepancy: the QMK file says `@` = `A(KC_NUHS)` but on macOS
Sequoia with Swedish input, `@` is actually `A(KC_2)`. Always verify
against the OS keyboard viewer when in doubt.

The Win layers use native Swedish keyboard combos (AltGr+key, Shift+key) — same approach as Mac but using RALT instead of LALT for AltGr, and some different key positions (e.g. `{` = RALT(KC_7), `\` = RALT(KC_MINS)).

## Always update on feature changes

When modifying layers, keycodes, or behaviors, always update:
- `README.md` — user-facing docs
- `docs/layout.json` — machine-readable layout documentation
