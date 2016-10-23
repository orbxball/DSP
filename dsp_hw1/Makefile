CC = gcc
CFLAG = -std=c99

target = train test
var = 1

all: $(target)

train: train.c
	$(CC) $(CFLAG) $< -o $@

test: test.c
	$(CC) $(CFLAG) $< -o $@

diff: diff.c
	$(CC) $(CFLAG) $< -o $@

run: runtrain runtest

runtrain: train
	./train $(var) model_init.txt seq_model_01.txt model_01.txt
	./train $(var) model_init.txt seq_model_02.txt model_02.txt
	./train $(var) model_init.txt seq_model_03.txt model_03.txt
	./train $(var) model_init.txt seq_model_04.txt model_04.txt
	./train $(var) model_init.txt seq_model_05.txt model_05.txt

runtest: test
	./test modellist.txt testing_data1.txt result1.txt

clean:
	rm -f train model_01.txt model_02.txt model_03.txt model_04.txt model_05.txt

cleann:
	rm -f train model_01.txt model_02.txt model_03.txt model_04.txt model_05.txt test result1.txt result2.txt

cleannn:
	rm -f train model_01.txt model_02.txt model_03.txt model_04.txt model_05.txt test result1.txt result2.txt diff acc.txt
