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
# The getreuer/qmk-modules repo structure mirrors QMK's expected module path:
#   repo/socd_cleaner/ -> modules/getreuer/socd_cleaner/
MODULES_DIR ?= $(QMK_HOME)/modules/getreuer

# Homebrew arm-none-eabi-gcc 15.x is missing newlib headers.
# Use gcc@8 and ensure binutils is in PATH.
ARM_GCC8   := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-gcc@8/*/bin)
ARM_BINUTILS := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-binutils/*/bin)
ifneq ($(ARM_GCC8),)
  export PATH := $(ARM_GCC8):$(ARM_BINUTILS):$(PATH)
endif

KEYMAP_DIR := $(QMK_HOME)/keyboards/$(KEYBOARD)/keymaps/$(KEYMAP)
SRC_FILES  := keymap.c keymap.json config.h rules.mk

.PHONY: all compile flash compiledb lint link clean

all: compile

# Clone QMK firmware (shallow — submodules fetched on demand by QMK make)
$(QMK_HOME):
	git clone --depth 1 https://github.com/qmk/qmk_firmware.git $@

# Clone community modules (getreuer — includes socd_cleaner and other modules, only socd_cleaner is used)
$(MODULES_DIR):
	git clone --depth 1 https://github.com/getreuer/qmk-modules.git $@

# Symlink keymap files into the QMK tree
link: | $(QMK_HOME) $(MODULES_DIR)
	@mkdir -p $(KEYMAP_DIR)
	@$(foreach f,$(SRC_FILES),ln -snf $(CURDIR)/$(f) $(KEYMAP_DIR)/$(f);)

compile: link
	$(MAKE) -C $(QMK_HOME) $(KEYBOARD):$(KEYMAP)

flash: link
	$(MAKE) -C $(QMK_HOME) $(KEYBOARD):$(KEYMAP):flash

lint: link
	cd $(QMK_HOME) && qmk lint -kb $(KEYBOARD) -km $(KEYMAP)

# Generate compile_commands.json for clangd / LSP
# QMK's --compiledb omits the user's keymap.c because the build uses a
# generated wrapper; append an entry cloned from default_keyboard.c so
# clangd can resolve community module symbols in keymap.c.
compiledb: link
	qmk compile -kb $(KEYBOARD) -km $(KEYMAP) --compiledb
	ln -snf $(QMK_HOME)/compile_commands.json compile_commands.json
	@# Clone a neighbouring entry's flags and force-include
	@# community_modules_introspection.h so clangd can resolve symbols
	@# (e.g. socd_cleaner_t) that are only in scope because QMK's build
	@# wraps keymap.c inside keymap_introspection.c.
	@python3 -c "import json, pathlib; \
db_path = pathlib.Path('$(QMK_HOME)/compile_commands.json'); \
user_keymap = str(pathlib.Path.cwd() / 'keymap.c'); \
db = json.loads(db_path.read_text()); \
template = next(e for e in db if e['file'].endswith('src/default_keyboard.c')); \
db = [e for e in db if e['file'] != user_keymap]; \
args = list(template['arguments']) + ['-include', 'community_modules_introspection.h']; \
db.append({'arguments': args, 'directory': template['directory'], 'file': user_keymap}); \
db_path.write_text(json.dumps(db, indent=2))"

clean:
	rm -rf .build compile_commands.json