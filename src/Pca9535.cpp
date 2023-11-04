/**************************************************************************
 * 
 * Pca9535 class member functions for configuration and reading with I2Chip. 
 *       
 * Copyright (C) 2023 Jaakko Koivuniemi.
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
 * Fri  3 Nov 15:04:38 CDT 2023
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "Pca9535.hpp"

using namespace std;

Pca9535::~Pca9535() { };

/// Write INPUT_PORT_0 as command and transfer two bytes from the chip.
uint16_t Pca9535::GetInputs()
{
  uint16_t Inputs;

  I2Chip::I2cWriteUInt8(PCA9535_INPUT_PORT_0, address, buffer, error);
  Inputs = I2Chip::I2cReadUInt16(address, buffer, error);

  return Inputs;
}
   
/// Write OUTPUT_PORT_0 as command and transfer two bytes from the chip.
uint16_t Pca9535::GetOutputs()
{
  uint16_t Outputs;

  I2Chip::I2cWriteUInt8(PCA9535_OUTPUT_PORT_0, address, buffer, error);
  Outputs = I2Chip::I2cReadUInt16(address, buffer, error);

  return Outputs;
}
   
/// Write POLARITY_INVERSION_0 as command and transfer two bytes from the chip.
uint16_t Pca9535::GetPolInversions()
{
  uint16_t Polarities;

  I2Chip::I2cWriteUInt8(PCA9535_POLARITY_INVERSION_0, address, buffer, error);
  Polarities = I2Chip::I2cReadUInt16(address, buffer, error);

  return Polarities;
}

/// Write CONFIG_PORT_0 as command and transfer two bytes from the chip.
uint16_t Pca9535::GetPortConfigs()
{
  uint16_t Configs;

  I2Chip::I2cWriteUInt8(PCA9535_CONFIG_PORT_0, address, buffer, error);
  Configs = I2Chip::I2cReadUInt16(address, buffer, error);

  return Configs;
}

