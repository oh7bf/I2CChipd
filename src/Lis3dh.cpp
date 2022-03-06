/**************************************************************************
 * 
 * Lis3dh class member functions for configuration and reading with I2C. 
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
 * Sat Feb 26 19:29:54 CST 2022
 * Edit:
 *
 * Jaakko Koivuniemi
 **/

#include "Lis3dh.hpp"
//#include <unistd.h>

using namespace std;

Lis3dh::~Lis3dh() { };


/// Read chip WHO_AM_I register and return True if 0x33
bool Lis3dh::WhoAmI()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_WHO_AM_I, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return (reg == 0x33);
}

/// Read register STATUS_REG.
uint8_t Lis3dh::GetStatus()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Read register REFERENCE.
uint8_t Lis3dh::GetReference()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_REFERENCE, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Read register CTRL_REG1.
uint8_t Lis3dh::GetCtrlReg1()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  return reg;
}

/// Set TEMP_EN bit in register TEMP_CFG_REG. 
void Lis3dh::TempEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_TEMP_CFG_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TEMP_CFG_REG, reg, address, buffer, error);
}

/// Clear TEMP_EN bit in register TEMP_CFG_REG.
void Lis3dh::TempDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_TEMP_CFG_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xBF;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TEMP_CFG_REG, reg, address, buffer, error);
}

/// Enable ADC.
void Lis3dh::ADCEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_TEMP_CFG_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TEMP_CFG_REG, reg, address, buffer, error);
}

/// Disable ADC.
void Lis3dh::ADCDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_TEMP_CFG_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TEMP_CFG_REG, reg, address, buffer, error);
}

/// Read bits ODR[3:0] from register CTRL_REG1.
uint8_t Lis3dh::GetDataRate()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF0;
  reg = reg >> 4;

  return reg;
}

/// Modify bits ODR[3:0] in register CTRL_REG1.
void Lis3dh::SetDataRate(uint8_t DataRate)
{
  uint8_t reg = 0;
  DataRate &= 0x0F;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);

  reg &= 0x0F;
  reg |=  DataRate << 4;
  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Set bit Xen in register CTRL_REG1. 
void Lis3dh::XEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Clear bit Xen in register CTRL_REG1. 
void Lis3dh::XDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFE;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Set bit Yen in register CTRL_REG1. 
void Lis3dh::YEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Clear bit Yen in register CTRL_REG1. 
void Lis3dh::YDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Set bit Zen in register CTRL_REG1. 
void Lis3dh::ZEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x04;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Clear bit Zen in register CTRL_REG1. 
void Lis3dh::ZDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFB;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);
}

/// Read bits HPM[1:0] in register CTRL_REG2.
uint8_t Lis3dh::GetHpm()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xC0;
  reg = reg >> 6;

  return reg;	
}

/// Modify bits HPM[1:0] in register CTRL_REG2
void Lis3dh::SetHpm(uint8_t Hpm)
{
  uint8_t reg = 0;
  Hpm &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x3F;
  reg |=  Hpm << 6;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Read bits HPCF[2:1] from register CTRL_REG2.
uint8_t Lis3dh::GetHpcf()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xCF;
  reg = reg >> 4;

  return reg;	
}

/// Modify bits HPCF[2:1] in register CTRL_REG2
void Lis3dh::SetHpcf(uint8_t Hpcf)
{
  uint8_t reg = 0;
  Hpcf &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xCF;
  reg |=  Hpcf << 4;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Clear bit FDS in register CTRL_REG2. 
void Lis3dh::FdBypass()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit FDS in register CTRL_REG2. 
void Lis3dh::FdSelect()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit HP_IA1 in CTRL_REG2.
void Lis3dh::HpIa1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Clear bit HP_IA1 in CTRL_REG2.
void Lis3dh::HpIa1Bypass()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFE;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit HP_IA2 in CTRL_REG2.
void Lis3dh::HpIa2Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Clear bit HP_IA2 in CTRL_REG2.
void Lis3dh::HpIa2Bypass()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Set bit HPCLICK in CTRL_REG2.
void Lis3dh::HpClickEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x04;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}

/// Clear bit HPCLICK in CTRL_REG2.
void Lis3dh::HpClickDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG2, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFB;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG2, reg, address, buffer, error);
}


/// Set bit BDU in CTRL_REG4.
void Lis3dh::BlockDataEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
}

/// Clear bit BDU in CTRL_REG4.
void Lis3dh::BlockDataDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
}

/// Read bits FS[1:0] from register CTRL_REG4.
uint8_t Lis3dh::GetFullScale()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x30;
  reg = reg >> 4;

  return reg;
}

