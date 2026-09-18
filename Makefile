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
#
# CI (.github/workflows/build.yml) builds by running `make compile`, so the
# pins below are the single source of truth for local *and* CI firmware.

KEYBOARD ?= keychron/q11/iso_encoder
KEYMAP   ?= custom
QMK_HOME ?= $(CURDIR)/.build/qmk_firmware

# Pinned sources. Bump these to upgrade — `make` re-syncs an existing checkout
# on the next build, no `make clean` required.
QMK_VERSION ?= 0.34.4
# getreuer/qmk-modules publishes no tags, so pin the commit.
MODULES_VERSION ?= 8c55ac1c5d547d1ff324ae2834b26f2075222c97

# The getreuer/qmk-modules repo structure mirrors QMK's expected module path:
#   repo/socd_cleaner/ -> modules/getreuer/socd_cleaner/
MODULES_DIR ?= $(QMK_HOME)/modules/getreuer

# Point the qmk CLI (compiledb, lint) at this pinned tree rather than whatever
# checkout `qmk config` names globally.
export QMK_HOME

# Parallel build jobs. getconf works on both macOS and Linux.
JOBS ?= $(shell getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)

# Homebrew arm-none-eabi-gcc 15.x is missing newlib headers.
# Use gcc@8 and ensure binutils is in PATH.
ARM_GCC8   := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-gcc@8/*/bin)
ARM_BINUTILS := $(wildcard /opt/homebrew/Cellar/arm-none-eabi-binutils/*/bin)
ifneq ($(ARM_GCC8),)
  export PATH := $(ARM_GCC8):$(ARM_BINUTILS):$(PATH)
endif

KEYMAP_DIR := $(QMK_HOME)/keyboards/$(KEYBOARD)/keymaps/$(KEYMAP)
SRC_FILES  := keymap.c keymap.json config.h rules.mk

.PHONY: all compile flash compiledb lint link sync clean

all: compile

# Clone QMK firmware pinned to $(QMK_VERSION) (shallow — submodules are fetched
# on demand by QMK's own make).
$(QMK_HOME):
	git clone --depth 1 --branch $(QMK_VERSION) https://github.com/qmk/qmk_firmware.git $@

# Clone community modules (getreuer — includes socd_cleaner and other modules,
# only socd_cleaner is used). Cloned with full history (~1.5 MB) so `sync` can
# check out any pinned commit without a second fetch. Order-only dependency on
# $(QMK_HOME): this lives inside it, and cloning it first would leave the
# directory non-empty and break QMK's clone.
$(MODULES_DIR): | $(QMK_HOME)
	git clone https://github.com/getreuer/qmk-modules.git $@
	git -C $@ checkout --detach --quiet $(MODULES_VERSION)

# Re-sync existing checkouts to the pins above. Without this a version bump is
# silently ignored until `make clean` — which is exactly how a local build
# drifts away from what CI produces.
sync: | $(QMK_HOME) $(MODULES_DIR)
	@if [ "$$(git -C $(QMK_HOME) describe --tags --exact-match 2>/dev/null)" != "$(QMK_VERSION)" ]; then \
	  echo "==> qmk_firmware -> $(QMK_VERSION)"; \
	  git -C $(QMK_HOME) fetch --depth 1 origin tag $(QMK_VERSION); \
	  git -C $(QMK_HOME) checkout --detach --quiet $(QMK_VERSION); \
	  rm -rf $(QMK_HOME)/.build; \
	fi
	@if [ "$$(git -C $(MODULES_DIR) rev-parse HEAD)" != "$(MODULES_VERSION)" ]; then \
	  echo "==> qmk-modules -> $(MODULES_VERSION)"; \
	  git -C $(MODULES_DIR) fetch --quiet origin; \
	  git -C $(MODULES_DIR) checkout --detach --quiet $(MODULES_VERSION); \
	  rm -rf $(QMK_HOME)/.build; \
	fi

# Symlink keymap files into the QMK tree
link: sync
	@mkdir -p $(KEYMAP_DIR)
	@$(foreach f,$(SRC_FILES),ln -snf $(CURDIR)/$(f) $(KEYMAP_DIR)/$(f);)

compile: link
	$(MAKE) -C $(QMK_HOME) $(KEYBOARD):$(KEYMAP) -j $(JOBS)

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
