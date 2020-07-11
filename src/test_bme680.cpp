/**************************************************************************
 * 
 * Test reading BME680 with I2Chip functions. 
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
 * Thu Jul  9 19:01:57 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_bme680.hpp"
#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_bmp680 i2cdev address" << std::endl;
}

/// test reading BME680 chip with I2C 

/// The I2C device file name and BME680 chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "BME680";

  if( address < 0x76 || address > 0x77 )
  {
    cout << "-- valid I2C chip address 118 - 119\n";
    return -1;
  }

  Bme680 chip = Bme680(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip.GetName();
  cout << "      " << chip.GetDevice();
  cout << "         " << chip.GetAddress() << "\n";

  cout << "-- read registers\n";
  uint8_t ID = chip.GetID();
  int error = chip.GetError();

  uint8_t config = 0;
  uint8_t ctrlhum =  0; 
  uint8_t ctrlmeas = 0;
  uint8_t ctrlgas1 = 0;
  uint8_t ctrlgas0 = 0;
  uint8_t meastatus = 0;

  double T = 0;
  double p = 0;
  double H = 0;
  uint8_t i = 0;
  int j = 0, k = 0;
  if( error != 0 )
  {
    cout << "-- problem reading ID register, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    config = chip.GetConfig();
    ctrlmeas = chip.GetControlMeasurement();
    ctrlhum = chip.GetHumidityControl();
    ctrlgas1 = chip.GetGasControl1();
    ctrlgas0 = chip.GetGasControl0();
    meastatus = chip.GetMeasStatus();

    cout << "ID = " << (int)(ID) << " ";
    cout << "CONFIG = " << (int)(config) << " ";
    cout << "CTRL_MEAS = " << (int)(ctrlmeas) << " ";
    cout << "CTRL_HUM = " << (int)(ctrlhum) << " ";
    cout << "CTRL_GAS1 = " << (int)(ctrlgas1) << " ";
    cout << "CTRL_GAS0 = " << (int)(ctrlgas0) << " ";
    cout << "MEAS_STATUS0 = " << (int)(meastatus) << "\n";

    if( chip.IsNewData() ) cout << "-- new data available\n";
    else cout << "-- no new data available\n";

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
      chip.SetOverSample( 0, i, 0 );
      ctrlmeas = chip.GetControlMeasurement();
      ctrlmeas &= 0xE0;
      ctrlmeas = ctrlmeas >> 5;
      cout << (int)ctrlmeas << ", ";
    }
    chip.SetOverSample( 0, 0, 0 );
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0xE0;
    ctrlmeas = ctrlmeas >> 5;
    cout << (int)ctrlmeas << "\n";

    cout << "-- set osrs_p 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetOverSample( 0, 0, i );
      ctrlmeas = chip.GetControlMeasurement();
      ctrlmeas &= 0x1C;
      ctrlmeas = ctrlmeas >> 2;
      cout << (int)ctrlmeas << ", ";
    }
    chip.SetOverSample( 0, 0, 0 );
    ctrlmeas = chip.GetControlMeasurement();
    ctrlmeas &= 0x1C;
    ctrlmeas = ctrlmeas >> 2;
    cout << (int)ctrlmeas << "\n";

    cout << "-- set osrs_h 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for(i = 1; i < 8; i++)
    {
      chip.SetOverSample( i, 0, 0 );
      ctrlhum = chip.GetHumidityControl();
      ctrlhum &= 0x07;
      cout << (int)ctrlhum << ", ";
    }
    chip.SetOverSample( 0, 0, 0 );
    ctrlmeas = chip.GetHumidityControl();
    ctrlmeas &= 0x07;
    cout << (int)ctrlmeas << "\n";

    cout << "-- read calibration data\n";
    if( !chip.GetCalibration() )
    {
      cout << "-- problem reading calibration data, quit now\n";
      return -1;
    } 
    else
    {
      cout << "-- 1 x H oversampling, 1 x T oversampling, 16 x p oversampling\n";
      chip.SetOverSample( 1, 2, 5 ); 

      cout << "-- filter 1\n";
      chip.SetFilter( 1 );

//      chip.RunGas();
//      chip.SetHeaterProfile( 0 );
//      chip.SetGasWaitTime(0, 0x59);
//      chip.SetGasHeatResistor(0, 1);

      cout << "    T[C]      H[%]     p[Pa]     t[ms]\n";
      for(j = 0; j < 10; j++)
      {
        chip.Forced();

	k = 0;
        while( !chip.IsNewData() )
	{
          k++;
          usleep( 1000 );
        }

        chip.GetTPHG();
        T = chip.GetTemperature();
        p = chip.GetPressure();
        H = chip.GetHumidity();
	cout << std::fixed;
        cout << j << "   ";
        cout << std::setprecision(2) << T << "     ";
        cout << std::setprecision(2) << H << "    ";
	cout << std::setprecision(0) << p << "     ";
        cout << k << "\n";
     }
    }
  }

  return 0;

};

