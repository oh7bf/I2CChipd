/**************************************************************************
 * 
 * Test reading LIS3DH with I2Chip functions. 
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
 * Sun Feb 27 20:41:09 CST 2022
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_lis3dh.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_lis3dh i2cdev address" << std::endl;
}

/// test reading LIS3DH chip with I2C 

/// The I2C device file name and LIS3DH chip address are given as parameters.
int main(int argc, char **argv)
{

  if( argc < 3 )
  {
    printusage();
    return 0;
  }

  uint16_t address = atoi( argv[ 2 ] );
  string i2cdev = argv[ 1 ];
  string name = "LIS3DH";

  if( address != 0x18 && address != 0x19 )
  {
    cout << "-- valid I2C chip address 24 or 25\n";
    return -1;
  }

  Lis3dh *chip = new Lis3dh(name, i2cdev, address);

  cout << "Name tag    I2C device file    I2C address\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice();
  cout << "         " << chip->GetAddress() << "\n";

  bool overrun = true;
  int i = 0, j = 0;
  int error = 0;
  uint8_t hpm = 0;
  uint8_t hpcf = 0;
  uint8_t datarate = 0;
  uint8_t fullscale = 0;
  uint8_t reference = 0;

  if( !chip->WhoAmI() )
  {
    cout << "-- problem reading chip ID, check parameters above and wiring, quit now\n";
    return -1;
  }
  else
  {
    cout << "-- read registers\n";
    cout << "STATUS_REG = " << (int)chip->GetStatus() << "\n";
    cout << "STATUS_REG = " << (int)chip->GetStatus() << "\n";
    cout << "REFERENCE  = " << (int)chip->GetReference() << "\n";
    cout << "CTRL_REG1  = " << (int)chip->GetCtrlReg1() << "\n";

    cout << "-- read data rate, high-pass filter mode, full scale, FIFO mode\n";
    cout << "Data rate = " << (int)chip->GetDataRate() << "\n";
    cout << "High-pass filter mode = " << (int)chip->GetHpm() << "\n";
    cout << "High-pass filter cutoff = " << (int)chip->GetHpcf() << "\n";
    cout << "Full scale = " << (int)chip->GetFullScale() << "\n";
    cout << "FIFO mode = " << (int)chip->GetFifoMode() << "\n";
    cout << "FIFO watermark = " << (int)chip->GetWatermark() << "\n";
    cout << "FIFO samples = " << (int)chip->Samples() << "\n";

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

    cout << "-- set high-pass filter mode 1, 2, 3, 0 and read back\n";
    for( i= 1; i < 4; i++)
    {
      chip->SetHpm( i );
      hpm = chip->GetHpm();
      cout << (int)hpm << ", ";
    }
    chip->SetHpm( 0 );
    hpm = chip->GetHpm();
    cout << (int)hpm << "\n";

    cout << "-- boot\n";
    chip->Boot();
    cout << "-- wait 10 ms\n";
    usleep( 10000 ); // sleep 1 ms

    cout << "-- set output data rate 0\n";
    chip->SetDataRate( 0 );
    cout << "-- read data rate bits\n";
    cout << (int)chip->GetDataRate() << "\n";
    cout << "-- enable X data\n";
    chip->XEnable();
    cout << "-- enable Y data\n";
    chip->YEnable();
    cout << "-- enable Z data\n";
    chip->ZEnable();
    cout << "-- set output data rate 10 Hz\n";
    chip->SetDataRate( 2 );
    cout << "-- read data rate bits\n";
    cout << (int)chip->GetDataRate() << "\n";

    cout << "-- set block data update\n";
    chip->BlockDataEnable();
    cout << "-- normal mode\n";
    chip->NormalMode();

    cout << "-- enable ADC\n";
    chip->ADCEnable();
    cout << "-- enable temperature measurement\n";
    chip->TempEnable();

    cout << "-- boot\n";
    chip->Boot();

    cout << "-- set block data update\n";
    chip->BlockDataEnable();
    cout << "-- normal mode\n";
    chip->NormalMode();

//    reference = chip->GetReference();

//    cout << "-- enable FIFO\n";
//    chip->FifoEnable();
//
//    cout << "-- set FIFO mode 2\n";
//    chip->SetFifoMode( 2 );
//
//    cout << "-- read FIFO mode\n";
//    cout << (int)chip->GetFifoMode() << "\n";

//    cout << "-- sleep 100 ms\n";
//    usleep( 100000 ); // sleep 100 ms

//    cout << "-- FS = " << chip->GetFS() << "\n";

    cout << "outX[g]       outY[g]        outZ[g]     latency[ms]     Temp\n";
    int i, j, error = 0;
    uint8_t status = 0;
    for( j = 0; j < 10; j++ )
    {
        i = 0;
    	while( !chip->NewDataXYZ() && i < 2000 )
        {
            usleep( 1000 ); // sleep 1 ms
            i++;	
        }

        if( i >= 2000 )
        {
            cout << "-- timeout waiting new data, exit now\n";
            return -1;
        }

	if( chip->OverRunXYZ() ) cout << "-- overrun\n";

	if( !chip->Readg() )
        {
          error = chip->GetError();
          cout << "-- error " << error << "\n";
          cout << "-- problem reading field, quit now\n";
          return -1;
        }

//        cout << chip->GetOutX() << "     ";
//        cout << chip->GetOutY() << "     ";
//       	cout << chip->GetOutZ() << "     ";

        cout << chip->Getgx() << "     ";
        cout << chip->Getgy() << "     ";
       	cout << chip->Getgz() << "     ";
	cout << i << "      ";
        chip->ReadAdc();
        cout << chip->GetAdc3() << "\n";
    }
  }

  delete chip;

  return 0;

};

