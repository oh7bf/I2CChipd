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
MODULES      += Bmp280.o
MODULES      += Bme680.o
MODULES      += Htu21d.o
MODULES      += SPIChip.o
MODULES      += Max31865.o
MODULES      += Ads1015.o
MODULES      += File.o
MODULES      += SQLite.o
MODULES      += i2chipd.o 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: i2chipd test_bmp280 test_bme680 test_tmp102 test_htu21d test_max31865 test_ads1015 

i2chipd: $(MODULES) 
	$(LD) $(LDFLAGS) -lsqlite3 $^ -o $@

test_bmp280: I2Chip.o Bmp280.o test_bmp280.o
	$(LD) $(LDFLAGS) $^ -o $@

test_bme680: I2Chip.o Bme680.o test_bme680.o
	$(LD) $(LDFLAGS) $^ -o $@

test_tmp102: I2Chip.o Tmp102.o test_tmp102.o
	$(LD) $(LDFLAGS) $^ -o $@

test_htu21d: I2Chip.o Htu21d.o test_htu21d.o
	$(LD) $(LDFLAGS) $^ -o $@

test_max31865: SPIChip.o Max31865.o test_max31865.o
	$(LD) $(LDFLAGS) $^ -o $@

test_ads1015: I2Chip.o Ads1015.o test_ads1015.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o