/// Modify bits FS[1:0] in register CTRL_REG4.
void Lis3dh::SetFullScale(uint8_t FullScale)
{
  uint8_t reg = 0;
  FullScale &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xCF;
  reg |=  FullScale << 4;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);

  if( FullScale == 0 ) FS = 2;
  else if( FullScale == 1 ) FS = 4;
  else if( FullScale == 2 ) FS = 8;
  else if( FullScale == 3 ) FS = 16;
}

/// Set bit BLE in CTRL_REG4.
void Lis3dh::BigEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
}

/// Clear bit BLE in CTRL_REG4.
void Lis3dh::LittleEndian()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xBF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
}

/// Set bit LPen in CTRL_REG1 and clear HR bit in CTRL_REG4.
void Lis3dh::LowPowerMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);

  So = 0.016; // Sensitivity at +-2 g full-scale
}

/// Clear bit LPen in CTRL_REG1 and clear HR bit in CTRL_REG4.
void Lis3dh::NormalMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);

  So = 0.004; // Sensitivity at +-2 g full-scale
}

/// Clear bit LPen in CTRL_REG1 and set HR bit in CTRL_REG4.
void Lis3dh::HighResolutionMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG1, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG1, reg, address, buffer, error);

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
  
  So = 0.001; // Sensitivity at +-2 g full-scale
}

/// Modify bits ST[1:0] in register CTRL_REG4.
void Lis3dh::SetSelfTest(uint8_t STMode)
{
  uint8_t reg = 0;
  STMode &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG4, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF9;
  reg |=  STMode << 1;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG4, reg, address, buffer, error);
}

/// Set bit BOOT in CTRL_REG5.
void Lis3dh::Boot()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG5, reg, address, buffer, error);
}

/// Set bit FIFO_EN in CTRL_REG5.
void Lis3dh::FifoEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG5, reg, address, buffer, error);
}

/// Clear bit FIFO_EN in CTRL_REG5.
void Lis3dh::FifoDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG5, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xBF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG5, reg, address, buffer, error);
}

/// Read bits FM[1:0] from register FIFO_CTRL_REG.
uint8_t Lis3dh::GetFifoMode()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xC0;
  reg = reg >> 6;

  return reg;
}

/// Modify bits FM[1:0] in register FIFO_CTRL_REG.
void Lis3dh::SetFifoMode(uint8_t FifoMode)
{
  uint8_t reg = 0;
  FifoMode &= 0x03;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x3F;
  reg |=  FifoMode << 6;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_FIFO_CTRL_REG, reg, address, buffer, error);
}

/// Clear bit TR in FIFO_CTRL_REG.
void Lis3dh::StreamFifoInt1()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xDF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_FIFO_CTRL_REG, reg, address, buffer, error);
}

/// Set bit TR in FIFO_CTRL_REG.
void Lis3dh::StreamFifoInt2()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_FIFO_CTRL_REG, reg, address, buffer, error);
}

/// Read bits FTH[4:0] from register FIFO_CTRL_REG.
uint8_t Lis3dh::GetWatermark()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x1F;

  return reg;
}

/// Modify bits FTH[4:0] in register FIFO_CTRL_REG.
void Lis3dh::SetWatermark(uint8_t Wtm)
{
  uint8_t reg = 0;

  Wtm &= 0x1F;
  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_CTRL_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xE0;
  reg |= Wtm;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_FIFO_CTRL_REG, reg, address, buffer, error);
}

/// Read FIFO_SRC_REG and return true if WTM bit set. 
bool Lis3dh::Watermark()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_SRC_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;

  return (reg == 0x80);
}

/// Read FIFO_SRC_REG and return true if OVRN_FIFO bit set. 
bool Lis3dh::Overrun()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_SRC_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x40;

  return (reg == 0x40);
}

/// Read FIFO_SRC_REG and return true if EMPTY bit set. 
bool Lis3dh::Empty()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_SRC_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x20;

  return (reg == 0x20);
}

/// Read bits FSS[4:0] from register FIFO_SRC_REG.
uint8_t Lis3dh::Samples()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_FIFO_SRC_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x1F;

  return reg;
}

/// Read STATUS_REG and return true if ZYXDA bit set. 
bool Lis3dh::NewDataXYZ()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x08;

  return (reg == 0x08);
}

