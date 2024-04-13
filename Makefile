# No built-in rules
.SUFFIXES:
SHELL = bash -e -o pipefail

C_FILES := $(wildcard src/*.c) $(wildcard src/*/*.c)
S_FILES := $(wildcard asm/*.s) $(wildcard asm/*/*.s) $(wildcard asm/data/*/*.s)
O_FILES := $(patsubst %.c,build/%.o,$(C_FILES)) $(patsubst %.s,build/%.o,$(S_FILES))

.PHONY: check
check: build/SCES_010.00
	sha256sum --check - <<<"28c8f46d28f971038ccd68135de4d5dfc0f7a1c00285748aea98276a5d37bf75  build/SCES_010.00"

# This rule causes the $(wildcard) for C_FILES etc. to be re-evaluated if splat split needs re-running
Makefile: build/kula_world.ld
	touch Makefile

build/kula_world.ld: kula_world.yaml $(wildcard *_addrs.txt)
	rm -rf src/nonmatched asm/ build/
	source venv/bin/activate && splat split kula_world.yaml

build/SCES_010.00: build/main.elf
	mipsel-linux-gnu-objcopy -O binary $< $@

build/main.elf: $(O_FILES)
	mipsel-linux-gnu-ld -nostdlib --no-check-sections -o $@ -T build/kula_world.ld -T build/undefined_syms_auto.txt -Map build/symbols.map

build/%.o: %.s
	@mkdir -p $$(dirname $@)
	python3 ~/maspsx/maspsx.py $< | mipsel-linux-gnu-as -EL -32 -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -Iinclude/ - -o $@

build/%.o: %.c psyq
	@mkdir -p $$(dirname $@)
	 psyq/cpppsx -I include/ -isystem psyq/INCLUDE/ $< | psyq/cc1psx -G128 -w -O3 -fpeephole -ffunction-cse -fpcc-struct-return -fcommon -fgnu-linker -funsigned-char -fverbose-asm -mgas -msoft-float -quiet | python3 ~/maspsx/maspsx.py --macro-inc --aspsx-version=2.56 -G128 | mipsel-linux-gnu-as -EL -32 -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -G0 -Iinclude/ - -o $@

psyq:
	mkdir -p psyq
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-40.tar.gz' | tar -C psyq -xz
	rm -rf psyq/INCLUDE psyq/LIB
	curl -L 'https://github.com/dezgeg/psyq-sdk-builder/releases/latest/download/psyq-42.tar.gz' | tar -C psyq -xz
