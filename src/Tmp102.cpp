/**************************************************************************
 * 
 * Tmp102 class member functions for configuration and reading with I2C. 
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
 * Sat Jul  4 15:13:58 CDT 2020
 * Edit: Mon Jul  6 16:27:21 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#include "Tmp102.hpp"

using namespace std;

Tmp102::~Tmp102() { };

/// First pointer register is set T_HIGH register followed by two byte transfer
/// from the chip. 
uint16_t Tmp102::GetHighLimit()
{
  uint16_t HighLimit = 0;

  I2Chip::I2cWriteUInt8(TMP102_TEMP_HIGH_REG, address, buffer, error);
  HighLimit = I2Chip::I2cReadUInt16(address, buffer, error);

  return HighLimit;
}

/// First pointer register is set T_LOW register followed by two byte transfer
/// from the chip. 
uint16_t Tmp102::GetLowLimit()
{
  uint16_t LowLimit = 0;

  I2Chip::I2cWriteUInt8(TMP102_TEMP_LOW_REG, address, buffer, error);
  LowLimit = I2Chip::I2cReadUInt16(address, buffer, error);

  return LowLimit;
} 

/// First pointer register is set CONFIG register followed by two byte transfer
/// from the chip. 
uint16_t Tmp102::GetConfig()
{
  uint16_t Config = 0;

  I2Chip::I2cWriteUInt8(TMP102_CONFIG_REG, address, buffer, error);
  Config = I2Chip::I2cReadUInt16(address, buffer, error);

  return Config;
} 

/// The CONFIG register is read to get ALERT pin status.
int Tmp102::GetPolarity()
{
  int Pol = -1;
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Pol = ( Config & 0x0400 ) >> 10;

  return Pol;
}

/// The CONFIG register is read to get two fault queue bits.
uint8_t Tmp102::GetFaultQueue()
{
  uint8_t FaultQueue = 0;
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  FaultQueue = (uint8_t)(( Config & 0x1800 ) >> 11);

  return FaultQueue;
}

/// The CONFIG register is read to get two conversion rate bits.
int Tmp102::GetConversionRate()
{
  int ConversionRate = 0;
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  ConversionRate = ( Config & 0x00C0 ) >> 6;

  return ConversionRate;
}

/// The CONFIG register is read to get ALERT bit.
int Tmp102::GetAlertBit()
{
  int Alert = -1;
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Alert = ( Config & 0x0020 ) >> 5;

  return Alert;
}

/// Write one byte to chip to change pointer value for reading registers.
void Tmp102::SetPointer(uint8_t PointerReg)
{
  I2Chip::I2cWriteUInt8(PointerReg, address, buffer, error);
}

/// Write two bytes to T_HIGH register.
void Tmp102::SetHighLimit(uint16_t THigh)
{
  I2Chip::I2cWriteRegisterUInt16(TMP102_TEMP_HIGH_REG, THigh, address, buffer, error);
}

/// Write two bytes to T_LOW register.
void Tmp102::SetLowLimit(uint16_t TLow)
{
  I2Chip::I2cWriteRegisterUInt16(TMP102_TEMP_LOW_REG, TLow, address, buffer, error);
}

/// Write two bytes to CONFIG register.
void Tmp102::SetConfig(uint16_t Config)
{
  I2Chip::I2cWriteRegisterUInt16(TMP102_CONFIG_REG, Config, address, buffer, error);
}

/// Set SD = 1 in CONFIG register.
void Tmp102::Shutdown()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config |= 0x0100;
  Tmp102::SetConfig( Config );
}

/// Set SD = 0 in CONFIG register.
void Tmp102::Continuous()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config &= 0xFEFF;
  Tmp102::SetConfig( Config );
}

/// Set TM = 0 in CONFIG register.
void Tmp102::ComparatorMode()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config &= 0xFDFF;
  Tmp102::SetConfig( Config );
}

/// Set TM = 1 in CONFIG register.
void Tmp102::InterruptMode()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config |= 0x0200;
  Tmp102::SetConfig( Config );
}

/// Set POL bit in CONFIG register.
void Tmp102::SetPolarity(int Pol)
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  if( Pol == 0 ) Config &= 0xFBFF; 
  else if( Pol == 1 ) Config |= 0x0400;

  Tmp102::SetConfig( Config );
}

/// Set two queue bits in CONFIG register.
void Tmp102::SetFaultQueue(uint16_t FaultQueue)
{
  uint16_t Config = 0;

  FaultQueue &= 0x0003;
  FaultQueue = FaultQueue << 11;
  Config = Tmp102::GetConfig();
  Config &= 0xE700;
  Config |= FaultQueue; 

  Tmp102::SetConfig( Config );

}

/// Set two conversion rate bits in CONFIG register.
void Tmp102::SetConversionRate(int ConversionRate)
{
  uint16_t Config = 0;

  ConversionRate &= 0x0003;
  ConversionRate = ConversionRate << 6;
  Config = Tmp102::GetConfig();
  Config &= 0xFF3F;
  Config |= ConversionRate; 

  Tmp102::SetConfig( Config );
}

/// Set OS = 1 in CONFIG register.
void Tmp102::OneShot()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config |= 0x8000; 

  Tmp102::SetConfig( Config );
}

/// Set EM = 1 in CONFIG register.
void Tmp102::ExtendedMode()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config |= 0x0010; 

  Tmp102::SetConfig( Config );
}

/// Set EM = 0 in CONFIG register.
void Tmp102::NormalMode()
{
  uint16_t Config = 0;

  Config = Tmp102::GetConfig();
  Config &= 0xFFEF; 

  Tmp102::SetConfig( Config );
}

/// Read two bytes from chip and use least significant bit to determine
/// normal or extended mode. SetPointer( 0 ) needs to be used before
/// calling this function. 
double Tmp102::GetTemperature()
{
  uint16_t data = 0;
  int16_t temp = 0;
  double temperature = 0;

  data = I2Chip::I2cReadUInt16(address, buffer, error);

  temp = (int16_t)data;

  if( ( data & 1 ) == 0 )
  {
    temp /= 16;
    temperature = (double)(temp * 0.0625);
  }
  else
  {
    temp /= 8;
    temperature = (double)(temp * 0.0625);
  }

  if( error != 0 ) temp = -9999.9999;

  return temperature;
}

