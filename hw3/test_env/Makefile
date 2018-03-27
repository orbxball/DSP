# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /data/DSP_HW3/103_2/srilm-1.5.10
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

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
        $(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c $<
run:
        @#TODO How to run your code toward different txt? 
        @for i in $(shell seq 1 10) ; do \
            echo "Running $$i.txt"; \
            ./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
        done;
map:
        @#TODO How to map?
        @echo "Mapping!"
        @#./mapping $(FROM) $(TO)
        @#matlab mapping.m $(FROM) $(TO)
        @#python mapping.py $(FROM) $(TO)
        @#sh mapping.sh $(FROM) $(TO)
        @#perl mapping.pl Big5-ZhuYin.map ZhuYin-Big5.map
clean:
        $(RM) $(OBJ) $(TARGET)

