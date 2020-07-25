/**************************************************************************
 * 
 * Test reading HTU21D with I2Chip functions. 
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
 * Fri Jul 24 14:35:51 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_htu21d.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_htu21d i2cdev" << std::endl;
}

/// test reading HTU21D chip with I2C 

/// The I2C device file name is given as parameter.
int main(int argc, char **argv)
{

  if( argc < 2 )
  {
    printusage();
    return 0;
  }

  string i2cdev = argv[ 1 ];
  string name = "HTU21D";

  Htu21d *chip = new Htu21d(name, i2cdev);

  cout << "Name tag    I2C device file\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice() << "\n";

  cout << "-- read user register\n";
  uint8_t reg = chip->GetUserRegister();

  int error = chip->GetError();

  if( error != 0 )
  {
    cout << "-- problem reading user register, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "USER = " << (int)reg << "\n";

    if( chip->IsEndBattery() ) cout << "-- supply voltage below 2.25 V\n";
    else cout << "-- supply voltage above 2.25 V\n";

    cout << "-- set resolution 1, 2, 3, 0 and read user register\n";
    for(uint8_t i = 1; i < 4; i++)
    {
      chip->SetResolution( i );
      cout << (int)chip->GetUserRegister() << ", ";
    }
    chip->SetResolution( 0 );
    cout << (int)chip->GetUserRegister() << "\n";

    cout << "-- trigger temperature measurement\n";
    chip->TriggerTemperature();
    usleep( 50000 ); // 50 ms delay

    cout << "-- read temperature register\n";
    if( !chip->ReadTemperature() )
    {
      cout << "I2C error flag: " << chip->GetError() << "\n";

      if( chip->IsCRCValid() ) cout << "-- CRC valid\n"; 
      else cout << "-- CRC error\n";
    }
    else
    {
      cout << "-- raw temperature value: " << chip->GetTReg() << ", CRC: ";
      cout << (int)chip->GetChecksum() << "\n";

      if( chip->IsCRCValid() ) cout << "-- CRC valid\n"; 
      else cout << "-- CRC error\n";

      cout << "-- temperature: " << chip->GetTemperature() << " C, ";
      cout << "I2C error flag: " << chip->GetError() << "\n";
      
      cout << "-- trigger humidity measurement\n";
      chip->TriggerHumidity();
      usleep( 50000 ); // 50 ms delay

      cout << "-- read humidity register\n";
      if( !chip->ReadHumidity() )
      {
        cout << "I2C error flag: " << chip->GetError() << "\n";

	if( chip->IsCRCValid() ) cout << "-- CRC valid\n"; 
        else cout << "-- CRC error\n";
      }
      else
      {
        cout << "-- raw humidity value: " << chip->GetTReg() << ", CRC: ";
        cout << (int)chip->GetChecksum() << "\n";

        if( chip->IsCRCValid() ) cout << "-- CRC valid\n"; 
        else cout << "-- CRC error\n";

        cout << "-- humidity: " << chip->GetHumidity() << " %%, ";
        cout << "I2C error flag: " << chip->GetError() << "\n";

	cout << "-- switch on heater and sleep 1 s\n";
        chip->HeaterOn();
	sleep( 1 );

        cout << "-- trigger temperature measurement\n";
        chip->TriggerTemperature();
        usleep( 50000 ); // 50 ms delay

        cout << "-- read temperature register\n";
        if( !chip->ReadTemperature() )
        {
          cout << "I2C error flag: " << chip->GetError() << "\n";

          if( chip->IsCRCValid() ) cout << "-- CRC valid\n"; 
          else cout << "-- CRC error\n";
        }
        else
        {
          cout << "-- temperature: " << chip->GetTemperature() << " C, ";
          cout << "I2C error flag: " << chip->GetError() << "\n";
      
          cout << "-- trigger humidity measurement\n";
          chip->TriggerHumidity();
          usleep( 50000 ); // 50 ms delay

          cout << "-- humidity: " << chip->GetHumidity() << " %%, ";
          cout << "I2C error flag: " << chip->GetError() << "\n";

	  cout << "-- switch off heater\n";
          chip->HeaterOff();
	}
      }
    }
  } 
  delete chip;

  return 0;

};

