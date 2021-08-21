/**************************************************************************
 * 
 * Bh1750fvi class member functions for configuration and reading with I2Chip. 
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
 * Sun 08 Aug 2021 04:51:22 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "Bh1750fvi.hpp"

using namespace std;

Bh1750fvi::~Bh1750fvi() { };

/// No active state.
void Bh1750fvi::PowerDown()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_POWER_DOWN, address, buffer, error);
}

/// Waiting for measurement command.
void Bh1750fvi::PowerOn()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_POWER_ON, address, buffer, error);
}

/// Reset Data register value. Reset command is not acceptable in Power Down mode.
void Bh1750fvi::Reset()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_RESET, address, buffer, error);
}

/// Start measurement at 1 lx resolution. Measurement time typically 120 ms.
void Bh1750fvi::ContHighResMode()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_CONT_HIGH_RES_MODE, address, buffer, error);
  mode2 = false;
}

/// Start measurement at 0.5 lx resolution. Measurement time typically 120 ms.
void Bh1750fvi::ContHighResMode2()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_CONT_HIGH_RES_MODE2, address, buffer, error);
  mode2 = true;
}

/// Start measurement at 4 lx resolution. Measurement time typically 16 ms.
void Bh1750fvi::ContLowResMode()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_CONT_LOW_RES_MODE, address, buffer, error);
  mode2 = false;
}

/// Start measurement at 1 lx resolution. Measurement time typically 120 ms. Power Down mode after measurement.
void Bh1750fvi::OneTimeHighResMode()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_ONE_TIME_HIGH_RES_MODE, address, buffer, error);
  mode2 = false;
}

/// Start measurement at 0.5 lx resolution. Measurement time typically 120 ms.  Power Down mode after measurement.  
void Bh1750fvi::OneTimeHighResMode2()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_ONE_TIME_HIGH_RES_MODE2, address, buffer, error);
  mode2 = true;
}

/// Start measurement at 4 lx resolution. Measurement time typically 16 ms. Power Down mode after measurement.
void Bh1750fvi::OneTimeLowResMode()
{
  I2Chip::I2cWriteUInt8(BH1750FVI_ONE_TIME_LOW_RES_MODE, address, buffer, error);
  mode2 = false;
}

/// MTreg value between 31 and 254. High resolution illuminance per 1 count (lx / count) = 69 / ( 1.2 * MTreg ) and half of this in mode 2.   
void Bh1750fvi::SetMeasurementTime(uint8_t MTreg)
{
  uint8_t high_bits = 0, low_bits = 0;

  if( MTreg >= 31 && MTreg <= 254 )
  {
    high_bits = MTreg >> 5;
    high_bits |= BH1750FVI_CHANGE_MEAS_TIME_HIGH;
    I2Chip::I2cWriteUInt8(high_bits, address, buffer, error);

    low_bits = MTreg & 0x1F;
    low_bits |= BH1750FVI_CHANGE_MEAS_TIME_LOW;
    I2Chip::I2cWriteUInt8(low_bits, address, buffer, error);

    MTRegister = MTreg;
  } 
}

/// Convert data to illuminance in lux using MTreg value. 
bool Bh1750fvi::ReadIlluminance()
{
  bool success = true;
  uint16_t data = 0;

  data = I2Chip::I2cReadUInt16(address, buffer, error);

  if( error != 0 )
  {
    success = false;
  }
  else
  {
    if( mode2 )
      Illuminance = double(data) * MTRegister / (1.2 * 69 * 2);
    else
      Illuminance = double(data) * MTRegister / (1.2 * 69 );
  }

  return success;
}

