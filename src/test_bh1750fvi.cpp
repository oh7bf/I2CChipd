/**************************************************************************
 * 
 * Test reading BH1750FVI with I2Chip functions. 
 *       
 * Copyright (C) 2021 Jaakko Koivuniemi.
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
 * Sun Aug  8 20:56:25 CDT 2021
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_bh1750fvi.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_bh1750fvi i2cdev address" << std::endl;
}

/// test reading BH1750FVI chip with I2C 

/// The I2C device file name and BH1750FVI chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "BH1750FVI";

  if( address != 0x23 && address != 0x5C )
  {
    cout << "-- valid I2C chip address 35 or 92\n";
    return -1;
  }

  Bh1750fvi *chip = new Bh1750fvi(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  cout << "-- power on\n";
  chip->PowerOn();

  int error = chip->GetError();

  if( error != 0 )
  {
    cout << "-- problem changing to Power On, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {

    cout << "-- start one time high resolution mode\n";
    chip->OneTimeHighResMode();

    cout << "-- sleep 700 ms\n";
    usleep( 700000 );   
 
    cout << "-- read illuminance\n";
    if( !chip->ReadIlluminance() )
    {
      error = chip->GetError();
      cout << "-- problem reading illuminance, quit now\n";
      return -1;
    }

    cout << "-- illuminance = " << chip->GetIlluminance() << " lx\n";

  }

  delete chip;

  return 0;

};

