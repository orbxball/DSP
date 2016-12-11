#! /usr/bin/python
import sys

infile = sys.argv[1]
outfile = sys.argv[2]
f = open(infile, 'r', encoding='BIG5-HKSCS')
out = open(outfile, 'w', encoding='BIG5-HKSCS')
dic = {}

for line in f:
    s_line = line.split(' ')
    ChineseChar = s_line[0]
    JuYin = s_line[1].split('/')
    for begin in JuYin:
        if begin[0] in dic:
            dic[begin[0]].append(ChineseChar)
        else:
            dic[begin[0]] = [ChineseChar]
    dic[ChineseChar] = [ChineseChar]
f.close()

for key in sorted(dic.keys()):
    out.write(key + ' ' + ' '.join(dic[key]) + '\n')
out.close()
