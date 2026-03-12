# Keychron Q11 ISO Nordic — Svorak Firmware

Custom QMK keymap for the Keychron Q11 (ISO encoder) with Swedish Dvorak (Svorak) layout and a Unicode special character layer for programming symbols.

## Flashing

### Prerequisites

**macOS:**

```bash
brew install qmk/qmk/qmk arm-none-eabi-gcc@8 arm-none-eabi-binutils
```

> The build uses `arm-none-eabi-gcc@8` automatically. Newer GCC versions (15.x) have broken newlib headers.

**macOS Unicode setup:**

System Settings → Keyboard → Input Sources → add **Unicode Hex Input** and select it as the active input source.

**Windows Unicode setup:**

Install [WinCompose](https://github.com/samhocevar/wincompose) and leave it running.

### Build and flash

The Q11 is a split keyboard — each half has its own MCU. Flash both halves with the same firmware:

```bash
make              # build firmware
make flash        # build and flash (repeat for each half)
```

**Flash the left half (primary/USB side):**

1. Unplug USB from the left half
2. Hold **Escape** while plugging USB back in — this enters DFU mode
3. Run `make flash`

**Flash the right half:**

1. Unplug USB from the left half, plug it into the right half
2. Press the **reset button** on the bottom of the right half (small pinhole — use a paperclip) to enter DFU mode
3. Run `make flash`
4. Unplug, reconnect normally (USB to left half, TRRS cable between halves)

### Other commands

| Command          | Description                                     |
|------------------|-------------------------------------------------|
| `make compiledb` | Generate `compile_commands.json` for editor LSP |
| `make lint`      | Run QMK lint on the keymap                      |
| `make clean`     | Remove `.build/` directory and generated files  |

## Layer overview

The Mac/Win hardware slider selects the base layer at boot and sets the Unicode input mode automatically.

| Layer | Name | Activation | RGB |
|-------|------|-----------|-----|
| 0 | MAC_SVORAK | Default (Mac switch) | Blue |
| 1 | MAC_QWERTY | Toggle MC_1 | Cyan |
| 2 | MAC_SPECIAL | Hold Right Cmd | Purple |
| 3 | WIN_SVORAK | Default (Win switch) | Blue |
| 4 | WIN_QWERTY | Toggle MC_1 | Cyan |
| 5 | WIN_SPECIAL | Hold Right Alt | Purple |
| 6 | NUMPAD | Hold MC_5 | Yellow (numpad keys only, rest off) |
| 7 | FN | Hold FN | Base color + yellow on active keys |

The physical arrow keys in the bottom-right corner always show the OS mode:
**purple** = Mac, **red** = Windows. This indicator is visible on every layer.

## Layout

See `docs/layout.json` for the full machine-readable layout definition.

### Svorak base layer (Swedish ISO)

```
 §  1  2  3  4  5  6  7  8  9  0  +  `
 ⇥  Å  Ä  Ö  P  Y  F  G  C  R  L  ,  `
 ↵  A  O  E  U  I  D  H  T  N  S  -  '
 ⇧  <  .  Q  J  K  X  B  M  W  V  Z  ⇧
```

### Key behaviors

| Key | Behavior |
|-----|----------|
| Physical Caps Lock | Enter (remapped in base layer) |
| Shift (tap or hold) | Shift modifier |
| Shift × 2 | Toggle Caps Lock |
| Shift × 3 | Toggle Caps Word |
| RGB | Both Shift keys red when Caps Lock or Caps Word is active |

### SPECIAL layer (hold Right Cmd / Right Alt)

Programming symbols sent via Unicode:

```
       {  }  [  ]  $  "  ?  &  <  >     ~
       ;  /  (  )  |     ^  #  "  ~
          :  =  @  !  \  %
```

### NUMPAD layer (hold MC_5)

```
               /  *  -
               7  8  9  /  *
               4  5  6  -  +     Enter
               1  2  3  +
         .  0
```

### FN layer (hold FN)

Left hand navigation, right hand arrows:

```
      Ins Home PgUp              ↑
      Del End  PgDn         ←    ↓    →
```

### QWERTY layer (toggle MC_1)

Standard QWERTY overlay for gaming. Toggle again to return to Svorak.

## Macro keys

| Key | Mac | Windows |
|-----|-----|---------|
| MC_1 | Toggle QWERTY | Toggle QWERTY |
| MC_2 | Screenshot (Cmd+Shift+4) | Print Screen |
| MC_3 | Lock screen (Ctrl+Cmd+Q) | Lock screen (Win+L) |
| MC_4 | Calculator | Calculator |
| MC_5 | Hold for NUMPAD | Hold for NUMPAD |

## Encoders

| Knob | Rotate | Press |
|------|--------|-------|
| Left | Back / Forward (Cmd+] / Cmd+[ on Mac, Alt+Right / Alt+Left on Win) | Mute |
| Right | Volume down / up | Mute |