/// Read STATUS_REG and return true if ZYXOR bit set. 
bool Lis3dh::OverRunXYZ()
{
  uint8_t reg = 0;
  I2Chip::I2cWriteUInt8(LIS3DH_STATUS_REG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  return (reg == 0x80);
}

/// Set bit I1_CLICK in CTRL_REG3.
void Lis3dh::ClickInt1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_CLICK in CTRL_REG3.
void Lis3dh::ClickInt1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_IA1 in CTRL_REG3.
void Lis3dh::Ia1Int1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_IA1 in CTRL_REG3.
void Lis3dh::Ia1Int1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xBF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_IA2 in CTRL_REG3.
void Lis3dh::Ia2Int1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_IA2 in CTRL_REG3.
void Lis3dh::Ia2Int1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xDF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_ZYXDA in CTRL_REG3.
void Lis3dh::ZYXDaInt1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_ZYXDA in CTRL_REG3.
void Lis3dh::ZYXDaInt1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_321DA in CTRL_REG3.
void Lis3dh::Da321Int1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_321DA in CTRL_REG3.
void Lis3dh::Da321Int1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_WTM in CTRL_REG3.
void Lis3dh::WtmInt1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x04; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_WTM in CTRL_REG3.
void Lis3dh::WtmInt1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFB; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I1_OVERRUN in CTRL_REG3.
void Lis3dh::OverrunInt1Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Clear bit I1_OVERRUN in CTRL_REG3.
void Lis3dh::OverrunInt1Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG3, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG3, reg, address, buffer, error);
}

/// Set bit I2_CLICK in CTRL_REG6.
void Lis3dh::ClickInt2Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Clear bit I2_CLICK in CTRL_REG6.
void Lis3dh::ClickInt2Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit I2_IA1 in CTRL_REG6.
void Lis3dh::Ia1Int2Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x40; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Clear bit I2_IA1 in CTRL_REG6.
void Lis3dh::Ia1Int2Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xBF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit I2_IA2 in CTRL_REG6.
void Lis3dh::Ia2Int2Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Clear bit I2_IA2 in CTRL_REG6.
void Lis3dh::Ia2Int2Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xDF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit I2_BOOT in CTRL_REG6.
void Lis3dh::Int2BootEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Clear bit I2_BOOT in CTRL_REG6.
void Lis3dh::Int2BootDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit I2_ACT in CTRL_REG6.
void Lis3dh::ActivityInt2Enable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}


/// Clear bit I2_ACT in CTRL_REG6.
void Lis3dh::ActivityInt2Disable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Clear bit INT_POLARITY in CTRL_REG6.
void Lis3dh::IntActiveHigh()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit INT_POLARITY in CTRL_REG6.
void Lis3dh::IntActiveLow()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CTRL_REG6, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CTRL_REG6, reg, address, buffer, error);
}

/// Set bit ZD in CLICK_CFG.
void Lis3dh::DoubleTapZEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x20; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit ZD in CLICK_CFG.
void Lis3dh::DoubleTapZDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xDF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Set bit ZS in CLICK_CFG.
void Lis3dh::SingleTapZEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x10; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit ZS in CLICK_CFG.
void Lis3dh::SingleTapZDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xEF; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Set bit YD in CLICK_CFG.
void Lis3dh::DoubleTapYEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x08; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit YD in CLICK_CFG.
void Lis3dh::DoubleTapYDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xF7; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Set bit YS in CLICK_CFG.
void Lis3dh::SingleTapYEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x04; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit YS in CLICK_CFG.
void Lis3dh::SingleTapYDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFB; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Set bit XD in CLICK_CFG.
void Lis3dh::DoubleTapXEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x02; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit XD in CLICK_CFG.
void Lis3dh::DoubleTapXDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFD; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Set bit XS in CLICK_CFG.
void Lis3dh::SingleTapXEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x01; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Clear bit XS in CLICK_CFG.
void Lis3dh::SingleTapXDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_CFG, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0xFE; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_CFG, reg, address, buffer, error);
}

/// Read CLICK_SRC and return true if IA bit set. 
bool Lis3dh::ClickInt()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x40;

  return (reg == 0x40);
}

/// Read CLICK_SRC and return true if DCLICK bit set. 
bool Lis3dh::DoubleClick()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x20;
      
  return (reg == 0x20);
}

/// Read CLICK_SRC and return true if SCLICK bit set. 
bool Lis3dh::SingleClick()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x10;
      
  return (reg == 0x10);
}

/// Read CLICK_SRC and return true if negative detection Sign bit set. 
bool Lis3dh::ClickSign()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x08;
      
  return (reg == 0x08);
}
      
/// Read CLICK_SRC and return true if Z bit set. 
bool Lis3dh::ClickZ()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x04;
      
  return (reg == 0x04);
}
      
/// Read CLICK_SRC and return true if Y bit set. 
bool Lis3dh::ClickY()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x02;
      
  return (reg == 0x02);
}
      
/// Read CLICK_SRC and return true if X bit set. 
bool Lis3dh::ClickX()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_SRC, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x01;
      
  return (reg == 0x01);
}

/// Read bits Ths[6:0] from register CLICK_THS.
uint8_t Lis3dh::GetClickThs()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;
      
  return reg;
}

