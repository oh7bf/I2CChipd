/**************************************************************************
 * 
 * Test reading ADS1015 with I2Chip functions. 
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
 * Thu Aug 13 16:51:21 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_ads1015.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_ads1015 i2cdev address" << std::endl;
}

/// test reading ADS1015 chip with I2C 

/// The I2C device file name and ADS1015 chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "ADS1015";

  if( address < 0x48 || address > 0x4B )
  {
    cout << "-- valid I2C chip address 72 - 75\n";
    return -1;
  }

  Ads1015 *chip = new Ads1015(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  cout << "-- read registers\n";
  uint16_t config = chip->GetConfig();
  uint16_t low_thresh= 0; 
  uint16_t high_thresh= 0;
  uint16_t pga = 0;
  uint16_t config2 = 0;

  int error = chip->GetError();

  if( error != 0 )
  {
    cout << "-- problem reading CONFIG register, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    pga = chip->GetPGA();
    low_thresh = chip->GetLowThreshold();
    high_thresh = chip->GetHighThreshold();
    cout << "CONFIG = " << config << " ";
    cout << "PGA = " << pga << " ";
    cout << "LOW THRESHOLD = " << low_thresh << " ";
    cout << "HIGH THRESHOLD = " << high_thresh << "\n";

    chip->Powerdown();
    chip->SetPointer( ADS1015_CONV_REG );
    chip->ReadConversion();
    cout << "-- voltage: " << chip->GetVoltage() << " V, ";
    cout << "I2C error flag: " << chip->GetError() << "\n";


    cout << "-- read status bits\n";
    cout << "COMP_POL = " << chip->GetCompPolarity() << " ";
    cout << "COMP_LAT = " << chip->GetCompLatching() << " ";
    cout << "COMP_QUE = " << chip->GetCompQueue() << "\n";

    cout << "-- set COMP_POL to 0, 1, 0 and read back\n";
    chip->SetCompPolarity( 0 );
    cout << "COMP_POL = " << chip->GetCompPolarity();
    chip->SetCompPolarity( 1 );
    cout << ", " << chip->GetCompPolarity();
    chip->SetCompPolarity( 0 );
    cout << ", " << chip->GetCompPolarity() << "\n";

    cout << "-- set COMP_LAT to 0, 1, 0 and read back\n";
    chip->SetCompLatching( 0 );
    cout << "COMP_LAT = " << chip->GetCompLatching();
    chip->SetCompLatching( 1 );
    cout << ", " << chip->GetCompLatching();
    chip->SetCompLatching( 0 );
    cout << ", " << chip->GetCompLatching() << "\n";

    cout << "-- set COMP_QUE to 0, 1, 2, 3 and read back\n";
    chip->SetCompQueue( 0 );
    cout << "COMP_QUE = " << chip->GetCompQueue();
    chip->SetCompQueue( 1 );
    cout << ", " << chip->GetCompQueue();
    chip->SetCompQueue( 2 );
    cout << ", " << chip->GetCompQueue(); 
    chip->SetCompQueue( 3 );
    cout << ", " << chip->GetCompQueue() << "\n";


    cout << "-- restore original CONFIG, TLOW and THIGH register values\n";
    chip->SetConfig( config );
    chip->SetLowThreshold( low_thresh );
    chip->SetHighThreshold( high_thresh );
    chip->SetPointer( ADS1015_CONV_REG );
  }

  delete chip;

  return 0;

};

