/**************************************************************************
 * 
 * Bmp280 class member functions for configuration and reading with I2C. 
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
 * Tue 07 Jul 2020 01:26:09 PM CDT
 * Edit: Wed 08 Jul 2020 04:43:18 PM CDT
 *
 * Jaakko Koivuniemi
 **/

#include "Bmp280.hpp"

using namespace std;

Bmp280::~Bmp280() { };


/// Pressure in Pascal from last measurement. 
double Bmp280::GetPressure() 
{ 
  double p = Pressure;
  p /= 256.0; 
  return  p; 
} 

/// Temperature in Celsius from last measurement. 
double Bmp280::GetTemperature() 
{ 
  double T = Temperature;
  T /= 100.0;
  return T; 
}

// Get ID register value.
uint8_t Bmp280::GetID()
{
  uint8_t ID = 0;

  I2Chip::I2cWriteUInt8(BMP280_ID_REG, address, buffer, error);
  ID = I2Chip::I2cReadUInt8(address, buffer, error);

  return ID;
}

// Get status register value.
uint8_t Bmp280::GetStatus()
{
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BMP280_STATUS_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  return status;
}

// Get control measurement register value.
uint8_t Bmp280::GetControlMeasurement()
{
  uint8_t ctrlmeas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrlmeas = I2Chip::I2cReadUInt8(address, buffer, error);

  return ctrlmeas;
} 

// Get configuration register value.
uint8_t Bmp280::GetConfig()
{
  uint8_t config = 0;

  I2Chip::I2cWriteUInt8(BMP280_CONFIG_REG, address, buffer, error);
  config = I2Chip::I2cReadUInt8(address, buffer, error);

  return config;
}

// Get two power mode bits 0 - 3.
uint8_t Bmp280::GetMode()
{
  uint8_t ctrl_meas = 0;

  ctrl_meas = Bmp280::GetControlMeasurement();
  ctrl_meas &= 0x03;

  return ctrl_meas;
}

// Set pointer register.
void Bmp280::SetPointer(uint8_t PointerReg)
{
  I2Chip::I2cWriteUInt8(PointerReg, address, buffer, error);
}

// Set control measurement register value.
void Bmp280::SetControlMeasurement(uint8_t CtrlMeas)
{
  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, CtrlMeas, address, buffer, error);
}

// Set configuration register value.
void Bmp280::SetConfig(uint8_t Config)
{
  I2Chip::I2cWriteRegisterUInt8(BMP280_CONFIG_REG, Config, address, buffer, error);
}

// Set three temperature oversampling bits with 0 - 7.
void Bmp280::SetTOverSample(uint8_t TOverSample)
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  TOverSample &= 0x07;
  TOverSample = TOverSample << 5;
  ctrl_meas &= 0x1F;
  ctrl_meas |= TOverSample;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Set three pressure oversampling bits with 0 - 7.
void Bmp280::SetPOverSample(uint8_t POverSample)
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  POverSample &= 0x07;
  POverSample = POverSample << 2;
  ctrl_meas &= 0xE3;
  ctrl_meas |= POverSample;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Set two power mode bits with 0 - 3.
void Bmp280::SetMode(uint8_t Mode)
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Mode &= 0x03;
  ctrl_meas &= 0xFC;
  ctrl_meas |= Mode;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);

}

// Set three inactive duration bits with 0 - 7.
void Bmp280::SetStandby(uint8_t Standby)
{
  uint8_t config = 0;

  I2Chip::I2cWriteUInt8(BMP280_CONFIG_REG, address, buffer, error);
  config = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Standby &= 0x07;
  Standby = Standby << 5;
  config &= 0x1F;
  config |= Standby;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CONFIG_REG, config, address, buffer, error);

}

// Set three filter bits with 0 - 7.
void Bmp280::SetFilter(uint8_t Filter)
{
  uint8_t config = 0;

  I2Chip::I2cWriteUInt8(BMP280_CONFIG_REG, address, buffer, error);
  config = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Filter &= 0x07;
  Filter = Filter << 2;
  config &= 0xE3;
  config |= Filter;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CONFIG_REG, config, address, buffer, error);
}

// Measurement going on.
bool Bmp280::IsMeasuring()
{
  bool measuring = false;
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BMP280_STATUS_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  if( (status & 0x08) == 0x08 ) measuring = true; else measuring = false;

  return measuring;
}

// Update going on.
bool Bmp280::IsUpdate()
{
  bool update = false;
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BMP280_STATUS_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  if( (status & 0x01) == 0x01 ) update = true; else update = false;

  return update;
}

// Power on reset.
void Bmp280::Reset()
{
  I2Chip::I2cWriteRegisterUInt8(BMP280_RESET_REG, 0xB6, address, buffer, error);
}

