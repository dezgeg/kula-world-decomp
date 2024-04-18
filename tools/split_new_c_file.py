import re
import sys

func = sys.argv[1]
end = False
for line in open('function_addrs.txt').readlines():
    m = re.match('^(.*) = (.*);.*', line)
    if end:
        next_func_addr = int(m[2], 16)
        break
    if m[1] == func:
        func_addr = int(m[2], 16)
        end = True

print(hex(func_addr - 0x11000 + 0x800))
print(hex(next_func_addr - 0x11000 + 0x800))