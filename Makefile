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

# Homebrew arm-none-eabi-gcc 15.x is missing newlib headers.
# Use gcc@8 and ensure binutils is in PATH.
ARM_GCC8   := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-gcc@8/*/bin)
ARM_BINUTILS := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-binutils/*/bin)
ifneq ($(ARM_GCC8),)
  export PATH := $(ARM_GCC8):$(ARM_BINUTILS):$(PATH)
endif

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

# Generate compile_commands.json for clangd / LSP
compiledb: link
	qmk compile -kb $(KEYBOARD) -km $(KEYMAP) --compiledb
	ln -snf $(QMK_HOME)/compile_commands.json compile_commands.json

clean:
	rm -rf .build compile_commands.json
