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
 * Edit: Wed 08 Jul 2020 07:21:17 PM CDT
 *
 * Jaakko Koivuniemi
 **/

#include "test_bmp280.hpp"
#include <iostream>
#include <iomanip>
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
  cout << "         " << chip.GetAddress() << "\n";

  cout << "-- read registers\n";
  uint8_t ID = chip.GetID();
  int error = chip.GetError();

  uint8_t config = 0;
  uint8_t status =  0; 
  uint8_t ctrlmeas = 0;

  double T = 0;
  double p = 0;
  uint8_t i = 0;
  int j = 0;
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

    cout << "-- set t_sb 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetStandby( i );
      config = chip.GetConfig();
      config &= 0xE0;
      config = config >> 5;
      cout << (int)config << ", ";
    }
    chip.SetStandby( 0 );
    config = chip.GetConfig();
    config &= 0xE0;
    config = config >> 5;
    cout << (int)config << "\n";

    cout << "-- set filter 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetFilter( i );
      config = chip.GetConfig();
      config &= 0x1C;
      config = config >> 2;
      cout << (int)config << ", ";
    }
    chip.SetFilter( 0 );
    config = chip.GetConfig();
    config &= 0x1C;
    config = config >> 2;
    cout << (int)config << "\n";

    cout << "-- set osrs_t 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetTOverSample( i );
      ctrlmeas = chip.GetControlMeasurement();
      ctrlmeas &= 0xE0;
      ctrlmeas = ctrlmeas >> 5;
      cout << (int)ctrlmeas << ", ";
    }
    chip.SetTOverSample( 0 );
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0xE0;
    ctrlmeas = ctrlmeas >> 5;
    cout << (int)ctrlmeas << "\n";

    cout << "-- set osrs_p 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetPOverSample( i );
      ctrlmeas = chip.GetControlMeasurement();
      ctrlmeas &= 0x1C;
      ctrlmeas = ctrlmeas >> 2;
      cout << (int)ctrlmeas << ", ";
    }
    chip.SetPOverSample( 0 );
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0x1C;
    ctrlmeas = ctrlmeas >> 2;
    cout << (int)ctrlmeas << "\n";

    cout << "-- set forced mode and read back\n";
    chip.Forced();
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0x03;
    cout << "mode=" << (int)ctrlmeas << "\n";

    cout << "-- set normal mode and read back\n";
    chip.Normal();
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0x03;
    cout << "mode=" << (int)ctrlmeas << "\n";
    usleep( 30000 );

    cout << "-- set sleep mode and read back\n";
    chip.Sleep();
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0x03;
    cout << "mode=" << (int)ctrlmeas << "\n";

    cout << "-- read calibration data\n";
    if( !chip.GetCalibration() )
    {
      cout << "-- problem reading calibration data, quit now\n";
      return -1;
    } 
    else
    {
      cout << "-- 1 x T oversampling, 1 x p oversampling\n";
      chip.SetTOverSample( 1 ); // 1 x oversampling
      chip.SetPOverSample( 1 ); // 1 x oversampling

      cout << "-- forced conversion\n";
      chip.Forced();

      usleep( 10000 );
      cout << "-- read temperature and pressure registers\n";
      chip.Measure();
      T = chip.GetTemperature();
      p = chip.GetPressure();
      cout << "T =" << T << " C, p =" << std::setprecision(9) << p << " Pa\n";

      cout << "-- chip reset\n";
      chip.Reset();
      usleep( 10000 );

      cout << "-- 1 x T oversampling, 1 x p oversampling\n";
      chip.SetTOverSample( 1 ); // 1 x oversampling
      chip.SetPOverSample( 1 ); // 1 x oversampling

      chip.Forced();
      usleep( 10000 );
      cout << "-- read temperature and pressure registers\n";
      chip.Measure();
      T = chip.GetTemperature();
      p = chip.GetPressure();
      cout << "T =" << T << " C, p =" << std::setprecision(9) << p << " Pa\n";

      cout << "-- normal mode\n";
      chip.SetStandby( 1 ); // 62.5 ms
      chip.Normal();
      cout << "   T[C]      p[Pa]\n";
      for( j = 0; j < 10; j++)
      {
        usleep( 62500 );
        chip.Measure();
	T = chip.GetTemperature();
        p = chip.GetPressure();
        cout << j << "  " << T << "     ";
        cout << std::setprecision(9) << p << "\n";
      }
    }

  }

  return 0;

};

