/**************************************************************************
 * 
 * Test reading TMP102 with I2Chip functions. 
 *       
 * Copyright (C) 2020 Jaakko Koivuniemi.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************
 *
 * Sun Jul  5 09:22:54 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_tmp102.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_tmp102 i2cdev address" << std::endl;
}

/// test reading TMP102 chip with I2C 

/// The I2C device file name and TMP102 chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "TMP102";

  if( address < 0x48 || address > 0x4B )
  {
    cout << "-- valid I2C chip address 72 - 75\n";
    return -1;
  }

  Tmp102 chip = Tmp102(name, i2cdev, address);

  cout << "-- name tag: " << chip.GetName() << "\n";
  cout << "-- I2C device file: " << chip.GetDevice() << "\n";
  cout << "-- chip I2C address: " << chip.GetAddress() << "\n";
  cout << "\n";

  cout << "-- configuration register: " << chip.GetConfig() << "\n";
  cout << "-- alert polarity: " << chip.GetPolarity() << "\n";
  cout << "-- fault queue: " << (int)(chip.GetFaultQueue()) << "\n";
  cout << "-- conversion rate: " << chip.GetConversionRate() << "\n";
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";
  cout << "-- low T limit: " << chip.GetLowLimit() << "\n";
  cout << "-- high T limit: " << chip.GetHighLimit() << "\n";
  cout << "\n";

  chip.SetPointer( TMP102_TEMP_REG );
  cout << "-- temperature: " << chip.GetTemperature() << "\n";
  cout << "-- error flag: " << chip.GetError() << "\n";
  chip.ComparatorMode();
  chip.SetPolarity( 1 );
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";
  chip.SetPolarity( 0 );
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";

  chip.SetLowLimit(5000);
  cout << "-- low limit: " << chip.GetLowLimit() << "\n";
  chip.SetHighLimit(10000);
  cout << "-- high limit: " << chip.GetHighLimit() << "\n";
  usleep(50000);
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";

  chip.SetLowLimit(10000);
  cout << "-- low limit: " << chip.GetLowLimit() << "\n";
  chip.SetHighLimit(20000);
  cout << "-- high limit: " << chip.GetHighLimit() << "\n";
  usleep(50000);
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";

  chip.SetLowLimit(20000);
  cout << "-- low limit: " << chip.GetLowLimit() << "\n";
  chip.SetHighLimit(40000);
  cout << "-- high limit: " << chip.GetHighLimit() << "\n";
  usleep(50000);
  cout << "-- alert bit: " << chip.GetAlertBit() << "\n";

  cout << "\n";

  chip.Shutdown();
  sleep(1);
  chip.OneShot();
  usleep(50000);
  chip.SetPointer( 0 );
  cout << "-- temperature: " << chip.GetTemperature() << "\n";
  chip.Continuous();
  sleep(1);
  chip.SetPointer( 0 );
  cout << "-- temperature: " << chip.GetTemperature() << "\n";

  return 0;

};

