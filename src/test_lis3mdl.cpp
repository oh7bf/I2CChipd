/**************************************************************************
 * 
 * Test reading LIS3MDL with I2Chip functions. 
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
 * Sat Sep 11 19:15:50 CDT 2021
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_lis3mdl.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_lis3mdl i2cdev address" << std::endl;
}

/// test reading LIS3MDL chip with I2C 

/// The I2C device file name and LIS3MDL chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "LIS3MDL";

  if( address != 0x1C && address != 0x1E )
  {
    cout << "-- valid I2C chip address 28 or 30\n";
    return -1;
  }

  Lis3mdl *chip = new Lis3mdl(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  bool overrun = true;
  int i = 0, j = 0;
  int error = 0;
  uint8_t xyopmode = 0;
  uint8_t zopmode = 0;
  uint8_t datarate = 0;
  uint8_t fullscale = 0;

  if( !chip->WhoAmI() )
  {
    cout << "-- problem reading chip ID, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "-- read registers\n";
    cout << "STATUS_REG = " << (int)chip-> GetStatus() << "\n";
    cout << "INT_CFG = " << (int)chip->GetIntConfig() << "\n";
    cout << "INT_SRC = " << (int)chip->GetIntSource() << "\n";
    cout << "INT_THS = " << chip->GetIntThreshold() << "\n";
    cout << "-- read operation mode, full scale and sample rate\n";
    cout << "XY operation mode = " << (int)chip->GetXYOpMode() << "\n";
    cout << "Data rate = " << (int)chip->GetDataRate() << "\n";
    cout << "Full scale = " << (int)chip->GetFullScale() << "\n";
    cout << "Operation mode = " << (int)chip->GetOpMode() << "\n";
    cout << "Z operation mode = " << (int)chip->GetZOpMode() << "\n";

    cout << "-- set XY operation mode 1, 2, 3, 0 and read back\n";
    for( i= 1; i < 4; i++)
    {
      chip->SetXYOpMode( i );
      xyopmode = chip->GetXYOpMode();
      cout << (int)xyopmode << ", ";
    }
    chip->SetXYOpMode( 0 );
    xyopmode = chip->GetXYOpMode();
    cout << (int)xyopmode << "\n";

    cout << "-- set Z operation mode 1, 2, 3, 0 and read back\n";
    for( i= 1; i < 4; i++)
    {
      chip->SetZOpMode( i );
      zopmode = chip->GetZOpMode();
      cout << (int)zopmode << ", ";
    }
    chip->SetZOpMode( 0 );
    zopmode = chip->GetZOpMode();
    cout << (int)zopmode << "\n";

    cout << "-- set data rate 1, 2, 3, 4, 5, 6, 7, 0 and read back\n";
    for( i= 1; i < 8; i++)
    {
      chip->SetDataRate( i );
      datarate = chip->GetDataRate();
      cout << (int)datarate << ", ";
    }
    chip->SetDataRate( 0 );
    datarate = chip->GetDataRate();
    cout << (int)datarate << "\n";

    cout << "-- set full scale 1, 2, 3, 0 and read back\n";
    for( i= 1; i < 4; i++)
    {
      chip->SetFullScale( i );
      fullscale = chip->GetFullScale();
      cout << (int)fullscale << ", ";
    }
    chip->SetFullScale( 0 );
    fullscale = chip->GetFullScale();
    cout << (int)fullscale << "\n";

    cout << "-- set XY ultra-high performance mode\n";
    chip->SetXYOpMode( 3 );

    cout << "-- set Z ultra-high performance mode\n";
    chip->SetZOpMode( 3 );

//    cout << "-- set data rate 10 Hz\n";
//    chip->SetDataRate( 0 );

    cout << "-- set full scale +- 4 G\n";
    chip->SetFullScale( 0 );

    cout << "-- fast read enable\n";
    chip->FastReadEnable();

//    cout << "-- set big endian mode\n";
//    chip->BigEndian();

    chip->ReadB();

    cout << "-- start single conversion mode\n";
//    chip->ContinuousMode();
//    chip->PowerDown();
    chip->SingleConversionMode();

    cout << "Bx[G]       By[G]        Bz[G]        latency[s]\n";
    j = 0;
    overrun = true;
    while( j  < 10 && overrun )
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
    }
    j++;
  }

  chip->PowerDown();
  chip->ReadB();

  delete chip;

  return 0;

};

