#! /usr/bin/python

f = open("ZhuYin-Big5.map", "r", encoding="BIG5-HKSCS");
o = open("output", "w");

for i in range(1, 20):
    line = f.readline()
    if i != 11: continue;
    line = line.replace(' ', '\n')
    o.write(line);
f.close()
o.close()