/// Modify bits Ths[6:0] in register CLICK_THS.
void Lis3dh::SetClickThs(uint8_t Ths)
{
  uint8_t reg = 0;
  Ths &= 0x7F;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  reg |= Ths;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_THS, reg, address, buffer, error);
}

/// Set bit LIR_Click in CLICK_THS.
void Lis3dh::ClickLatchEnable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg |= 0x80; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_THS, reg, address, buffer, error);
}

/// Clear bit LIR_Click in CLICK_THS.
void Lis3dh::ClickLatchDisable()
{
  uint8_t reg = 0;

  I2Chip::I2cWriteUInt8(LIS3DH_CLICK_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F; 

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_CLICK_THS, reg, address, buffer, error);
}

/// Modify bits TLI[6:0] in register TIME_LIMIT.
void Lis3dh::SetClickTlimit(uint8_t Tli)
{
  uint8_t reg = 0;
  Tli &= 0x7F;

  I2Chip::I2cWriteUInt8(LIS3DH_TIME_LIMIT, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  reg |= Tli;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TIME_LIMIT, reg, address, buffer, error);
}

/// Modify bits TLA[7:0] in register TIME_LATENCY.
void Lis3dh::SetClickTlatency(uint8_t Tla)
{
  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TIME_LATENCY, Tla, address, buffer, error);
}

/// Modify bits TW[7:0] in register TIME_WINDOW.
void Lis3dh::SetClickTwindow(uint8_t Tw)
{
  I2Chip::I2cWriteRegisterUInt8(LIS3DH_TIME_WINDOW, Tw, address, buffer, error);
}

/// Read bits Acth[6:0] from register ACT_THS.
uint8_t Lis3dh::GetActThreshold()
{
  uint8_t reg;

  I2Chip::I2cWriteUInt8(LIS3DH_ACT_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x7F;
      
  return reg;
}

/// Modify bits Acth[6:0] in register ACT_THS.
void Lis3dh::SetActThreshold(uint8_t Ths)
{
  uint8_t reg = 0;
  Ths &= 0x7F;

  I2Chip::I2cWriteUInt8(LIS3DH_ACT_THS, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
  reg &= 0x80;
  reg |= Ths;

  I2Chip::I2cWriteRegisterUInt8(LIS3DH_ACT_THS, reg, address, buffer, error);
}

/// Read bits ActD[7:0] from register ACT_DUR.
uint8_t Lis3dh::GetActDuration()
{
  uint8_t reg;

  I2Chip::I2cWriteUInt8(LIS3DH_ACT_DUR, address, buffer, error);
  reg = I2Chip::I2cReadUInt8(address, buffer, error);
      
  return reg;
}

/// Modify bits ActD[7:0] in register ACT_DUR.
void Lis3dh::SetActDuration(uint8_t Dur)
{
  I2Chip::I2cWriteRegisterUInt8(LIS3DH_ACT_DUR, Dur, address, buffer, error);
}

/// Self-test procedure. Return true if success.
bool Lis3dh::SelfTest()
{
  return false;
}

/// Read chip OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L and OUT_Z_H registers and return true if success.
bool Lis3dh::Readg()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS3DH_OUT_X_L | LIS3DH_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2cReadBytes(6, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      outX = (int16_t)( buffer[ 0 ] | ( buffer[ 1 ] << 8 ) );
      outY = (int16_t)( buffer[ 2 ] | ( buffer[ 3 ] << 8 ) );
      outZ = (int16_t)( buffer[ 4 ] | ( buffer[ 5 ] << 8 ) );

      gx =  FS * (double)outX / 32768.0;
      gy =  FS * (double)outY / 32768.0;
      gz =  FS * (double)outZ / 32768.0;
    }
  }

  return success;
}

/// Read chip OUT_ADC1_L, OUT_ADC1_H, OUT_ADC2_L, OUT_ADC2_H, OUT_ADC3_L and OUT_ADC3_H registers and return true if success.
bool Lis3dh::ReadAdc()
{
  bool success = true;

  I2Chip::I2cWriteUInt8(LIS3DH_OUT_ADC1_L | LIS3DH_MULTI_RW, address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    I2cReadBytes(6, address, buffer, error);

    if( error != 0 )
    {
      success = false;
    }
    else
    {
      Adc1  = (uint16_t)buffer[ 0 ];
      Adc1 |= (uint16_t)(buffer[ 1 ] << 8);
      Adc2  = (uint16_t)buffer[ 2 ];
      Adc2 |= (uint16_t)(buffer[ 3 ] << 8);
      Adc3  = (uint16_t)buffer[ 4 ];
      Adc3 |= (uint16_t)(buffer[ 5 ] << 8);
    }
  }

  return success;
}

/// Read chip FIFO and return number of values 0 - 32.
int8_t Lis3dh::ReadFifo()
{
  return 0;
}

