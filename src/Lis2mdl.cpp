/**************************************************************************
 * 
 * Lis2mdl class member functions for configuration and reading with I2C. 
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
 * Sat 26 Mar 2022 10:50:20 AM CET
 * Edit: Fri Apr 22 17:20:05 CDT 2022
 *
 * Jaakko Koivuniemi
 **/

#include "Lis2mdl.hpp"
#include <unistd.h>

using namespace std;

Lis2mdl::~Lis2mdl() { };

/// Read registers OFFSET_X_REG_L and OFFSET_X_REG_H.
int16_t Lis2mdl::GetXOffset()
{
  int16_t data = -9999;

  I2Chip::I2cWriteUInt8(LIS2MDL_OFFSET_X_REG_L | LIS2MDL_MULTI_RW, address, buffer, error);
  I2Chip::I2cReadBytes(2, address, buffer, error);

  if( error == 0 )
  {
    data = (int16_t)( buffer[ 0 ] ) | ( buffer[ 1 ] << 8 );
  }

  return data;
}

/// Write registers OFFSET_X_REG_L and OFFSET_X_REG_H.
void Lis2mdl::SetXOffset(int16_t offset)
{
  int16_t byte_swapped = ( offset << 8 ) | ( ( offset >> 8 ) & 0x00FF );

  I2Chip::I2cWriteRegisterUInt16(LIS2MDL_OFFSET_X_REG_L | LIS2MDL_MULTI_RW, byte_swapped, address, buffer, error);
}

/// Read registers OFFSET_Y_REG_L and OFFSET_Y_REG_H.
int16_t Lis2mdl::GetYOffset()
{
  int16_t data = -9999;

  I2Chip::I2cWriteUInt8(LIS2MDL_OFFSET_Y_REG_L | LIS2MDL_MULTI_RW, address, buffer, error);
  I2Chip::I2cReadBytes(2, address, buffer, error);
      
  if( error == 0 )
  {
    data = (int16_t)( buffer[ 0 ] ) | ( buffer[ 1 ] << 8 );
  }

  return data;
}

/// Write registers OFFSET_Y_REG_L and OFFSET_Y_REG_H.
void Lis2mdl::SetYOffset(int16_t offset)
{
  int16_t byte_swapped = ( offset << 8 ) | ( ( offset >> 8 ) & 0x00FF );

  I2Chip::I2cWriteRegisterUInt16(LIS2MDL_OFFSET_Y_REG_L | LIS2MDL_MULTI_RW, byte_swapped, address, buffer, error);
}

/// Read registers OFFSET_Z_REG_L and OFFSET_Z_REG_H.
int16_t Lis2mdl::GetZOffset()
{
  int16_t data = -9999;

  I2Chip::I2cWriteUInt8(LIS2MDL_OFFSET_Z_REG_L | LIS2MDL_MULTI_RW, address, buffer, error);
  I2Chip::I2cReadBytes(2, address, buffer, error);
      
  if( error == 0 )
  {
    data = (int16_t)( buffer[ 0 ] ) | ( buffer[ 1 ] << 8 );
  }

  return data;
}

/// Write registers OFFSET_Z_REG_L and OFFSET_Z_REG_H.
void Lis2mdl::SetZOffset(int16_t offset)
{
  int16_t byte_swapped = ( offset << 8 ) | ( ( offset >> 8 ) & 0x00FF );

  I2Chip::I2cWriteRegisterUInt16(LIS2MDL_OFFSET_Y_REG_L | LIS2MDL_MULTI_RW, byte_swapped, address, buffer, error);
}

/// Read chip WHO_AM_I register and return true if 0x40
bool Lis2mdl::WhoAmI()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_WHO_AM_I, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return (reg == 0x40);
}

/// Set bit COMP_TEMP_EN in register CFG_REG_A.
void Lis2mdl::TempCompEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Clear bit COMP_TEMP_EN in register CFG_REG_A.
void Lis2mdl::TempCompDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Set bit REBOOT in register CFG_REG_A.
void Lis2mdl::Reboot()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Set bit SOFT_RST in register CFG_REG_A.
void Lis2mdl::SoftReset()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Set bit LP in register CFG_REG_A.
void Lis2mdl::LowPowerEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Clear bit LP in register CFG_REG_A.
void Lis2mdl::LowPowerDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Read bits ODR[1:0] in register CFG_REG_A.
uint8_t Lis2mdl::GetDataRate()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x0C;
  reg = reg >> 2;

  return reg;
}

/// Modify bits ODR[1:0] in register CFG_REG_A.
void Lis2mdl::SetDataRate(uint8_t DataRate)
{
  uint8_t reg = 0;
  DataRate &= 0x03;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF3;
  reg |=  DataRate << 2;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Read bits MD[1:0] from register CFG_REG_A.
uint8_t Lis2mdl::GetOpMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x03;

  return reg;
}

/// Write bits MD[1:0] from register CFG_REG_A.
void Lis2mdl::SetOpMode(uint8_t OpMode)
{
  uint8_t reg = 0;
  OpMode &= 0x03;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFC;
  reg |= OpMode;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Set bits MD1 and set MD0 in register CFG_REG_A.
void Lis2mdl::IdleMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x03;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Clear bit MD1 and set MD0 in register CFG_REG_A.
void Lis2mdl::SingleMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;
  reg |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Clear bits MD1 and MD0 in register CFG_REG_A.
void Lis2mdl::ContinuousMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_A, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFC;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_A, reg, address, buffer, error);
}

