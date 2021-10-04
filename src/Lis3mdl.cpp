/**************************************************************************
 * 
 * Lis3mdl class member functions for configuration and reading with I2C. 
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
 * Fri Sep 10 16:30:57 CDT 2021
 * Edit: Mon Sep 13 19:05:44 CDT 2021
 *
 * Jaakko Koivuniemi
 **/

#include "Lis3mdl.hpp"
#include <unistd.h>

using namespace std;

Lis3mdl::~Lis3mdl() { };

/// Set TEMP_EN bit in register CTRL_REG1. 
void Lis3mdl::TempEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Clear TEMP_EN bit in register CTRL_REG1.
void Lis3mdl::TempDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Read bits OM[1:0] in register CTRL_REG1.
uint8_t Lis3mdl::GetXYOpMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;
  reg = reg >> 5;

  return reg;	
}

/// Modify bits OM[1:0] in register CTRL_REG1.
void Lis3mdl::SetXYOpMode(uint8_t OpModeXY)
{
  uint8_t reg = 0;

  OpModeXY &= 0x03;
  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x9F;
  reg |=  OpModeXY << 5;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Read bits DO[2:0] in register CTRL_REG1.
uint8_t Lis3mdl::GetDataRate()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x1C;
  reg = reg >> 2;

  return reg;
}

/// Modify bits DO[2:0] in register CTRL_REG1.
void Lis3mdl::SetDataRate(uint8_t DataRate)
{
  uint8_t reg = 0;

  DataRate &= 0x07;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xE3;
  reg |=  DataRate << 2;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Set bit FAST_ODR in register CTRL_REG1. 
void Lis3mdl::FastDataEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}


/// Clear bit FAST_ODR in register CTRL_REG1. 
void Lis3mdl::FastDataDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Set bit ST in register CTRL_REG1. 
void Lis3mdl::SelfTestEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x01;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Clear bit ST in register CTRL_REG1. 
void Lis3mdl::SelfTestDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFE;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, reg, address, buffer, error);
}

/// Read bits FS[0:1] in register CTRL_REG2.
uint8_t Lis3mdl::GetFullScale()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x60;
  reg = reg >> 5;
      
  return reg;
}

/// Write bits FS[0:1] in register CTRL_REG2.
void Lis3mdl::SetFullScale(uint8_t FullScale)
{
  uint8_t reg = 0;

  FullScale &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x9F;
  reg |= FullScale << 5;
  reg &= 0x6C;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG2, reg, address, buffer, error);

  switch( FullScale )
  {
    case 0 : 
      Gain = 6842;
      break;
    case 1 :
      Gain = 3421;
      break;
    case 2 :
      Gain = 2281;
      break;
    case 3 :
      Gain = 1711;
      break;
  }
}

/// Set bit REBOOT in register CTRL_REG2. 
void Lis3mdl::Reboot()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80;
  reg &= 0x6C;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit SOFT_RST in register CTRL_REG2. 
void Lis3mdl::SoftReset()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40;
  reg &= 0x6C;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit LP in register CTRL_REG3.
void Lis3mdl::LowPowerEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20;
  reg &= 0x27;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit LP in register CTRL_REG3.
void Lis3mdl::LowPowerDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x27;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Read bits MD[1:0] from register CTRL_REG3.
uint8_t Lis3mdl::GetOpMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x03;
      
  return reg;
}

/// Write bits MD[1:0] from register CTRL_REG3.
void Lis3mdl::SetOpMode(uint8_t OpMode)
{
  uint8_t reg = 0;

  OpMode &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= OpMode;
  reg &= 0x27;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Set bits MD1 and MD0 in register CTRL_REG3.
void Lis3mdl::PowerDown()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x03;
  reg &= 0x27;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bits MD1 and MD0 in register CTRL_REG3.
void Lis3mdl::ContinuousMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x24;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit MD1 and set MD0 in register CTRL_REG3.
void Lis3mdl::SingleConversionMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x26;
  reg |= 0x01;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG3, reg, address, buffer, error);
}

/// Read bits OMZ[1:0] from register CTRL_REG4.
uint8_t Lis3mdl::GetZOpMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x0C;
  reg = reg >> 2;

  return reg;
}

/// Write bits OMZ[1:0] from register CTRL_REG4.
void Lis3mdl::SetZOpMode(uint8_t ZOpMode)
{
  uint8_t reg = 0;

  ZOpMode &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x02;
  reg |= ZOpMode <<  2;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG4, reg, address, buffer, error);
}

/// Clear bit BLE in register CTRL_REG4.
void Lis3mdl::BigEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x0C;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG4, reg, address, buffer, error);
}

/// Set bit BLE in register CTRL_REG4.
void Lis3mdl::LittleEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;
  reg &= 0x0E;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG4, reg, address, buffer, error);
}

/// Set bit FAST_READ in register CTRL_REG5..
void Lis3mdl::FastReadEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80;
  reg &= 0xC0;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG5, reg, address, buffer, error);
}

/// Clear bit FAST_READ in register CTRL_REG5..
void Lis3mdl::FastReadDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x40;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG5, reg, address, buffer, error);
}

/// Set bit BDU in register CTRL_REG5.
void Lis3mdl::BlockDataEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40;
  reg &= 0xC0;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG5, reg, address, buffer, error);
}

/// Clear bit BDU in register CTRL_REG5.
void Lis3mdl::BlockDataDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG5, reg, address, buffer, error);
}

