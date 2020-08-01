/**************************************************************************
 * 
 * Max31865 class member functions for configuration and reading with SPI. 
 *       
 * Copyright (C) 2018 - 2020 Jaakko Koivuniemi.
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
 * Sat  3 Nov 20:21:27 CDT 2018
 * Edit: Sun  4 Nov 19:29:16 CST 2018
 *
 * Jaakko Koivuniemi
 **/



#include "Max31865.hpp"
#include <cmath>

using namespace std;


Max31865::~Max31865() { };

/// Turn off RTD bias.
void Max31865::BiasOff()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg &= 0x7F;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Turn on RTD bias.
void Max31865::BiasOn()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x80;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Initiate one shot measurement of RTD.
void Max31865::OneShot()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x20;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Fault detection with automatic delay.
void Max31865::FaultDetection()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x84;
   confreg &= 0x95;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Fault detection with cycle 1.
void Max31865::FaultDetectionCycle1()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x88;
   confreg &= 0x99;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Fault detection with cycle 2.
void Max31865::FaultDetectionCycle2()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x8C;
   confreg &= 0x9D;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}


/// Change configuration to three wire measurement.
void Max31865::ThreeWire()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x10;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Change configuration to two or four wire measurement.
void Max31865::TwoFourWire()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg &= 0xEF;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Clear fault status.
void Max31865::FaultStatusClear()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x02;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Set filter frequency to 50 Hz.
void Max31865::Filter50Hz()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg |= 0x01;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Set filter frequency to 60 Hz.
void Max31865::Filter60Hz()
{
   uint8_t confreg = 0x00;
   uint8_t readbuffer[ 2 ];

   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_READ, 0x00, 2, readbuffer);
   confreg = readbuffer[ 1 ];
   confreg &= 0xFE;
   SPIChip::SPIWriteByteRead(MAX31865_CONFIG_WRITE, confreg, 2, readbuffer);
}

/// Read high fault limit from chip.
uint16_t Max31865::GetHighFault()
{
   uint8_t readbuffer[ 3 ];
   uint16_t highfault = 0x0000;

   SPIChip::SPIWriteByteRead(MAX31865_HIGHFAULT_READ, 0x00, 3, readbuffer);
   highfault = ( (uint16_t)readbuffer[ 1 ] ) << 8;
   highfault |= (uint16_t)readbuffer[ 2 ]; 

   return highfault;
}

/// Set high fault register 16-bit value.
void Max31865::SetHighFault(uint16_t highfault)
{
   SPIChip::SPIWriteWord(MAX31865_HIGHFAULT_WRITE, highfault);
}


/// Read low fault limit from chip.
uint16_t Max31865::GetLowFault()
{
   uint8_t readbuffer[ 3 ];
   uint16_t lowfault = 0x0000;

   SPIChip::SPIWriteByteRead(MAX31865_LOWFAULT_READ, 0x00, 3, readbuffer);
   lowfault = ( (uint16_t)readbuffer[ 1 ] ) << 8;
   lowfault |= (uint16_t)readbuffer[ 2 ]; 

   return lowfault;
}

/// Set low fault register 16-bit value.
void Max31865::SetLowFault(uint16_t lowfault)
{
   SPIChip::SPIWriteWord(MAX31865_LOWFAULT_WRITE, lowfault);
}

/// Read fault status byte.
bool Max31865::IsFault()
{
   uint8_t readbuffer[ 2 ];
   uint8_t rtdlsb = 0x00;
   bool isfault = false;

   SPIChip::SPIWriteByteRead(MAX31865_RTDLSB_READ, 0x00, 2, readbuffer);
   rtdlsb = (uint8_t)readbuffer[ 1 ]; 
   if( (rtdlsb & 0x01) == 0x01 ) isfault = true;

   return isfault;
}

/// Read fault status byte.
uint8_t Max31865::GetFaultStatusByte()
{
   uint8_t readbuffer[ 2 ];
   uint8_t faultstatus = 0x00;

   SPIChip::SPIWriteByteRead(MAX31865_FAULTSTATUS_READ, 0x00, 2, readbuffer);
   faultstatus = (uint8_t)readbuffer[ 1 ]; 

   return faultstatus;
}

/// Read RTD resistance value scaled from reference resistor Rref.
void Max31865::ReadResistance()
{
   uint8_t readbuffer[ 3 ];

   SPIChip::SPIWriteByteRead(MAX31865_RTD_READ, 0x00, 3, readbuffer);
   RTD = ( (uint16_t)readbuffer[ 1 ] ) << 8;
   RTD |= (uint16_t)readbuffer[ 2 ]; 
   Resistance = RTD * Rref / 65536.0;
}

// Callendar-Van Dusen equation:
// R(T) = R0 ( 1 + a T + b T^2 + c(T - 100) T^3 ) 
double R( double T)
{
  const double a = 3.90830e-3;
  const double b = -5.77500e-7;
  const double c = -4.18301e-12;
  double res = -999;
  if( T < 0 ) res = 100*( 1 + a*T + b*T*T + c*(T - 100)*T*T*T ); 
  else res = 100*( 1 + a*T + b*T*T ); 
  return res;
}

// Callendar-Van Dusen equation derivative:
// R'(T) = R0 ( a + 2 b T + 3 c (T - 100) T^2 + c T^3 ) 
double DR( double T)
{
  const double a = 3.90830e-3;
  const double b = -5.77500e-7;
  const double c = -4.18301e-12;
  double res = -999;

  if( T < 0 ) res = 100*( a + 2*b*T + 3*c*(T - 100)*T*T + c*T*T*T );  
  else res = 100*( a + 2*b*T );  

  return res;
}

/// Read RTD value and estimate temperature from Callendar-Van Dusen equation.
void Max31865::CalcTemperature()
{
   const double tolerance = 1e-7;
   const double epsilon = 1e-14;
   const int maxiterations = 20;

   double T = -9999, T0 = 0, T1 = 0;
   double y, yprime;

// approximate temperature [C]
   T0 = (double)(RTD/64 - 256);

/// https://en.wikipedia.org/wiki/Newton%27s_method
   bool solution = false;
   for( int i = 0; i < maxiterations ; i++ )
   {
      y = R( T0 ) - Resistance;
      yprime = DR( T0 );

      fprintf(stderr, SD_DEBUG "%4d %+9.6f %+9.6f %+9.6f %+9.6f\n", i, T0, T1, y, yprime);

      if( abs( yprime ) < epsilon ) break;

      T1 = T0 - y/yprime;

      if( abs( T1 - T0 ) <= tolerance * abs( T1 ) )
      {
         solution = true;
         break;
      }

      T0 = T1;
   }

   if( solution ) T = T1; 

   Temperature = T;
}