/// Set bit OFF_CANC_ONE_SHOT in register CFG_REG_B.
void Lis2mdl::OffsetCancelSingleEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Clear bit OFF_CANC_ONE_SHOT in register CFG_REG_B.
void Lis2mdl::OffsetCancelSingleDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Set bit OFF_CANC in register CFG_REG_B.
void Lis2mdl::OffsetCancelEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Clear bit OFF_CANC in register CFG_REG_B.
void Lis2mdl::OffsetCancelDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Set bit LPF in register CFG_REG_B.
void Lis2mdl::LowPassEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Clear bit LPF in register CFG_REG_B.
void Lis2mdl::LowPassDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_B, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFE;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_B, reg, address, buffer, error);
}

/// Set bit BDU in register CFG_REG_C.
void Lis2mdl::BlockDataEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10;
  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}

/// Clear bit BDU in register CFG_REG_C.
void Lis2mdl::BlockDataDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}

/// Clear bit BLE in register CFG_REG_C.
void Lis2mdl::BigEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}

/// Set bit BLE in register CFG_REG_C.
void Lis2mdl::LittleEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}

/// Read register STATUS_REG.
uint8_t Lis2mdl::GetStatus()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Get interrupt threshold 0 - 65535.
uint16_t Lis2mdl::GetIntThreshold()
{
  uint16_t data = 9999;

  I2Chip::I2cWriteUInt8(LIS2MDL_INT_THS_L_REG | LIS2MDL_MULTI_RW, address, buffer, error);

  I2Chip::I2cReadBytes(2, address, buffer, error);
  if( error == 0 )
  {
    data = (uint16_t)( buffer[ 0 ] ) | ( buffer[ 1 ] << 8 );
  }

  return data;
}

/// Set interrupt threshold 0 - 65535.
void Lis2mdl::SetIntThreshold(uint16_t IntThreshold)
{
  uint16_t byte_swapped = ( IntThreshold << 8 ) | ( ( IntThreshold >> 8 ) & 0x00FF );

  I2Chip::I2cWriteRegisterUInt16(LIS2MDL_INT_THS_L_REG | LIS2MDL_MULTI_RW, byte_swapped, address, buffer, error);
}

/// Read chip OUTX_L_REG, OUTX_H_REG, OUTY_L_REG, OUTY_H_REG, OUTZ_L_REG, OUTZ_H_REG, TEMP_OUT_L_REG and TEMP_OUT_H_REG registers and return true if success.
bool Lis2mdl::ReadB()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS2MDL_OUTX_L_REG | LIS2MDL_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  { 
    I2cReadBytes(8, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    { 
      outX = (int16_t)( buffer[ 0 ] | ( buffer[ 1 ] << 8 ) );
      outY = (int16_t)( buffer[ 2 ] | ( buffer[ 3 ] << 8 ) );
      outZ = (int16_t)( buffer[ 4 ] | ( buffer[ 5 ] << 8 ) );
      temp = (int16_t)( buffer[ 6 ] | ( buffer[ 7 ] << 8 ) );
      
      Bx = 100 * outX / Gain;
      By = 100 * outY / Gain;
      Bz = 100 * outZ / Gain;
      T = temp / 256.0 + 25.0;
    }   
  }

  return success;
}

/// Read chip OUTX_L_REG and OUTX_H_REG registers and return true if success.
bool Lis2mdl::ReadBx()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS2MDL_OUTX_H_REG | LIS2MDL_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2cReadBytes(2, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outX = (int16_t)( buffer[ 0 ] | ( buffer[ 1 ] << 8 ) );
      Bx = 100 * outX / Gain;
    }
  }

  return success;
}

/// Read chip OUTY_L_REG and OUTY_H_REG registers and return true if success.
bool Lis2mdl::ReadBy()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS2MDL_OUTY_H_REG | LIS2MDL_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2cReadBytes(2, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outY = (int16_t)( buffer[ 0 ] | ( buffer[ 1 ] << 8 ) );
      By = 100 * outY / Gain;
    }
  }

  return success;
}

/// Read chip OUTZ_L_REG and OUTZ_H_REG registers and return true if success.
bool Lis2mdl::ReadBz()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS2MDL_OUTZ_H_REG | LIS2MDL_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2cReadBytes(2, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outZ = (int16_t)( buffer[ 0 ] | ( buffer[ 1 ] << 8 ) );
      Bz = 100 * outZ / Gain;
    }
  }

  return success;
}

/// Read STATUS_REG and test if bit ZYXDA bit is set.
bool Lis2mdl::NewDataXYZ()
{
  bool newdata = false;
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x08;

  if( reg == 0x00) newdata = false; else newdata = true;

  return newdata;
}

/// Read STATUS_REG and test if bit ZYXOR bi it set.
bool Lis2mdl::OverRunXYZ()
{
  bool overrun = false;
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;

  if( reg == 0x00) overrun = false; else overrun = true;

  return overrun;
}

/// Set bit Self_test in register CFG_REG_C. 
void Lis2mdl::SelfTestEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}

/// Clear bit Self_test in register CFG_REG_C. 
void Lis2mdl::SelfTestDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_CFG_REG_C, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_CFG_REG_C, reg, address, buffer, error);
}


/// Read register INT_CTRL_REG.
uint8_t Lis2mdl::GetIntConfig()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS2MDL_INT_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Set register INT_CFG.
void Lis2mdl::SetIntConfig(uint8_t IntConf)
{
  IntConf &= 0xE7;

  I2Chip::I2cWriteRegisterUInt8(LIS2MDL_INT_CTRL_REG, IntConf, address, buffer, error);
}

/// Read register INT_SOURCE_REG.
uint8_t Lis2mdl::GetIntSource()
{
  uint8_t reg = 0;
      
  I2Chip::I2cWriteUInt8(LIS2MDL_INT_SOURCE_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
      
  return reg;
}

