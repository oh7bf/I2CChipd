/**************************************************************************
 * 
 * Test reading LTR390UV with I2Chip functions. 
 *       
 * Copyright (C) 2022 Jaakko Koivuniemi.
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
 * Mon 02 May 2022 08:22:11 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_ltr390uv.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_ltr390uv i2cdev" << std::endl;
}

/// test reading LTR390UV chip with I2C 

/// The I2C device file name is given as parameter.
int main(int argc, char **argv)
{

  if( argc < 2 )
  {
    printusage();
    return 0;
  }

  string i2cdev = argv[ 1 ];
  string name = "LTR390UV";

  Ltr390uv *chip = new Ltr390uv(name, i2cdev);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  cout << "-- PART_ID = " << (int)chip->GetID() << "\n";

  int error = chip->GetError();

  if( error != 0 )
  {
    cout << "-- problem reading PART_ID, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "-- MAIN_STATUS = " << (int)chip->GetStatus() << "\n";
    cout << "-- Resolution = " << (int)chip->GetResolution() << "\n";
    cout << "-- Measurement rate = " << (int)chip->GetMeasRate() << "\n";
    cout << "-- Gain = " << (int)chip->GetGain() << "\n";
    cout << "-- set ambient light mode\n";
    chip->AmbientLightMode();
    chip->LightEnable();

    int i = 0;

    while( !chip->NewData() && i < 200 )
    {
      usleep( 10000 );
    }

    if( i < 200 )
    {
      if( chip->ReadAmbientLight() )
      {
        cout << "-- ambient light = " << chip->GetAmbientLight();
        cout << " with latency " << 10*i << " ms\n";
      }
      else
      {
        cout << "-- failed to read ambient light\n";
      }
    }
    else
    {
      cout << "-- timeout while reading chip\n";
    }
  }

  delete chip;

  return 0;

};

