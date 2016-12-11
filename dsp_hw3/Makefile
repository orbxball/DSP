# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /tmp2/b03502040/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run

TEST1DIR=result1
TESTDATA=testdata
ANSDIR=result2

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

diff:
	@for i in $(shell seq 1 10); do \
		diff $(TEST1DIR)/$${i}.txt $(ANSDIR)/$${i}.txt; \
	done;

run:
	@#TODO How to run your code toward different txt? 
	[ -d $(ANSDIR) ] || mkdir -p $(ANSDIR);
	@for i in $(shell seq 1 10) ; do \
            echo "Running $$i.txt"; \
            ./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
        done;
map:
	@#TODO How to map?
	echo "Mapping!"
	@#./mapping $(FROM) $(TO)
	@#matlab mapping.m $(FROM) $(TO)
	python mapping.py $(FROM) $(TO)
	@#sh mapping.sh $(FROM) $(TO)
	@#perl mapping.pl Big5-ZhuYin.map ZhuYin-Big5.map

build_lm:
	perl separator_big5.pl corpus.txt > corpus_seg.txt
	$(SRIPATH)/bin/i686-m64/ngram-count -text corpus_seg.txt -write lm.cnt -order 2
	$(SRIPATH)/bin/i686-m64/ngram-count -read lm.cnt -lm bigram.lm -unk -order 2

test1: 
	[ -d $(TEST1DIR) ] || mkdir -p $(TEST1DIR);
	for i in $(shell seq 1 10); do \
		$(SRIPATH)/bin/i686-m64/disambig -text testdata/$${i}.txt -map $(TO) -lm $(LM) -order 2 > $(TEST1DIR)/$${i}.txt; \
	done;

separate:
	mv $(TESTDATA) old_testdata;
	mkdir $(TESTDATA)
	for i in $(shell seq 1 10); do \
		perl separator_big5.pl old_testdata/$${i}.txt > $(TESTDATA)/$${i}.txt; \
	done

clean:
	$(RM) $(OBJ) $(TARGET)

cleann:
	$(RM) -r $(OBJ) $(TARGET) $(ANSDIR) $(TO) corpus_seg.txt lm.cnt 
