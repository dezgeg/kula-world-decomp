import re
import sys
import yaml

FMT = '0x%05x, %s, %s'

def addr2off(addr):
    return addr - 0x11000 + 0x800

func_or_addr = sys.argv[1]
target_obj_file = None if len(sys.argv) < 3 else sys.argv[2]
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

if not found:
    print("0x%05x" % (addr2off(int(func_or_addr, 16))))
else:
    func_off = addr2off(func_addr)
    next_func_off = addr2off(next_func_addr)
    print("%s: 0x%05x" % (func_or_addr, func_off))
    print("%s: 0x%05x" % (next_func, next_func_off))

    yml = yaml.load(open('kula_world.yaml').read(), Loader=yaml.Loader)
    subsegments = yml['segments'][1]['subsegments']
    for i, (offset, section, name) in enumerate(subsegments):
        if offset >= func_off:
            break
    else:
        print('not found')
    print(FMT % tuple(subsegments[i-1]))
    print(FMT % tuple(subsegments[i]))
    print(FMT % tuple(subsegments[i+1]))

    if target_obj_file:
        if func_off == subsegments[i][0] and next_func_off == subsegments[i+1][0]:
            print("Target function is only one in the nonmatched c file")
            if subsegments[i-1][2] == target_obj_file:
                print("Merge into previous (deletion is enough here)")
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":DELETEME:'") % tuple(subsegments[i]))
                print("sed -i kula_world.yaml -e '/DELETEME/d'")
            elif subsegments[i+1][2] == target_obj_file:
                print("Merge into next (pull back startpoint of next)")
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":d'") % tuple(subsegments[i]))
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":" + FMT + ":'") % (tuple(subsegments[i+1]) + tuple([func_off, 'c', subsegments[i+1][2]])))
            else:
                print("Just rename")
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":" + FMT + ":'") % (tuple(subsegments[i]) + tuple([subsegments[i][0], 'c', target_obj_file])))
        elif func_off == subsegments[i][0]:
            print("Target function is the first one in the nonmatched c file")
            if subsegments[i-1][2] == target_obj_file:
                print("...and we want to move it to the previous file")
                print("Extend startpoint of matching object")
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":" + FMT + ":'") % (tuple(subsegments[i]) + tuple([next_func_off, 'c', subsegments[i][2]])))
            else:
                print("...and we want to move it to split it off to its own file")
                print(("sed -i kula_world.yaml -e 's:" + FMT + ":" + FMT + ":'") % (tuple(subsegments[i]) + tuple([next_func_off, 'c', subsegments[i][2]])))
