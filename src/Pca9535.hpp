/**************************************************************************
 * 
 * Pca9535 class definitions and constructor. Base class is I2Chip. 
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
 * Fri  3 Nov 12:44:51 CDT 2023 
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _PCA9535_HPP
#define _PCA9535_HPP

#include "I2Chip.hpp"

#define PCA9535_INPUT_PORT_0 0x00
#define PCA9535_INPUT_PORT_1 0x01
#define PCA9535_OUTPUT_PORT_0 0x02
#define PCA9535_OUTPUT_PORT_1 0x03
#define PCA9535_POLARITY_INVERSION_0 0x04
#define PCA9535_POLARITY_INVERSION_1 0x05
#define PCA9535_CONFIG_PORT_0 0x06
#define PCA9535_CONFIG_PORT_1 0x07

/// Class for Pca9535 inherited from I2Chip base class. 

/// The constructor _Pca9535_ sets name tag, device file name and chip address
/// used in data transfer. 
class Pca9535 : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address = 0x20;///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

   public:
    /// Construct Pca9535 object with parameters.
    Pca9535(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Pca9535 object with parameters using default address.
    Pca9535(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x20)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Pca9535();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Get port 0 and 1 inputs.
    uint16_t GetInputs();
   
    /// Get port 0 and 1 output registers.
    uint16_t GetOutputs();
   
    /// Get port 0 and 1 polarity inversion registers.
    uint16_t GetPolInversions();
   
    /// Get port 0 and 1 configuration registers.
    uint16_t GetPortConfigs();
};

#endif
