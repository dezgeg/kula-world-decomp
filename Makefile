.SUFFIXES: # No built-in rules
.SECONDARY: # Don't delete intermediates
SHELL := bash -e -o pipefail

C_FILES := $(wildcard src/*.c) $(wildcard src/*/*.c)
S_FILES := $(wildcard asm/*.s) $(wildcard asm/*/*.s) $(wildcard asm/data/*/*.s)
O_FILES := $(patsubst %.c,build/%.o,$(C_FILES)) $(patsubst %.s,build/%.o,$(S_FILES))

.PHONY: check
check: build/SCES_010.00
	sha256sum --check - <<<"28c8f46d28f971038ccd68135de4d5dfc0f7a1c00285748aea98276a5d37bf75  build/SCES_010.00"

# This rule causes the $(wildcard) for C_FILES etc. to be re-evaluated if splat split needs re-running
Makefile: build/kula_world.ld
	touch Makefile

build/subdirs:
	mkdir -p $(sort $(dir $(O_FILES))) build/subdirs

build/kula_world.ld: kula_world.yaml venv $(wildcard *_addrs.txt)
	rm -rf src/nonmatched asm/ build/
	source venv/bin/activate && splat split kula_world.yaml

build/SCES_010.00: build/main.elf
	mipsel-linux-gnu-objcopy -O binary $< $@

build/main.elf: $(O_FILES)
	mipsel-linux-gnu-ld -nostdlib --no-check-sections -o $@ -T build/kula_world.ld -T build/undefined_syms_auto.txt -T hacks.txt -Map build/symbols.map

build/%.i: %.c psyq build/subdirs
	psyq/cpppsx -isystem psyq/INCLUDE/ -I include/ -undef -D__GNUC__=2 -D__OPTIMIZE__ -lang-c -Dmips -D__mips__ -D__mips -Dpsx -D__psx__ -D__psx -D_PSYQ -D__EXTENSIONS__ -D_MIPSEL -D__CHAR_UNSIGNED__ -D_LANGUAGE_C -DLANGUAGE_C $< -o $@

build/%.s: build/%.i psyq build/subdirs
	psyq/cc1psx -G128 -w -O3 -quiet $< -o $@

build/%.o: build/%.s build/subdirs
	 python3 tools/maspsx/maspsx.py --macro-inc --aspsx-version=2.56 -G128 --run-assembler -no-pad-sections --use-comm-section --use-comm-for-lcomm -Iinclude/ -o $@ < $<

# TODO: figure out how to avoid this duplicate rule
build/%.o: %.s build/subdirs
	 python3 tools/maspsx/maspsx.py --aspsx-version=2.56 -G128 --run-assembler -no-pad-sections -Iinclude/ -o $@ < $<

psyq:
	mkdir -p psyq
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-40.tar.gz' | tar -C psyq -xz
	rm -rf psyq/INCLUDE psyq/LIB
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-42.tar.gz' | tar -C psyq -xz

venv:
	virtualenv venv
	source venv/bin/activate && pip3 install -U splat64[mips] pycparser pynacl toml Levenshtein
