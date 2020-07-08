/**************************************************************************
 * 
 * Test reading BMP280 with I2Chip functions. 
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
 * Tue 07 Jul 2020 02:19:06 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_bmp280.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_bmp280 i2cdev address" << std::endl;
}

/// test reading BMP280 chip with I2C 

/// The I2C device file name and BMP280 chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "BMP280";

  if( address < 0x76 || address > 0x77 )
  {
    cout << "-- valid I2C chip address 118 - 119\n";
    return -1;
  }

  Bmp280 chip = Bmp280(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip.GetName();
  cout << "      " << chip.GetDevice();
  cout << "        " << chip.GetAddress() << "\n";

  cout << "-- read registers\n";
  uint8_t ID = chip.GetID();
  uint8_t config = 0;
  uint8_t status =  0; 
  uint8_t ctrlmeas = 0;
  int osrs_p = 0, osrs_t = 0, filter = 0;

  int error = chip.GetError();

  int32_t T = 0;
  uint32_t p = 0;
  if( error != 0 )
  {
    cout << "-- problem reading ID register, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    config = chip.GetConfig();
    status = chip.GetStatus();
    ctrlmeas = chip.GetControlMeasurement();

    cout << "ID = " << (int)(ID) << " ";
    cout << "CONFIG = " << (int)(config) << " ";
    cout << "STATUS = " << (int)(status) << " ";
    cout << "CTRL_MEAS = " << (int)(ctrlmeas) << "\n";

    cout << "-- read calibration data\n";
    if( !chip.GetCalibration() )
    {
      cout << "-- problem reading calibration data, quit now\n";
      return -1;
    } 
    else
    {
      chip.SetTOverSample( 1 );
      chip.SetPOverSample( 1 );
      chip.Forced();
      sleep( 1 );
      cout << chip.tfine << "\n";
      T = chip.GetTemperature();
      cout << chip.tfine << "\n";
      p = chip.GetPressure();
      cout << chip.tfine << "\n";
      cout << "100 x T =" << T << "\n";
      cout << "p(Q24.8) =" << p << "\n";
    }

  }


  return 0;

};

