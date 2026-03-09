# Keychron Q11 ISO Nordic — Dvorak Firmware

Custom QMK keymap for the Keychron Q11 with Dvorak layout, Swedish characters (å, ä, ö), and a Unicode-based special character layer triggered by holding Option/Alt.

## Setup

### macOS

```bash
brew install qmk/qmk/qmk
qmk setup -H .build/qmk_firmware
```

### WSL / Ubuntu

```bash
sudo apt update && sudo apt install -y git python3-pip
python3 -m pip install --user qmk
# add ~/.local/bin to PATH if qmk command is not found:
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc && source ~/.bashrc
qmk setup -H .build/qmk_firmware
```

## Build

| Command          | Description                                          |
|------------------|------------------------------------------------------|
| `make`           | Build firmware                                       |
| `make flash`     | Build and flash to keyboard                          |
| `make compiledb` | Generate `compile_commands.json` for editor LSP      |
| `make lint`      | Run QMK lint on the keymap                           |
| `make clean`     | Remove `.build/` directory and generated files       |

Override the keyboard target if your Q11 variant differs:

```bash
make KEYBOARD=keychron/q11/iso_encoder
```

## Layout

The Mac/Win hardware switch selects the default layer at boot. Both use the same Dvorak alpha layout — only the bottom-row modifiers and Unicode input mode differ.

### Base layer (Dvorak)

```
,------,------,------,------,------,------,------.         ,------,------,------,------,------,------,------.
| Esc  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |         |  F7  |  F8  |  F9  | F10  | F11  | F12  |  Vol |
|------+------+------+------+------+------+------|         |------+------+------+------+------+------+------|
|  `   |   1  |   2  |   3  |   4  |   5  |   6  |         |   7  |   8  |   9  |   0  |   -  |   =  | Bksp |
|------+------+------+------+------+------+------'         '------+------+------+------+------+------+------|
| Tab  |   Å  |   Ä  |   Ö  |   P  |   Y  |                      |   F  |   G  |   C  |   R  |   L  |  , " |
|------+------+------+------+------+------|                      |------+------+------+------+------+------|
| Caps |   A  |   O  |   E  |   U  |   I  |                      |   D  |   H  |   T  |   N  |   S  | - Ent|
|------+------+------+------+------+------+------.         ,------+------+------+------+------+------+------|
|Shift |  <>  |   Q  |   J  |   K  |   X  |      |         |      |   B  |   M  |   W  |   V  |   Z  |Shift |
`------+------+------+------+------+------+------'         `------+------+------+------+------+------+------'
         Ctrl   Opt*  Cmd         Space                             Cmd   Opt   App   Ctrl    FN

         Ctrl   Win   Alt*        Space                             Alt   Win   App   Ctrl    FN    ← Win mode
```

`Opt*` / `Alt*` is the custom key that activates the special character layer.
Rotary knob controls volume (press = mute).

### Opt/Alt + key (special characters)

Hold `Opt*` and press a key to output a Unicode character instead of the base letter.
Empty cells (`·`) pass through as normal Alt+key.

```
,------,------,------,------,------,------,------.         ,------,------,------,------,------,------,------.
|      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |
|------+------+------+------+------+------+------|         |------+------+------+------+------+------+------|
|      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |
|------+------+------+------+------+------+------'         '------+------+------+------+------+------+------|
|      |   {  |   }  |   [  |   ]  |   $  |                      |   "  |   ?  |   &  |   <  |   >  |  ␣ ~ |
|------+------+------+------+------+------|                      |------+------+------+------+------+------|
|      |   ;  |   /  |   (  |   )  |   |  |                      |   ·  |   .  |   #  |   "  |   ·  |  · · |
|------+------+------+------+------+------+------.         ,------+------+------+------+------+------+------|
|      |   ·  |   :  |   =  |   @  |   !  |      |         |      |   \  |   %  |   .  |   ·  |   ·  |      |
`------+------+------+------+------+------+------'         `------+------+------+------+------+------+------'
```

Bypass: if Cmd, Ctrl, or Shift are held alongside Opt, special characters are
disabled and Opt behaves as normal Alt (so shortcuts like Cmd+Opt+key still work).

### GAME layer

Toggle with **FN + H**. Overrides alpha keys with standard QWERTY for gaming.
`Opt*` becomes plain Alt (no special characters). RGB turns solid red as indicator.

### FN layer (hold FN)

| Key        | Function                                       |
|------------|------------------------------------------------|
| F1 – F2    | Brightness down / up                           |
| F3 – F4    | Mission Control / Launchpad                    |
| F5 – F6    | RGB brightness down / up                       |
| F7 – F9    | Previous / Play-Pause / Next                   |
| F10 – F12  | Mute / Volume down / up                        |
| Knob       | RGB toggle                                     |
| A – F row  | RGB controls (mode, brightness, hue, sat, spd) |
| H          | Toggle GAME layer                              |
| Right Opt  | Cycle Unicode mode (macOS ↔ WinCompose)        |

## OS prerequisites

### macOS

System Settings → Keyboard → Input Sources → add **Unicode Hex Input** and select it as the active input source.

### Windows

Install [WinCompose](https://github.com/samhocevar/wincompose) and leave it running. The firmware uses it automatically to send Unicode characters.
