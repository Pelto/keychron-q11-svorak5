# README Layout Diagrams — Design Spec

Replace `docs/layout.json` with full-keyboard ASCII diagrams in README.md. Every key on every layer shown in its physical position, human-readable and agent-parseable.

## Problem

The current README diagrams are partial (missing F-row, bottom modifiers, macro column, right edge, knobs) and positionally ambiguous (symbols shown without anchoring to physical key positions). `docs/layout.json` has the full data but is a separate file that drifts from the README.

## Design decisions

| Decision | Choice |
|---|---|
| Single source of truth | README.md (delete `docs/layout.json`) |
| Transparency handling | Fully resolved (Approach A) — every cell shows what the key produces |
| Split representation | Single wide diagram, 4-space gap between halves |
| Cell width | 5 characters, space-padded |
| Row labels | `ROW 0`–`ROW 5` prefix for agent parsing |
| Modifier labels | OS-native names (`Opt`/`Cmd` on Mac, `Win`/`Alt` on Win) |
| Text vs Unicode | Text only, except `←↑↓→` for arrows |
| Macro key labels | Function names (`Scrn`, `Lock`, `Calc`), not `MC1`–`MC5` |
| Knobs | Omitted from grid, kept in separate Encoders section |
| MOD layers | Full 6-row diagram (no compact treatment) |
| Layer count | All 13 layers get a diagram |

## Diagram format

Each layer is a fenced code block with 6 rows matching the physical keyboard layout. Row labels prefix each line for machine parsing.

```
### Layer N: LAYER_NAME
Activation: <how to activate> · RGB: <color>
Transparent keys resolve to LAYER_NAME (layer N)

ROW 0  │ Esc  │BriDn │BriUp │ MCtl │ LPad │ RGB- │ RGB+ │    │ Prev │ Play │ Next │ Mute │ Vol- │ Vol+ │ Ins  │ Del  │
ROW 1  │ QWrt │  §   │  1   │  2   │  3   │  4   │  5   │    │  6   │  7   │  8   │  9   │  0   │  +   │  `   │ Bspc │PgUp │
ROW 2  │ Scrn │ Tab  │  Å   │  Ä   │  Ö   │  P   │  Y   │    │  F   │  G   │  C   │  R   │  L   │  ,   │  ¨   │PgDn │
ROW 3  │ Lock │ Entr │  A   │  O   │  E   │  U   │  I   │    │  D   │  H   │  T   │  N   │  S   │  -   │  '   │ Entr │Home │
ROW 4  │ Calc │ LSft │ NmPd │  .   │  Q   │  J   │  K   │    │  X   │  B   │  M   │  W   │  V   │  Z   │ RSft │  ↑   │
ROW 5  │ NmPd │ Ctrl │ Opt  │ Cmd  │  Fn  │    SpcL     │    │    SpcR     │ Spec │  Fn  │ Ctrl │  ←   │  ↓   │  →   │
```

### Format rules

- **Cell width**: 6 characters between pipes (labels up to 5 chars, centered with space padding). E.g., `│ Esc  │`, `│  Å   │`.
- **Borders**: `│` between cells.
- **Split gap**: The gap between halves is `│    │` — the closing pipe of the last left-half cell, 4 spaces, then the opening pipe of the first right-half cell (6 characters total replacing what would be a single `│` between adjacent cells).
- **Row labels**: `ROW N  ` prefix (5 chars for `ROW N` + 2 spaces = 7 characters) before the first `│`.
- **Row 5**: Space keys span 2 cell widths each — 13 characters between bounding pipes (2 × 6 content + 1 absorbed interior pipe). The split gap falls between the two space keys. Modifier row has 13 physical keys vs 15–17 on other rows, so row 5 is narrower and does not align column-for-column with rows above.
- **Row 2**: ISO layout — row 2 has one fewer key on the right half than row 3 (no Enter key on this row; Enter electrically lives in row 3). Right half has 8 keys on row 2 vs 9 on rows 1/3.
- **Transparent keys**: Fully resolved to the fallthrough layer's actual output. The header line states which layer they resolve to. All diagrams assume QWERTY is toggled off — when QWERTY is active, transparent keys on overlay layers (FN, SPECIAL, MOD) resolve to the QWERTY layer instead of the Svorak base.
- **`··`**: Indicates a key position that genuinely does nothing (distinct from transparent — `··` means no function on any layer in the stack).

## Transparency chains

All diagrams assume QWERTY is toggled off. When QWERTY is active, QMK's `KC_TRNS` falls through to the highest active layer below, so overlay layers (FN, SPECIAL, MOD) would resolve to the QWERTY layer instead of Svorak.

Each layer's transparent keys resolve to (with QWERTY off):

| Layer | Resolves to |
|---|---|
| 0: MAC_SVORAK | None (base layer) |
| 1: MAC_QWERTY | MAC_SVORAK (layer 0) |
| 2: MAC_SPECIAL | MAC_SVORAK (layer 0) |
| 3: WIN_SVORAK | None (base layer) |
| 4: WIN_QWERTY | WIN_SVORAK (layer 3) |
| 5: WIN_SPECIAL | WIN_SVORAK (layer 3) |
| 6: NUMPAD | MAC_SVORAK (layer 0)* |
| 7: MAC_FN | MAC_SVORAK (layer 0) |
| 8: WIN_FN | WIN_SVORAK (layer 3) |
| 9: MAC_MOD_L | MAC_SVORAK (layer 0) |
| 10: MAC_MOD_R | MAC_SVORAK (layer 0) |
| 11: WIN_MOD_L | WIN_SVORAK (layer 3) |
| 12: WIN_MOD_R | WIN_SVORAK (layer 3) |

*NUMPAD can be activated from either Mac or Win base layers. Diagram resolves against MAC_SVORAK. A note states: "When activated from WIN_SVORAK, non-numpad keys resolve to that layer instead. The numpad keys themselves are identical."

## Abbreviation dictionary

Included as a legend at the top of the `## Full layer diagrams` section.

