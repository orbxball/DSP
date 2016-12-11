#! /usr/bin/python

import sys

f = open('lm.cnt', 'r', encoding='big5-HKSCS')
mode = sys.argv[1]
total = 0

if mode == 'sha':
    for i in range(1, 224):
        buf = f.readline()
        if i == 1: continue
        total += int(buf.split('\t')[-1].strip())
        #print(buf.split('\t')[-1].strip())
    print(total)
elif mode == '1gram':
    count = 0
    for line in f:
        if len(line.split(' ')) == 1:
            count += 1
    print(count)
else:
    print("There's no such mode available")
f.close()
