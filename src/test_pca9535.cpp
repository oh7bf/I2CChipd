/**************************************************************************
 * 
 * Test reading PCA9535 with I2Chip functions. 
 *       
 * Copyright (C) 2023 Jaakko Koivuniemi.
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
 * Thu 02 Nov 2023 06:49:00 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_pca9535.hpp"
#include "I2Chip.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_pca9535 i2cdev address" << std::endl;
}

/// test reading PCA9535 chip with I2C 

/// The I2C device file name and PCA9535 chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "PCA9535";

  if( address < 0x20 || address > 0x27 )
  {
    cout << "-- valid I2C chip address 32 - 39\n";
    return -1;
  }

  Pca9535 *chip = new Pca9535(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  cout << "-- inputs: " << chip->GetInputs() << "\n";

  int error = chip->GetError();

  if( error != 0 )
  {
    cout << "-- problem reading inputs, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "-- outputs: " << chip->GetOutputs() << "\n";
    cout << "-- polarity inversions: " << chip->GetPolInversions() << "\n";
    cout << "-- port configs: " << chip->GetPortConfigs() << "\n";
  }

  delete chip;

  return 0;

};

