/**************************************************************************
 * 
 * Ltr390uv class member functions for configuration and reading with I2Chip. 
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
 * Sun 01 May 2022 06:41:35 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "Ltr390uv.hpp"

using namespace std;

Ltr390uv::~Ltr390uv() { };

/// Set bit SW Reset in MAIN_CTRL register.
void Ltr390uv::Reset()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_CTRL, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_MAIN_CTRL, reg, address, buffer, error);
}

/// Clear bit UVS_Mode in MAIN_CTRL register.
void Ltr390uv::AmbientLightMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_CTRL, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_MAIN_CTRL, reg, address, buffer, error);
}

/// Set bit UVS_Mode in MAIN_CTRL register.
void Ltr390uv::UltravioletMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_CTRL, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_MAIN_CTRL, reg, address, buffer, error);
}

/// Clear ALS/UVS Enable bit in MAIN_CTRL register.
void Ltr390uv::LightDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_CTRL, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_MAIN_CTRL, reg, address, buffer, error);
}

/// Set ALS/UVS Enable bit in MAIN_CTRL register.
void Ltr390uv::LightEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_CTRL, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_MAIN_CTRL, reg, address, buffer, error);
}

/// Read bits [6:4] from register ALS_UVS_MEAS_RATE.
uint8_t Ltr390uv::GetResolution()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_ALS_UVS_MEAS_RATE, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x70;
  
  return ( reg >> 4 );
}

/// Set bits [6:4] in register ALS_UVS_MEAS_RATE.
void Ltr390uv::SetResolution(uint8_t Resolution)
{
  uint8_t reg = 0;
  Resolution &= 0x07;

  if( Resolution < 6 )
  {
    I2Chip::I2cWriteUInt8(LTR390UV_ALS_UVS_MEAS_RATE, address, buffer, error);
    reg = I2Chip::I2cReadUInt8(address, buffer, error);
    reg &= 0x07;
    reg |= Resolution << 4;

    I2Chip::I2cWriteRegisterUInt8(LTR390UV_ALS_UVS_MEAS_RATE, reg, address, buffer, error);

// set IntTime here
    switch( Resolution )
    {
      case 5:
        IntTime = 0.125;
        break;
      case 4:
        IntTime = 0.25;
	break;
      case 3:
        IntTime = 0.5;
	break;
      case 2:
        IntTime = 1.0;
	break;
      case 1:
        IntTime = 2.0;
        break;
      case 0:
        IntTime = 4.0;
	break;
    }
  }
}

/// Read bits [2:0] from register ALS_UVS_MEAS_RATE.
uint8_t Ltr390uv::GetMeasRate()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_ALS_UVS_MEAS_RATE, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x07;
  
  return reg;
}

/// Set bits [2:0] in register ALS_UVS_MEAS_RATE.
void Ltr390uv::SetMeasRate(uint8_t MeasRate)
{
  uint8_t reg = 0;
  MeasRate &= 0x07;

  I2Chip::I2cWriteUInt8(LTR390UV_ALS_UVS_MEAS_RATE, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x77;
  reg |= MeasRate;

  I2Chip::I2cWriteRegisterUInt8(LTR390UV_ALS_UVS_MEAS_RATE, reg, address, buffer, error);
}

/// Read bits [2:0] from register ALS_UVS_GAIN.
uint8_t Ltr390uv::GetGain()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_ALS_UVS_GAIN, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x07;
  
  return reg;
}

/// Set bits [2:0] in register ALS_UVS_GAIN.
void Ltr390uv::SetGain(uint8_t Gain)
{
  Gain &= 0x07;

  if( Gain < 5 )
  {
    I2Chip::I2cWriteRegisterUInt8(LTR390UV_ALS_UVS_GAIN, Gain, address, buffer, error);

    // set AlsGain here
    switch( Gain )
    {
      case 0:
        AlsGain = 1;
        break;
      case 1:
        AlsGain = 3;
        break;
      case 2:
        AlsGain = 6;
        break;
      case 3:
        AlsGain = 9;
        break;
      case 4:
        AlsGain = 18;
        break;
    }
  }
}

/// Read bits [7:0] from register PART_ID.
uint8_t Ltr390uv::GetID()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_PART_ID, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  
  return reg;
}

/// Read bits [5:3] from register MAIN_STATUS.
uint8_t Ltr390uv::GetStatus()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_STATUS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  
  return reg;
}

/// Read bit Power-On Status from register MAIN_STATUS.
bool Ltr390uv::IsPowerOn()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_STATUS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x20;
 
  return ( reg == 0x20 );
}

/// Read bit ALS/UVS Interrupt Status from register MAIN_STATUS.
bool Ltr390uv::IsInterrupt()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_STATUS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x10;
 
  return ( reg == 0x10 );
}

/// Read bit ALS/UVS Data Status from register MAIN_STATUS.
bool Ltr390uv::NewData()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_MAIN_STATUS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x08;
 
  return ( reg == 0x08 );
}

/// Read registers ALS_DATA_0, ALS_DATA_1 and ALS_DATA_2.
bool Ltr390uv::ReadAmbientLight()
{
  bool success = true;
  uint8_t reg = 0;
  uint32_t data = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_ALS_DATA_0, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    reg = I2Chip::I2cReadUInt8(address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      data = (uint32_t)reg;

      I2Chip::I2cWriteUInt8(LTR390UV_ALS_DATA_1, address, buffer, error);

      if( error != 0 )
      {
        success = false;
      }
      else
      {
        reg = I2Chip::I2cReadUInt8(address, buffer, error);

	if( error != 0 )
        {
          success = false;
	}
        else
        {
          data |= ( (uint32_t)reg ) << 8;

	  I2Chip::I2cWriteUInt8(LTR390UV_ALS_DATA_2, address, buffer, error);

          if( error != 0 )
          {
            success = false;
          }
          else
          {
            reg = I2Chip::I2cReadUInt8(address, buffer, error);

            data |= ( (uint32_t)reg ) << 16;
            AlsData = data;
            Ambientlight = 0.6 * AlsData * Wfact / ( AlsGain * IntTime );
	  }
	}
      }
    }
  }

  return success;
}

/// Read registers UVS_DATA_0, UVS_DATA_1 and UVS_DATA_2.
bool Ltr390uv::ReadUltraviolet()
{
  bool success = true;
  uint8_t reg = 0;
  uint32_t data = 0;

  I2Chip::I2cWriteUInt8(LTR390UV_UVS_DATA_0, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    reg = I2Chip::I2cReadUInt8(address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      data = (uint32_t)reg;

      I2Chip::I2cWriteUInt8(LTR390UV_UVS_DATA_1, address, buffer, error);

      if( error != 0 )
      {
        success = false;
      }
      else
      {
        reg = I2Chip::I2cReadUInt8(address, buffer, error);
    
        if( error != 0 )
        {
          success = false;
	}
        else
        {
          data |= ( (uint32_t)reg ) << 8;

          I2Chip::I2cWriteUInt8(LTR390UV_UVS_DATA_2, address, buffer, error);

          if( error != 0 )
          {
            success = false;
          }
          else
          {
            reg = I2Chip::I2cReadUInt8(address, buffer, error);

            data |= ( (uint32_t)reg ) << 16;
            UviData = data;
            UVI = UviData * Wfact / UVsensitivity;
	  }
	}
      }
    }
  }

  return success;
}

/// Set bits [5:4] to 01 in INT_CFG register.
void Ltr390uv::IntAmbientLight()
{
}

/// Set bits [5:4] to 11 in INT_CFG register.
void Ltr390uv::IntUltraviolet()
{
}

/// Set bit 2 in INT_CFG register.
void Ltr390uv::IntEnable()
{
}

/// Clear bit 2 in INT_CFG register.
void Ltr390uv::IntDisable()
{
}

/// Read bits [7:4] from INT Persist register.
uint8_t Ltr390uv::GetIntPersist()
{
  return 0;
}

/// Set bits [7:4] in INT Persist register.
void Ltr390uv::SetIntPersist(uint8_t IntPersist)
{
}

/// Read registers UVS/ALS_THRES_LOW_0, UVS/ALS_THRES_LOW_1 and UVS/ALS_THRES_LOW_2.
uint32_t Ltr390uv::GetThrsLow()
{
}

/// Write registers UVS/ALS_THRES_LOW_0, UVS/ALS_THRES_LOW_1 and UVS/ALS_THRES_LOW_2.
void Ltr390uv::SetThrsLow(uint32_t ThrsLow)
{
}

/// Read registers UVS/ALS_THRES_UP_0, UVS/ALS_THRES_UP_1 and UVS/ALS_THRES_UP_2.
uint32_t Ltr390uv::GetThrsUp()
{
}

/// Write registers UVS/ALS_THRES_UP_0, UVS/ALS_THRES_UP_1 and UVS/ALS_THRES_UP_2.
void Ltr390uv::SetThrsUp(uint32_t ThrsUp)
{
}

