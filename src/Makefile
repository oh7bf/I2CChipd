# -*- mode: makefile -*-
# 
# Makefile for compiling 'i2chipd' on Raspberry Pi. 
#
# Fri Jul  3 11:50:56 CDT 2020
# Edit: Sat 26 Mar 2022 01:07:33 PM CET
#
# Jaakko Koivuniemi

CXX           = g++
CXXFLAGS      = -g -O2 -Wall -Wextra -std=c++11 
INCDIM        = /home/me/dim_v20r32/dim
LIBDIM        = /home/me/dim_v20r32/linux
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
MODULES      += Bh1750fvi.o
MODULES      += Lis3mdl.o
MODULES      += Lis3dh.o
MODULES      += Lis2mdl.o
MODULES      += File.o
MODULES      += SQLite.o
MODULES      += i2chipd.o 

%.o : %.cpp
	$(CXX) -I$(INCDIM) $(CXXFLAGS) -c $<

all: i2chipd test_bmp280 test_bme680 test_tmp102 test_htu21d test_max31865 test_ads1015 test_bh1750fvi test_lis3mdl test_lis3dh test_lis2mdl

i2chipd: $(MODULES) 
	$(LD) $(LDFLAGS) -L$(LIBDIM) $^ -ldim -lsqlite3 -o $@

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

test_bh1750fvi: I2Chip.o Bh1750fvi.o test_bh1750fvi.o
	$(LD) $(LDFLAGS) $^ -o $@

test_lis3mdl: I2Chip.o Lis3mdl.o test_lis3mdl.o
	$(LD) $(LDFLAGS) $^ -o $@

test_lis3dh: I2Chip.o Lis3dh.o test_lis3dh.o
	$(LD) $(LDFLAGS) $^ -o $@

test_lis2mdl: I2Chip.o Lis2mdl.o test_lis2mdl.o
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o

