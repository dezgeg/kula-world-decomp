# Common stuff to each version

.SUFFIXES: # No built-in rules
.SECONDARY: # Don't delete intermediates
SHELL := bash -e -o pipefail
CROSS := mipsel-linux-gnu

.PHONY: all
all: check

psyq:
	mkdir -p psyq
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-40.tar.gz' | tar -C psyq -xz --exclude={INCLUDE,LIB,COFF,ELF}
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-42.tar.gz' | tar -C psyq -xz ./INCLUDE ./LIB ./ELF

venv:
	virtualenv venv
	source venv/bin/activate && pip3 install -U splat64[mips] pycparser pynacl toml Levenshtein

# Version-specific stuff

UNPADDED_SIZE := $(shell printf %d 0x934b0)
PADDED_SIZE := 0x93800

C_FILES := $(wildcard src/*.c) $(wildcard src/*/*.c)
S_FILES := $(wildcard asm/*.s) $(wildcard asm/*/*.s) $(wildcard asm/data/*/*.s)
O_FILES := $(patsubst %.c,build/eu-SCES_010.00/%.o,$(C_FILES)) $(patsubst %.s,build/eu-SCES_010.00/%.o,$(S_FILES))

.PHONY: check
check: build/eu-SCES_010.00/SCES_010.00
	sha256sum --check - <<<"28c8f46d28f971038ccd68135de4d5dfc0f7a1c00285748aea98276a5d37bf75  build/eu-SCES_010.00/SCES_010.00"

# This rule causes the $(wildcard) for C_FILES etc. to be re-evaluated if splat split needs re-running
Makefile: build/eu-SCES_010.00/kula_world.ld
	touch Makefile

build/eu-SCES_010.00/subdirs:
	mkdir -p $(sort $(dir $(O_FILES))) build/eu-SCES_010.00/subdirs

build/eu-SCES_010.00/kula_world.ld: kula_world.yaml psyq venv $(wildcard *_addrs.txt)
	rm -rf src/nonmatched asm/ build/eu-SCES_010.00/
	mkdir -p build
	for f in $$(cd psyq/ELF; echo *.A); do mkdir -p build/eu-SCES_010.00/$$f; ar x psyq/ELF/$$f --output=build/eu-SCES_010.00/$$f; done
	dd if=discs/eu/SCES_010.00 of=build/eu-SCES_010.00/truncated.bin count=1 bs=$(UNPADDED_SIZE)
	source venv/bin/activate && splat split kula_world.yaml

build/eu-SCES_010.00/SCES_010.00: build/eu-SCES_010.00/main.elf
	$(CROSS)-objcopy --pad-to=$(PADDED_SIZE) -O binary $< $@

build/eu-SCES_010.00/main.elf: $(O_FILES)
	$(CROSS)-ld -nostdlib --no-check-sections -o $@ -T build/eu-SCES_010.00/kula_world.ld -T build/eu-SCES_010.00/undefined_syms_auto.txt -Map build/eu-SCES_010.00/symbols.map

build/eu-SCES_010.00/%.i: %.c psyq build/eu-SCES_010.00/subdirs
	psyq/cpppsx -isystem psyq/INCLUDE/ -I include/ -undef -D__GNUC__=2 -D__OPTIMIZE__ -lang-c -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D__CHAR_UNSIGNED__ -D_LANGUAGE_C -DLANGUAGE_C $< -o $@

build/eu-SCES_010.00/%.s: build/eu-SCES_010.00/%.i psyq build/eu-SCES_010.00/subdirs
	psyq/cc1psx -gcoff -G128 -w -O3 -quiet $< -o $@

build/eu-SCES_010.00/%.o: build/eu-SCES_010.00/%.s build/eu-SCES_010.00/subdirs
	 python3 tools/maspsx/maspsx.py --aspsx-version=2.56 -G128 --run-assembler --gnu-as-path=$(CROSS)-as --no-pad-sections --use-comm-section --use-comm-for-lcomm --macro-inc -Iasm/ -o $@ < $<
	$(CROSS)-objcopy --set-section-alignment .bss=4 --set-section-alignment .data=4 $@

build/eu-SCES_010.00/%.o: %.s build/eu-SCES_010.00/subdirs
	$(CROSS)-as -G128 -no-pad-sections -Iasm/ -o $@ $<
	$(CROSS)-objcopy --set-section-alignment .bss=4 --set-section-alignment .data=4 $@
