# -*- mode: makefile -*-
# 
# Makefile for compiling 'i2chipd' on Raspberry Pi. 
#
# Fri Jul  3 11:50:56 CDT 2020
# Edit: 
# Jaakko Koivuniemi

CXX           = g++
CXXFLAGS      = -g -O2 -Wall -Wextra -std=c++11 
LD            = g++
LDFLAGS       = -O2

MODULES       = I2Chip.o 
MODULES      += Tmp102.o
MODULES      += i2chipd.o 



%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: i2chipd test_tmp102 

i2chipd: $(MODULES) 
	$(LD) $(LDFLAGS) $^ -o $@

test_tmp102: I2Chip.o Tmp102.o test_tmp102.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o

