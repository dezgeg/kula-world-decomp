import re
import sys

def addr2off(addr):
    return addr - 0x11000 + 0x800

func_or_addr = sys.argv[1]
found = False
for line in open('function_addrs.txt').readlines():
    m = re.match('^(.*) = (.*);.*', line)
    if found:
        next_func_addr = int(m[2], 16)
        next_func = m[1]
        break
    if m[1] == func_or_addr:
        func_addr = int(m[2], 16)
        found = True

if found:
    print("%s: 0x%05x" % (func_or_addr, addr2off(func_addr)))
    print("%s: 0x%05x" % (next_func, addr2off(next_func_addr)))
else:
    print("0x%05x" % (addr2off(int(func_or_addr, 16))))
