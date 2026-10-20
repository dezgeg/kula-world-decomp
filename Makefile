# Common stuff to each version

.SUFFIXES: # No built-in rules
.NOTINTERMEDIATE: # Don't delete intermediates
SHELL := bash -e -o pipefail
CROSS := mipsel-linux-gnu

VERSIONS ?= eu-SCES_010.00 us-SLUS_007.24

SYS_DEFINES := -D__GNUC__=2 -D__OPTIMIZE__ -lang-c -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D__CHAR_UNSIGNED__ -D_LANGUAGE_C -DLANGUAGE_C
C_FILES := $(wildcard src/*.c) $(wildcard src/*/*.c)

.PHONY: all check $(foreach ver,$(VERSIONS),check-$(ver))
all: check
check: $(foreach ver,$(VERSIONS),check-$(ver))

psyq:
	mkdir -p psyq/4.2 psyq/4.4
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-40.tar.gz' | tar -C psyq -xz --exclude={INCLUDE,LIB,COFF,ELF}
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-42.tar.gz' | tar -C psyq/4.2 -xz ./INCLUDE ./LIB ./ELF
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-44.tar.gz' | tar -C psyq/4.4 -xz ./INCLUDE ./LIB ./ELF

venv:
	virtualenv venv
	source venv/bin/activate && pip3 install -U splat64[mips] pycparser pynacl toml Levenshtein

# This rule causes the $(wildcard) for C_FILES etc. to be re-evaluated if splat split needs re-running
Makefile: $(foreach ver,$(VERSIONS),build/$(ver)/kula_world.ld)
	touch Makefile

# Version-specific rules template

define VERSION_RULES
ver := $(1)
include config/$(1)/conf.mk

TARGET_NAME_$(1) := $$(TARGET_NAME)
DISC_BIN_$(1) := $$(DISC_BIN)
UNPADDED_SIZE_$(1) := $$(UNPADDED_SIZE)
PADDED_SIZE_$(1) := $$(PADDED_SIZE)
SHA256_$(1) := $$(SHA256)
PSYQ_VER_$(1) := $$(PSYQ_VER)
DEFINES_$(1) := $$(DEFINES)

S_FILES_$(1) := $$(wildcard asm/$(1)/*.s) $$(wildcard asm/$(1)/*/*.s) $$(wildcard asm/$(1)/data/*/*.s)
O_FILES_$(1) := $$(patsubst %.c,build/$(1)/%.o,$$(C_FILES)) $$(patsubst %.s,build/$(1)/%.o,$$(S_FILES_$(1)))

check-$(1): build/$(1)/$$(TARGET_NAME_$(1))
	sha256sum --check - <<<"$$(SHA256_$(1))  $$<"

build/$(1)/subdirs:
	mkdir -p $$(sort $$(dir $$(O_FILES_$(1)))) build/$(1)/subdirs

build/$(1)/kula_world.ld: config/$(1)/splat.yaml psyq venv $$(wildcard config/$(1)/*_addrs.txt)
	rm -rf src/nonmatched asm/$(1)/ build/$(1)/
	mkdir -p build/$(1)
	for f in $$$$(cd psyq/$$(PSYQ_VER_$(1))/ELF; echo *.A); do mkdir -p build/$(1)/$$$$f; ar x psyq/$$(PSYQ_VER_$(1))/ELF/$$$$f --output=build/$(1)/$$$$f; done
	dd if=$$(DISC_BIN_$(1)) of=build/$(1)/truncated.bin count=1 bs=$$(UNPADDED_SIZE_$(1))
	source venv/bin/activate && splat split config/$(1)/splat.yaml

build/$(1)/$$(TARGET_NAME_$(1)): build/$(1)/main.elf
	$$(CROSS)-objcopy --pad-to=$$(PADDED_SIZE_$(1)) -O binary $$< $$@

build/$(1)/main.elf: $$(O_FILES_$(1))
	$$(CROSS)-ld -nostdlib --no-check-sections -o $$@ -T build/$(1)/kula_world.ld -T build/$(1)/undefined_syms_auto.txt -Map build/$(1)/symbols.map

build/$(1)/%.i: %.c psyq build/$(1)/subdirs
	psyq/cpppsx -isystem psyq/$$(PSYQ_VER_$(1))/INCLUDE/ -I include/ -undef $$(SYS_DEFINES) $$(DEFINES_$(1)) $$< -o $$@

build/$(1)/%.s: build/$(1)/%.i psyq build/$(1)/subdirs
	psyq/cc1psx -gcoff -G128 -w -O3 -quiet $$< -o $$@

build/$(1)/%.o: build/$(1)/%.s build/$(1)/subdirs
	python3 tools/maspsx/maspsx.py --aspsx-version=2.56 -G128 --run-assembler --gnu-as-path=$$(CROSS)-as --no-pad-sections --use-comm-section --use-comm-for-lcomm --macro-inc -Iasm/$(1)/ -o $$@ < $$<
	$$(CROSS)-objcopy --set-section-alignment .bss=4 --set-section-alignment .data=4 $$@

build/$(1)/%.o: %.s build/$(1)/subdirs
	$$(CROSS)-as -G128 -no-pad-sections -Iasm/$(1)/ -o $$@ $$<
	$$(CROSS)-objcopy --set-section-alignment .bss=4 --set-section-alignment .data=4 $$@
endef

$(foreach ver,$(VERSIONS),$(eval $(call VERSION_RULES,$(ver))))
