# Keychron Q11 ISO Dvorak — Build System
#
# Usage:
#   make              build firmware
#   make flash        build and flash to keyboard
#   make compiledb    generate compile_commands.json for editor LSP
#   make clean        remove build artifacts
#
# First run will clone qmk_firmware (~2 min). Subsequent builds are fast.
# Override KEYBOARD if your Q11 variant differs.

KEYBOARD ?= keychron/q11/iso_encoder
KEYMAP   ?= custom
QMK_HOME ?= .build/qmk_firmware

KEYMAP_DIR := $(QMK_HOME)/keyboards/$(KEYBOARD)/keymaps/$(KEYMAP)
SRC_FILES  := keymap.c config.h rules.mk

.PHONY: all compile flash compiledb lint link clean

all: compile

# Clone QMK firmware (shallow — submodules fetched on demand by QMK make)
$(QMK_HOME):
	git clone --depth 1 https://github.com/qmk/qmk_firmware.git $@

# Symlink keymap files into the QMK tree
link: | $(QMK_HOME)
	@mkdir -p $(KEYMAP_DIR)
	@$(foreach f,$(SRC_FILES),ln -snf $(CURDIR)/$(f) $(KEYMAP_DIR)/$(f);)

compile: link
	$(MAKE) -C $(QMK_HOME) $(KEYBOARD):$(KEYMAP)

flash: link
	$(MAKE) -C $(QMK_HOME) $(KEYBOARD):$(KEYMAP):flash

lint: link
	cd $(QMK_HOME) && qmk lint -kb $(KEYBOARD) -km $(KEYMAP)

# Generate compile_commands.json for clangd / LSP (requires: pip install qmk)
compiledb: link
	cd $(QMK_HOME) && qmk generate-compilation-database -kb $(KEYBOARD) -km $(KEYMAP)
	ln -snf $(QMK_HOME)/compile_commands.json compile_commands.json

clean:
	rm -rf .build compile_commands.json