| Label | Meaning |
|---|---|
| `LSft` / `RSft` | Left/Right Shift (tap dance: x2=Caps Word, x3=Caps Lock) |
| `Bspc` | Backspace |
| `Entr` | Enter |
| `Ctrl` | Control |
| `Opt` | Option (Mac) |
| `Cmd` | Command (Mac) |
| `Win` | Windows key |
| `Alt` | Alt (Win) |
| `Fn` | Function layer hold |
| `Spec` | Special layer activator (MO — occupies Right Cmd position on Mac, Right Alt on Win) |
| `NmPd` | Numpad layer hold |
| `QWrt` | QWERTY toggle |
| `Scrn` | Screenshot (Cmd+Shift+4 Mac / PrScr Win) |
| `Lock` | Lock screen |
| `Calc` | Calculator |
| `PgUp` / `PgDn` | Page Up / Page Down |
| `Home` | Home |
| `Ins` | Insert |
| `Del` | Delete |
| `SpcL` / `SpcR` | Space Left / Space Right (tap=Space, hold=MOD layer) |
| `End` | End |
| `F1`–`F12` | Function keys (shown as-is, no abbreviation needed) |
| `PEnt` | Numpad Enter |
| `PDot` | Numpad decimal point |
| `SelA` | Select All |
| `Undo` / `Redo` | Undo / Redo |
| `Copy` / `Pste` / `Cut` | Copy / Paste / Cut |
| `WrdL` / `WrdR` | Word Left / Word Right |
| `BriDn` / `BriUp` | Brightness Down / Up |
| `MCtl` | Mission Control |
| `LPad` | Launchpad |
| `RGB-` / `RGB+` | RGB brightness down / up |
| `Prev` / `Play` / `Next` | Media previous / play / next |
| `Vol-` / `Vol+` | Volume down / up |
| `Mute` | Mute |
| `PrSc` | Print Screen (Win) |
| `CpLk` | Caps Lock (Win QWERTY only) |
| `··` | No function on this layer |

## README structure

The README is reorganized as follows:

1. **Keep as-is**: Title, Flashing, Prerequisites, Build and flash, Other commands
2. **Keep as-is**: Layer overview table (activation/RGB summary)
3. **Dismantle `## Layout` section**: The current `## Layout` heading contains both partial diagrams and narrative subsections (Key behaviors, SPECIAL layer, NUMPAD layer, FN layer, Thumb modifier layers, QWERTY layer). Action:
   - **Delete**: The `## Layout` heading itself, the `docs/layout.json` reference, the "Svorak base layer" partial diagram, and all other partial inline diagrams (SPECIAL, NUMPAD, FN)
   - **Promote to top-level sections**: Key behaviors, Thumb modifier layers, QWERTY layer — these narrative subsections become `##`-level sections in their current document order
4. **Keep as-is**: Macro keys, Encoders sections
5. **Add**: `## Full layer diagrams` section at the end, containing:
   - Legend (abbreviation table)
   - Note about QWERTY transparency caveat
   - All 13 layer diagrams in order (layers 0–12)

## File changes

| File | Action |
|---|---|
| `README.md` | Remove `## Layout` section; add `## Full layer diagrams` appendix |
| `docs/layout.json` | Delete |
| `CLAUDE.md` | Remove `docs/layout.json` from "always update" instruction |
| `MEMORY.md` | Update to remove `docs/layout.json` reference |

## Out of scope

- Knob/encoder documentation changes (keep current Encoders section)
- Changes to keymap.c
- Changes to narrative sections (Key behaviors, Macro keys, etc.) beyond removing stale references to layout.json
