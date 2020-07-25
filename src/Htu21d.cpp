/**************************************************************************
 * 
 * Htu21d class member functions for configuration and reading with I2Chip. 
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
 * Fri Jul 24 09:44:39 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "Htu21d.hpp"

using namespace std;

Htu21d::~Htu21d() { };


// Read user register value.
uint8_t Htu21d::GetUserRegister()
{
  uint8_t reg = 0;

  I2cWriteUInt8(HTU21D_READ_USER_REG, HTU21D_ADDRESS, buffer, error);
  reg = I2cReadUInt8(HTU21D_ADDRESS, buffer, error);

  return reg;
}

// Start on-chip heater.
void Htu21d::HeaterOn()
{
  uint8_t reg = 0;

  reg = Htu21d::GetUserRegister();

  reg |= 0x04;

  I2cWriteRegisterUInt8(HTU21D_WRITE_USER_REG, reg, HTU21D_ADDRESS, buffer, error);
}

// Stop on-chip heater.
void Htu21d::HeaterOff()
{
  uint8_t reg = 0;

  reg = Htu21d::GetUserRegister();

  reg &= 0xFB;

  I2cWriteRegisterUInt8(HTU21D_WRITE_USER_REG, reg, HTU21D_ADDRESS, buffer, error);
}

// Soft reset.
void Htu21d::Reset()
{
  I2cWriteUInt8(HTU21D_SOFT_RESET, HTU21D_ADDRESS, buffer, error);
}

// Set two resolution bits with 0 - 3.
void Htu21d::SetResolution(uint8_t Resolution)
{
  uint8_t reg = 0;

  reg = Htu21d::GetUserRegister();

  if( ( Resolution & 0x02 ) == 0x02 ) reg |= 0x80; else reg &= 0x7F;
  if( ( Resolution & 0x01 ) == 0x01 ) reg |= 0x01; else reg &= 0xFE;

  I2cWriteRegisterUInt8(HTU21D_WRITE_USER_REG, reg, HTU21D_ADDRESS, buffer, error);

}

// Trigger temperature measurement.
void Htu21d::TriggerTemperature()
{
  I2cWriteUInt8(HTU21D_TRIG_TEMP_MEAS, HTU21D_ADDRESS, buffer, error);
}

// Trigger humidity measurement.
void Htu21d::TriggerHumidity()
{
  I2cWriteUInt8(HTU21D_TRIG_HUM_MEAS, HTU21D_ADDRESS, buffer, error);
}

// Read chip temperature register and return true if success.
bool Htu21d::ReadTemperature()
{
  uint32_t div = 0x988000; // 0x131 << 15 (x^8 + x^5 + x^4 + 1)
  uint32_t testbit = 0x800000; 
  uint32_t data = 0;

  I2cReadBytes(3, HTU21D_ADDRESS, buffer, error);

  if( error != 0 )
  {
    return false;
  }
  else
  {
    // check that status bit is '0'
    if( ( buffer[ 1 ] & 0x02 ) == 0x02 )
    {
      error = -20;
      return false;
    }
    else
    {
      tadc = (uint16_t)( buffer[ 0 ] << 8 | buffer[ 1 ] );
      checksum = buffer[ 2 ];

      data = (uint32_t) ( buffer[ 0 ] << 16 | buffer[ 1 ] << 8 | buffer[ 2 ] );

      // calculate CRC
      while( data > 0 && div >= 0x131 )
      {
//        fprintf(stderr, SD_DEBUG "%06x %06x %06x\n", data, div, testbit);
      	if( ( testbit & data ) == testbit ) data ^= div;
        testbit = testbit >> 1;
        div = div >> 1;
      }

      if( data == 0 ) crc = true; else crc = false;

      if( crc ) Temperature = -46.85 + 175.72 * ( (double)( tadc & 0xFFFC ) ) / 65536.0;

      return crc;
    }
  }
}



// Read chip humidity register and return true if success.
bool Htu21d::ReadHumidity()
{
  uint32_t div = 0x988000; // 0x131 << 15 (x^8 + x^5 + x^4 + 1)
  uint32_t testbit = 0x800000; 
  uint32_t data = 0;

  I2cReadBytes(3, HTU21D_ADDRESS, buffer, error);

  if( error != 0 )
  {
    return false;
  }
  else
  {
    // check that status bit is '1'
    if( ( buffer[ 1 ] & 0x02 ) == 0x00 )
    {
      error = -21;
      return false;
    }
    else
    {
      hadc = (uint16_t)( buffer[ 0 ] << 8 | buffer[ 1 ] );
      checksum = buffer[ 2 ];

      data = (uint32_t) ( buffer[ 0 ] << 16 | buffer[ 1 ] << 8 | buffer[ 2 ] );

      // calculate CRC
      while( data > 0 && div >= 0x131 )
      {
//        fprintf(stderr, SD_DEBUG "%06x %06x %06x\n", data, div, testbit);
      	if( ( testbit & data ) == testbit ) data ^= div;
        testbit = testbit >> 1;
        div = div >> 1;
      }

      if( data == 0 ) crc = true; else crc = false;

      if( crc ) Humidity = -6.0 + 125.0 * ( (double)( hadc & 0xFFFC ) ) / 65536.0;
      return crc;

    }
  }
}

// Return true if supply voltage VDD < 2.25 V.
bool Htu21d::IsEndBattery()
{
  uint8_t reg = 0;

  reg = Htu21d::GetUserRegister();

  if( ( 0x40 & reg ) == 0x40 ) return true; else return false;
}
