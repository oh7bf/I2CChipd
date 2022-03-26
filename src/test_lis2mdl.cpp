/**************************************************************************
 * 
 * Test reading LIS2MDL with I2Chip functions. 
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
 * Sat 26 Mar 2022 01:19:51 PM CET
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_lis2mdl.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_lis2mdl i2cdev" << std::endl;
}

/// test reading LIS2MDL chip with I2C 

/// The I2C device file name and LIS2MDL chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 2 )
  {
    printusage();
    return 0;
  }

  string i2cdev = argv[ 1 ];
  string name = "LIS2MDL";

  Lis2mdl *chip = new Lis2mdl(name, i2cdev);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  bool overrun = true;
  int i = 0, j = 0;
  int error = 0;
  uint8_t opmode = 0;
  uint8_t datarate = 0;

  if( !chip->WhoAmI() )
  {
    cout << "-- problem reading chip ID, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "-- read registers\n";
    cout << "STATUS_REG = " << (int)chip-> GetStatus() << "\n";
    cout << "INT_CTRL_REG = " << (int)chip->GetIntConfig() << "\n";
    cout << "INT_SOURCE_REG = " << (int)chip->GetIntSource() << "\n";
    cout << "INT_THS_REG = " << chip->GetIntThreshold() << "\n";
    cout << "-- read operation mode, full scale and sample rate\n";
    cout << "Data rate = " << (int)chip->GetDataRate() << "\n";
    cout << "Operation mode = " << (int)chip->GetOpMode() << "\n";

    cout << "-- set operation mode 0, 1, 2, 3 and read back\n";
    for( i= 0; i < 4; i++)
    {
      chip->SetOpMode( i );
      opmode = chip->GetOpMode();
      cout << (int)opmode << ", ";
    }
    cout << "\n";

    cout << "-- set data rate 1, 2, 3, 0 and read back\n";
    for( i= 1; i < 4; i++)
    {
      chip->SetDataRate( i );
      datarate = chip->GetDataRate();
      cout << (int)datarate << ", ";
    }
    chip->SetDataRate( 0 );
    datarate = chip->GetDataRate();
    cout << (int)datarate << "\n";

    cout << "-- read X offset\n";
    cout << chip->GetXOffset() << "\n";

    cout << "-- set X offset -12345\n";
    chip->SetXOffset( -12345 );

    cout << "-- read X offset\n";
    cout << chip->GetXOffset() << "\n";

    cout << "-- set X offset 0\n";
    chip->SetXOffset( 0 );

    cout << "-- read Y offset\n";
    cout << chip->GetYOffset() << "\n";

    cout << "-- set Y offset -12345\n";
    chip->SetYOffset( -12345 );

    cout << "-- read Y offset\n";
    cout << chip->GetYOffset() << "\n";

    cout << "-- set Y offset 0\n";
    chip->SetYOffset( 0 );

    cout << "-- read Z offset\n";
    cout << chip->GetZOffset() << "\n";

    cout << "-- set Z offset -12345\n";
    chip->SetZOffset( -12345 );

    cout << "-- read Z offset\n";
    cout << chip->GetZOffset() << "\n";

    cout << "-- set Z offset 0\n";
    chip->SetZOffset( 0 );

    cout << "-- read interrupt threshold\n";
    cout << chip->GetIntThreshold() << "\n";

    cout << "-- set interrupt threshold 12345\n";
    chip->SetIntThreshold( 12345 );

    cout << "-- read interrupt threshold\n";
    cout << chip->GetIntThreshold() << "\n";

    cout << "-- set interrupt threshold 0\n";
    chip->SetIntThreshold( 0 );

//    cout << "-- set data rate 10 Hz\n";
//    chip->SetDataRate( 0 );

//    cout << "-- set big endian mode\n";
//    chip->BigEndian();

    cout << "-- enable temperature compensation\n";
    chip->TempCompEnable();

    cout << "-- enable block data\n";
    chip->BlockDataEnable();

    cout << "-- start single conversion mode\n";
//    chip->ContinuousMode();
    chip->SingleMode();

    cout << "Bx[G]       By[G]        Bz[G]        latency[s]\n";
    j = 0;
    overrun = true;
    while( j  < 10 )
    { 
      i = 0;
      while( !chip->NewDataXYZ() && i < 1000 )
      {
        // sleep 100 us
        usleep( 100 );
        i++;
      }

      if( !chip->NewDataXYZ() )
      {
        cout << "-- time out reading chip\n";
      }
      else
      {
      	if( chip->OverRunXYZ() )
	{
	  cout << "-- overrun\n";
	  overrun = true;
	}
	else
        {
          overrun = false;
	}

	if( !chip->ReadB() )
        {
          error = chip->GetError();
          cout << "-- error " << error << "\n";
          cout << "-- problem reading field, quit now\n";
          return -1;
        }
//        cout << chip->GetOutX() << "   ";
//        cout << chip->GetOutY() << "   ";
//        cout << chip->GetOutZ() << "\n";

	cout << chip->GetBx() << "   ";
        cout << chip->GetBy() << "   ";
        cout << chip->GetBz() << "           ";
        cout << i*0.001;
        cout << "\n";
      }	    

      j++;

      chip->SingleMode();
    }
  }

  chip->IdleMode();
  chip->ReadB();

  delete chip;

  return 0;

};

