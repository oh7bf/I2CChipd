/**************************************************************************
 * 
 * Ads1015 class member functions for configuration and reading with I2C. 
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
 * Sat Aug  8 20:19:22 CDT 2020
 * Edit: Thu 13 Aug 2020 07:47:45 PM CDT
 *
 * Jaakko Koivuniemi
 **/

#include "Ads1015.hpp"

using namespace std;

Ads1015::~Ads1015() { };

/// First pointer register is set to HIGH_THRESH register followed by two byte 
/// transfer from the chip. 
int16_t Ads1015::GetHighThreshold()
{
  int16_t HighThreshold = 0;

  I2Chip::I2cWriteUInt8(ADS1015_HIGH_THRESH_REG, address, buffer, error);
  HighThreshold = (int16_t)I2Chip::I2cReadUInt16(address, buffer, error);

  return HighThreshold;
}

/// First pointer register is set to LOW_THRESH register followed by two byte 
/// transfer from the chip. 
int16_t Ads1015::GetLowThreshold()
{
  int16_t LowThreshold = 0;

  I2Chip::I2cWriteUInt8(ADS1015_LOW_THRESH_REG, address, buffer, error);
  LowThreshold = (int16_t)I2Chip::I2cReadUInt16(address, buffer, error);

  return LowThreshold;
} 

/// First pointer register is set to CONFIG register followed by two byte 
/// transfer from the chip. 
uint16_t Ads1015::GetConfig()
{
  uint16_t Config = 0;

  I2Chip::I2cWriteUInt8(ADS1015_CONFIG_REG, address, buffer, error);
  Config = I2Chip::I2cReadUInt16(address, buffer, error);

  return Config;
} 

/// The CONFIG register is read to get programmable gain amplifier setting.
uint16_t Ads1015::GetPGA()
{
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  pga = ( (Config & 0x0E00 ) >> 9);

  return pga;
}

/// The CONFIG register is read to get multiplexer configuration setting.
uint16_t Ads1015::GetMux()
{
  uint16_t mux = 0;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  mux = ( (Config & 0x7000 ) >> 12);

  return mux;
}

/// The CONFIG register is read to get mode bit.
uint16_t Ads1015::GetCompMode()
{
  uint16_t CompMode = 2;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  CompMode = ( Config & 0x0010 ) >> 4;

  return CompMode;
}

/// The CONFIG register is read to get polarity bit.
uint16_t Ads1015::GetCompPolarity()
{
  uint16_t CompPol = 2;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  CompPol = ( Config & 0x0008 ) >> 3;

  return CompPol;
}

/// The CONFIG register is read to get latching bit.
uint16_t Ads1015::GetCompLatching()
{
  uint16_t CompLatch = 2;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  CompLatch = ( Config & 0x0004 ) >> 2;

  return CompLatch;
}

/// The CONFIG register is read to get two queue bits.
uint16_t Ads1015::GetCompQueue()
{
  uint16_t CompQueue = 5;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  CompQueue = Config & 0x0003;

  return CompQueue;
}

/// The CONFIG register is read to get three data rate bits.
uint16_t Ads1015::GetDataRate()
{
  uint16_t Rate = 8;
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  Rate = ( Config & 0x00E0 ) >> 5;

  return Rate;
}

/// Write one byte to chip to change pointer value for reading registers.
void Ads1015::SetPointer(uint8_t PointerReg)
{
  I2Chip::I2cWriteUInt8(PointerReg, address, buffer, error);
}

/// Write two bytes to HIGH_THRESH register.
void Ads1015::SetHighThreshold(int16_t HighThreshold)
{
  I2Chip::I2cWriteRegisterUInt16(ADS1015_HIGH_THRESH_REG, HighThreshold, address, buffer, error);
}

/// Write two bytes to LOW_THRESH register.
void Ads1015::SetLowThreshold(int16_t LowThreshold)
{
  I2Chip::I2cWriteRegisterUInt16(ADS1015_LOW_THRESH_REG, LowThreshold, address, buffer, error);
}

/// Write two bytes to CONFIG register.
void Ads1015::SetConfig(uint16_t Config)
{
  I2Chip::I2cWriteRegisterUInt16(ADS1015_CONFIG_REG, Config, address, buffer, error);
}

