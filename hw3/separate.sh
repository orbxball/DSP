#! /bin/bash

SP_TD=s_testdata

[ -d ${SP_TD} ] || mkdir -p ${SP_TD}

for i in $(seq 1 10); do
	perl separator_big5.pl testdata/${i}.txt > ${SP_TD}/${i}.txt
done
