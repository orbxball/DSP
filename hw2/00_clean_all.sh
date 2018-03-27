#!/bin/bash

if [ -d MFCC/ ]; then
	echo "rm MFCC/ -r"
	rm MFCC/ -r
	echo "mkdir MFCC"
	mkdir MFCC
fi

if [ -d hmm/ ]; then
	echo "rm hmm/ -r"
	rm hmm/ -r
	echo "mkdir hmm"
	mkdir hmm
fi

if [ -d result/ ]; then
	echo "rm result/ -r"
	rm result/ -r
	echo "mkdir result"
	mkdir result
fi

cd bin; make clean; cd ..
