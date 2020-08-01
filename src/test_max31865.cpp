/**************************************************************************
 * 
 * Test reading MAX31865 with SPIChip functions. 
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
 * Fri 31 Jul 2020 05:15:43 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "test_max31865.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

void printusage()
{
  std::cout << "Usage: test_max31865 spidev" << std::endl;
}

int main(int argc, char **argv)
{
  if( argc < 2 )
  {
    printusage();
    return 0;
  }

  string device = argv[ 1 ];
  string name = "MAX31865";

  uint32_t clock = 500000; // SPI clock frequency
  double Rref = 430.0; // reference resistor

  Max31865 *chip = new Max31865(name, device, clock, Rref);

  cout << "Name tag      SPI device file\n";
  cout << chip->GetName();
  cout << "      " << chip->GetDevice() << "\n";
 
  cout << "-- set high fault 40000 and low fault 400, read back\n"; 
  chip->SetHighFault(40000);
  cout << "high fault = " << chip->GetHighFault(); 
  chip->SetLowFault(400);
  cout << " low fault = " << chip->GetLowFault() << " \n";

  cout << "-- switch on bias and sleep 0.5 s\n";
  chip->BiasOn();
  usleep( 500000 );

  cout << "-- start one shot measurement and sleep 100 ms\n";
  chip->OneShot();
  usleep( 100000 );

  cout << "-- start fault detection\n";
  chip->FaultDetection();
  usleep( 1000 );

  chip->ReadResistance();
  chip->CalcTemperature();
  cout << "T = " << chip->GetTemperature();
  cout << " C";

  if( chip->IsFault() ) cout << " fault = " << (int)chip->GetFaultStatusByte();
  cout << "\n";

  delete chip;

  return 0;
};

