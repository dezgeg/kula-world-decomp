#!/bin/sh
psyq/cc1psx -G128 -w -O3 -quiet $1 -o $3.s
python3 tools/maspsx/maspsx.py --aspsx-version=2.56 -G128 --run-assembler --gnu-as-path=mipsel-linux-gnu-as --no-pad-sections --use-comm-section --use-comm-for-lcomm --macro-inc -Iinclude/ -o $3 < $3.s