// Sleep mode Mode[1:0] = 00.
void Bmp280::Sleep()
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  ctrl_meas &= 0xFC;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Normal mode Mode[1:0] = 11.
void Bmp280::Normal()
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  ctrl_meas |= 0x03;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Forced mode Mode[1:0] = 01.
void Bmp280::Forced()
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BMP280_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);

  ctrl_meas &= 0xFD;
  ctrl_meas |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(BMP280_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Read chip calibration data and return true if success.
bool Bmp280::GetCalibration()
{
  I2Chip::I2cWriteUInt8(BMP280_DIG_T1_REG, address, buffer, error);

  if( error != 0 )
  {
    return false;
  }
  else
  {
    I2Chip::I2cReadBytes(24, address, buffer, error);

    if( error != 0 )
    {
      return false;
    }
    else
    {
      dig_T1 = (uint16_t)( (buffer[ 1 ] << 8) | buffer[ 0 ] );
      dig_T2 = (int16_t)( (buffer[ 3 ] << 8) | buffer[ 2 ] );
      dig_T3 = (int16_t)( (buffer[ 5 ] << 8) | buffer[ 4 ] );
      dig_P1 = (uint16_t)( (buffer[ 7 ] << 8) | buffer[ 6 ] );
      dig_P2 = (int16_t)( (buffer[ 9 ] << 8) | buffer[ 8 ] );
      dig_P3 = (int16_t)( (buffer[ 11 ] << 8) | buffer[ 10 ] );
      dig_P4 = (int16_t)( (buffer[ 13 ] << 8) | buffer[ 12 ] );
      dig_P5 = (int16_t)( (buffer[ 15 ] << 8) | buffer[ 14 ] );
      dig_P6 = (int16_t)( (buffer[ 17 ] << 8) | buffer[ 16 ] );
      dig_P7 = (int16_t)( (buffer[ 19 ] << 8) | buffer[ 18 ] );
      dig_P8 = (int16_t)( (buffer[ 21 ] << 8) | buffer[ 20 ] );
      dig_P9 = (int16_t)( (buffer[ 23 ] << 8) | buffer[ 22 ] );
    }
  }

  return true;
}

// Read chip temperature and pressure registers and do conversion.
int Bmp280::Measure()
{
  int32_t tadc = 0, var1 = 0, var2 = 0;
  int64_t p = 0, pvar1 = 0, pvar2 = 0;
  int32_t padc = 0;
  int32_t tfine;

  I2Chip::I2cWriteUInt8(BMP280_PRESS_MSB_REG, address, buffer, error);
  if( error != 0 )
  {
    return error;
  }
  else
  {
    I2Chip::I2cReadBytes(6, address, buffer, error);
    if( error != 0 )
    {
      return error;
    }
    else
    {
      tadc =  buffer[ 3 ] << 12;
      tadc |= (int32_t)( buffer[ 4 ] << 4 ); 
      tadc |= (int32_t)( buffer[ 5 ] >> 4 ); 

      var1 = ( ( (tadc >> 3) - ((int32_t)dig_T1 << 1) ) * (int32_t)dig_T2 ) >> 11; 
      var2 = ( ( ( ( (tadc >> 4) - ((int32_t)dig_T1)) * ( (tadc >> 4) - ((int32_t)dig_T1) ) ) >> 12 ) * ((int32_t)dig_T3) ) >> 14;
      tfine = var1 + var2;
      Temperature = (tfine * 5 + 128) >> 8; 

      padc =  buffer[ 0 ] << 12;
      padc |= (int32_t)( buffer[ 1 ] << 4 ); 
      padc |= (int32_t)( buffer[ 2 ] >> 4 ); 

      pvar1 = ((int64_t)tfine) - 128000;
      pvar2 = pvar1 * pvar1 * (int64_t)dig_P6;
      pvar2 = pvar2 + ( (pvar1 * (int64_t)dig_P5) << 17 );
      pvar2 = pvar2 + ( ( (int64_t)dig_P4) << 35 );
      pvar1 = ( (pvar1 * pvar1 * (int64_t)dig_P3) >> 8 ) + ( (pvar1 * (int64_t)dig_P2 ) << 12 );
      pvar1 = ( ( ( ( (int64_t)1) << 47) + var1) ) * ( (int64_t)dig_P1 ) >> 33;

      if( pvar1 == 0 )
      {
	Pressure = 0;
      }
      else
      {
        p = 1048576 - padc;
        p = ( ( (p << 31) - pvar2) * 3125 ) / pvar1;
        pvar1 = ( ( (int64_t)dig_P9) * ( p >> 13 ) * ( p >> 13 ) ) >> 25;
        pvar2 = ( ( (int64_t)dig_P8) * p ) >> 19;
        p = ( (p + pvar1 + pvar2) >> 8) + ( ( (int64_t)dig_P7 ) << 4 );

        Pressure = (uint32_t)p;
      }
    }
  }

  return 0;
}

