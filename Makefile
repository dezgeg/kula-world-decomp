# No built-in rules
.SUFFIXES:
SHELL = bash -e -o pipefail

C_FILES := $(wildcard src/*.c) $(wildcard src/*/*.c)
S_FILES := $(wildcard asm/*.s) $(wildcard asm/*/*.s) $(wildcard asm/data/*/*.s)
O_FILES := $(patsubst %.c,build/%.o,$(C_FILES)) $(patsubst %.s,build/%.o,$(S_FILES))

build/SCES_010.00: build/main.elf
	mipsel-linux-gnu-objcopy -O binary $< $@

build/main.elf: $(O_FILES)
	mipsel-linux-gnu-ld -nostdlib --no-check-sections -o $@ -T kula_world.ld -T undefined_syms_auto.txt -Map build/symbols.map

build/%.o: %.s
	@mkdir -p $$(dirname $@)
	python3 ~/maspsx/maspsx.py $< | mipsel-linux-gnu-as -EL -32 -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -Iinclude/ - -o $@

build/%.o: %.c
	@mkdir -p $$(dirname $@)
	~/wibo/build/wibo ~/PSYQ40/GNU/CPPPSX.EXE -I include/ -isystem ~/psyq42/INCLUDE/ $< | TMPDIR=/tmp ~/wibo/build/wibo ~/PSYQ40/GNU/CC1PSX.EXE -G4 -w -O3 -funsigned-char -fverbose-asm -msoft-float -fcommon -quiet | python3 ~/maspsx/maspsx.py --macro-inc --aspsx-version=2.56 | mipsel-linux-gnu-as -EL -32 -march=r3000 -mtune=r3000 -msoft-float -no-pad-sections -Iinclude/ - -o $@

