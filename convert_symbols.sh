tail -n +3 | grep -v '^"switchdata' | grep -v '^"switch' | grep -v '^"case' | grep '"Function"' | tr '",' '  ' | awk '{ printf("%s = 0x%s; // type:func\n", $1, $2); }'