/// Set three PGA bits in CONFIG register.
void Ads1015::SetPGA(uint16_t PGA)
{
  uint16_t Config = 0;

  PGA &= 0x0007;
  pga = PGA;
  PGA = PGA << 9;
  Config = Ads1015::GetConfig();
  Config &= 0xF1FF;
  Config |= PGA; 
  
  Ads1015::SetConfig( Config );

}

/// Set three MUX bits in CONFIG register.
void Ads1015::SetMux(uint16_t Mux)
{
  uint16_t Config = 0;

  Mux &= 0x0007;
  Mux = Mux << 12;
  Config = Ads1015::GetConfig();
  Config &= 0x8FFF;
  Config |= Mux; 
  
  Ads1015::SetConfig( Config );

}

/// Set two queue bits in CONFIG register.
void Ads1015::SetCompQueue(uint16_t CompQueue)
{
  uint16_t Config = 0;

  CompQueue &= 0x0003;
  Config = Ads1015::GetConfig();
  Config &= 0xFFFC;
  Config |= CompQueue; 
  
  Ads1015::SetConfig( Config );

}

/// Set three data rate bits in CONFIG register.
void Ads1015::SetDataRate(uint16_t DataRate)
{
  uint16_t Config = 0;

  DataRate &= 0x0007;
  DataRate = DataRate << 5;
  Config = Ads1015::GetConfig();
  Config &= 0xFF1F;
  Config |= DataRate; 

  Ads1015::SetConfig( Config );

}

/// Set polarity bit in CONFIG register.
void Ads1015::SetCompPolarity(uint16_t CompPolarity)
{
  uint16_t Config = 0;

  CompPolarity &= 0x0001;
  CompPolarity = CompPolarity << 3;
  Config = Ads1015::GetConfig();
  Config &= 0xFFF7;
  Config |= CompPolarity; 

  Ads1015::SetConfig( Config );

}

/// Set latching bit in CONFIG register.
void Ads1015::SetCompLatching(uint16_t CompLatching)
{
  uint16_t Config = 0;

  CompLatching &= 0x0001;
  CompLatching = CompLatching << 2;
  Config = Ads1015::GetConfig();
  Config &= 0xFFFB;
  Config |= CompLatching; 

  Ads1015::SetConfig( Config );
}

/// Set comparator mode bit in CONFIG register.
void Ads1015::SetCompMode(uint16_t CompMode)
{
  uint16_t Config = 0;

  CompMode &= 0x0001;
  CompMode = CompMode << 8;
  Config = Ads1015::GetConfig();
  Config &= 0xFEFF;
  Config |= CompMode; 

  Ads1015::SetConfig( Config );
}

/// Set MODE = 1 in CONFIG register.
void Ads1015::Powerdown()
{
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  Config |= 0x0100;
  Ads1015::SetConfig( Config );
}

/// Set MODE = 0 in CONFIG register.
void Ads1015::Continuous()
{
  uint16_t Config = 0;

  Config = Ads1015::GetConfig();
  Config &= 0xFEFF;
  Ads1015::SetConfig( Config );
}

/// Set OS = 1 in CONFIG register.
void Ads1015::SingleConversion()
{
  uint16_t Config = 0;
  Config = Ads1015::GetConfig();
  Config |= 0x8000; 
  Ads1015::SetConfig( Config );
}

/// Read two bytes from chip. SetPointer( ADS1015_CONV_REG ) needs to be 
/// called before using this function. Private variable _pga_ is used
/// in conversion to voltage. 
bool Ads1015::ReadConversion()
{
  bool success = true;

  adc = (int16_t)I2Chip::I2cReadUInt16(address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    switch( pga )
    {
      case 0:
        Voltage = adc * 6.144 / 32767;
        break;
      case 1:
        Voltage = adc * 4.096 / 32767;
	break;
      case 2:
        Voltage = adc * 2.048 / 32767;
        break;
      case 3:
        Voltage = adc * 1.024 / 32767;
        break;
      case 4:
        Voltage = adc * 0.512 / 32767;
        break;
      case 5:
        Voltage = adc * 0.256 / 32767;
        break;
      case 6:
        Voltage = adc * 0.256 / 32767;
        break;
      case 7:
        Voltage = adc * 0.256 / 32767;
	break;
      default:
	success = false;
    }
  }

  return success;
}