/// Read register STATUS_REG.
uint8_t Lis3mdl::GetStatus()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Read register INT_CFG.
uint8_t Lis3mdl::GetIntConfig()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_INT_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
      
  return reg;
}

/// Set register INT_CFG.
void Lis3mdl::SetIntConfig(uint8_t IntConf)
{
  IntConf &= 0xEF;
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_INT_CFG, IntConf, address, buffer, error);
}

/// Read register INT_SRC.
uint8_t Lis3mdl::GetIntSource()
{
  uint8_t reg = 0;
      
  I2Chip::I2cWriteUInt8(LIS3MDL_INT_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
      
  return reg;
}

/// Write register INT_SRC.
void Lis3mdl::SetIntSource(uint8_t IntSource)
{
  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_INT_SRC, IntSource, address, buffer, error);
}

/// Get interrupt threshold 0 - 65535.
uint16_t Lis3mdl::GetIntThreshold()
{
  uint16_t data;

  I2Chip::I2cWriteUInt8(LIS3MDL_INT_THS_L | LIS3MDL_MULTI_RW, address, buffer, error);
  data = I2Chip::I2cReadUInt16(address, buffer, error);

  return data;
}

/// Set interrupt threshold 0 - 65535.
void Lis3mdl::SetIntThreshold(uint16_t IntThreshold)
{
  I2Chip::I2cWriteRegisterUInt16(LIS3MDL_INT_THS_L | LIS3MDL_MULTI_RW, IntThreshold, address, buffer, error);
}

/// Read chip WHO_AM_I register and return true if 0x3D
bool Lis3mdl::WhoAmI()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_WHO_AM_I, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
      
  return (reg == 0x3D);
}

/// Read chip OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L and OUT_Z_H registers and return true if success.
bool Lis3mdl::ReadB()
{
  bool success = true;

  Lis3mdl::ReadBx();

  if( error != 0 )
  {
    success = false;
  }
  else
  { 
    Lis3mdl::ReadBy();

    if( error != 0 )
    {
      success = false;
    }
    else
    { 
      Lis3mdl::ReadBz();
    }   
  }

  return success;
}

/// Read chip OUT_X_L and OUT_X_H registers and return true if success.
bool Lis3mdl::ReadBx()
{
  uint8_t reg = 0;
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS3MDL_OUT_X_H , address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    outX = I2Chip::I2cReadUInt8(address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outX = outX << 8;

      I2Chip::I2cWriteUInt8(LIS3MDL_OUT_X_L , address, buffer, error);

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
          outX |= reg;

	  if( outX > 0x7FFF )
          {
            outX ^= 0xFFFF;
            outX++;
            Bx = -100 * outX / Gain;
          }
          else
          {
            Bx = 100 * outX / Gain;
          }	
  	}
      }
    }
  }

  return success;
}

/// Read chip OUT_Y_L and OUT_Y_H registers and return true if success.
bool Lis3mdl::ReadBy()
{
  uint8_t reg = 0;
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS3MDL_OUT_Y_H , address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    outY = I2Chip::I2cReadUInt8(address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outY = outY << 8;

      I2Chip::I2cWriteUInt8(LIS3MDL_OUT_Y_L , address, buffer, error);

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
          outY |= reg;

	  if( outY > 0x7FFF )
          {
            outY ^= 0xFFFF;
            outY++;
            By = -100 * outY / Gain;
          }
          else
          {
            By = 100 * outY / Gain;
          }	
  	}
      }
    }
  }

  return success;
}

/// Read chip OUT_Z_L and OUT_Z_H registers and return true if success.
bool Lis3mdl::ReadBz()
{
  uint8_t reg = 0;
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS3MDL_OUT_Z_H , address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    outZ = I2Chip::I2cReadUInt8(address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outZ = outZ << 8;

      I2Chip::I2cWriteUInt8(LIS3MDL_OUT_Z_L , address, buffer, error);

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
          outZ |= reg;

	  if( outZ > 0x7FFF )
          {
            outZ ^= 0xFFFF;
            outZ++;
            Bz = -100 * outZ / Gain;
          }
          else
          {
            Bz = 100 * outZ / Gain;
          }	
  	}
      }
    }
  }

  return success;
}

/// Read STATUS_REG and test if bit ZYXDA bit is set.
bool Lis3mdl::NewDataXYZ()
{
  bool newdata = false;
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x08;
  if( reg == 0x00) newdata = false; else newdata = true;
  
  return newdata;
}

/// Read STATUS_REG and test if bit ZYXOR bi it set.
bool Lis3mdl::OverRunXYZ()
{
  bool overrun = false;
  uint8_t reg = 0;
	
  I2Chip::I2cWriteUInt8(LIS3MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  if( reg == 0x00) overrun = false; else overrun = true;
  
  return overrun;
}

/// Set bit ST in CTRL_REG1 following AN4602.
bool Lis3mdl::SelfTest()
{
  bool success = false;
  uint16_t outZ = 0;

  I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, 0x1C, address, buffer, error);
  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, 0x40, address, buffer, error);
    if( error != 0 )
    {
      success = false;
    }
    else
    {
      usleep( 20000 );
      I2Chip::I2cWriteRegisterUInt8(LIS3MDL_CTRL_REG1, 0x00, address, buffer, error);
    }

  }

  return success;
}
