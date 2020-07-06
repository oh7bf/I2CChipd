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
 * Edit: Mon Jul  6 17:10:28 CDT 2020
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

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip.GetName();
  cout << "      " << chip.GetDevice();
  cout << "         " << chip.GetAddress() << "\n";

  cout << "-- read registers\n";
  uint16_t config = chip.GetConfig();
  uint16_t lowlimit = 0; 
  uint16_t highlimit = 0;
  uint16_t config2 = 0;
  int TM = 0, SD = 0;

  int error = chip.GetError();

  if( error != 0 )
  {
    cout << "-- problem reading CONFIG register, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    lowlimit = chip.GetLowLimit();
    highlimit = chip.GetHighLimit();

    cout << "CONFIG = " << config << " ";
    cout << "TLOW = " << lowlimit << " ";
    cout << "THIGH = " << highlimit << "\n";

    chip.SetPointer( TMP102_TEMP_REG );
    cout << "-- temperature: " << chip.GetTemperature() << " C, ";
    cout << "I2C error flag: " << chip.GetError() << "\n";
    cout << "-- set extended mode\n";

    chip.ExtendedMode();
    usleep(250000); // assume 4 Hz conversion rate
    chip.SetPointer( TMP102_TEMP_REG );
    cout << "-- temperature: " << chip.GetTemperature() << " C, ";
    cout << "I2C error flag: " << chip.GetError() << "\n";

    cout << "-- set normal mode\n";
    chip.NormalMode();
    usleep(250000); // assume 4 Hz conversion rate
    chip.SetPointer( TMP102_TEMP_REG );
    cout << "-- temperature: " << chip.GetTemperature() << " C, ";
    cout << "I2C error flag: " << chip.GetError() << "\n";

    cout << "-- read status bits\n";
    cout << "ALERT = " << chip.GetAlertBit() << " ";
    cout << "POL = " << chip.GetPolarity() << " ";
    cout << "FQ = " << (int)(chip.GetFaultQueue()) << " ";
    cout << "CR = " << chip.GetConversionRate() << "\n";

    cout << "-- set POL to 0, 1, 0 and read back\n";
    chip.SetPolarity( 0 );
    cout << "POL = " << chip.GetPolarity();
    chip.SetPolarity( 1 );
    cout << ", " << chip.GetPolarity();
    chip.SetPolarity( 0 );
    cout << ", " << chip.GetPolarity() << "\n";

    cout << "-- set FQ to 1, 2, 3, 0 and read back\n";
    chip.SetFaultQueue( 1 );
    cout << "FQ = " << (int)(chip.GetFaultQueue());
    chip.SetFaultQueue( 2 );
    cout << ", " << (int)(chip.GetFaultQueue());
    chip.SetFaultQueue( 3 );
    cout << ", " << (int)(chip.GetFaultQueue()); 
    chip.SetFaultQueue( 0 );
    cout << ", " << (int)(chip.GetFaultQueue()) << "\n";

    cout << "-- set CR to 1, 2, 3, 0 and read back\n";
    chip.SetConversionRate( 1 );
    cout << "CR = " << (int)(chip.GetConversionRate());
    chip.SetConversionRate( 2 );
    cout << ", " << (int)(chip.GetConversionRate());
    chip.SetConversionRate( 3 );
    cout << ", " << (int)(chip.GetConversionRate()); 
    chip.SetConversionRate( 0 );
    cout << ", " << (int)(chip.GetConversionRate()) << "\n";

    cout << "-- set interrupt mode and read TM\n";
    chip.InterruptMode();
    config2 = chip.GetConfig();
    TM = (int)( ( config2 & 0x0200 ) >> 9 );
    cout << "TM = " << TM << "\n";

    cout << "-- set comparator mode and read TM\n";
    chip.ComparatorMode();
    config2 = chip.GetConfig();
    TM = (int)( ( config2 & 0x0200 ) >> 9 );
    cout << "TM = " << TM << "\n";

    cout << "-- shutdown and read SD\n";
    chip.Shutdown();
    config2 = chip.GetConfig();
    SD = (int)( ( config2 & 0x0100 ) >> 8 );
    cout << "SD = " << SD << "\n";

    cout << "-- continuous conversion and read SD\n";
    chip.Continuous();
    config2 = chip.GetConfig();
    SD = (int)( ( config2 & 0x0100 ) >> 8 );
    cout << "SD = " << SD << "\n";

    cout << "-- shutdown 1 s and do one shot conversion\n";
    chip.Shutdown();
    sleep(1);
    chip.OneShot();
    usleep(50000);
    chip.SetPointer( 0 );
    cout << "-- temperature: " << chip.GetTemperature() << " C, ";
    cout << "I2C error flag: " << chip.GetError() << "\n";

    cout << "-- start continuos conversion and read temperature after 1 s\n";
    chip.Continuous();
    sleep(1);
    chip.SetPointer( 0 );
    cout << "-- temperature: " << chip.GetTemperature() << " C, ";
    cout << "I2C error flag: " << chip.GetError() << "\n";

    cout << "-- restore original CONFIG, TLOW and THIGH register values\n";
    chip.SetConfig( config );
    chip.SetLowLimit( lowlimit );
    chip.SetHighLimit( highlimit );

  }

  return 0;

};

