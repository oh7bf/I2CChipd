/**************************************************************************
 * 
 * I2Chip class definitions and constructor. 
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
 * Fri Jul  3 11:54:51 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _I2CHIP_HPP
#define _I2CHIP_HPP 

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <systemd/sd-daemon.h>
#include <string>

#define I2LOCK_MAX 10        ///< Maximum number of times I2C device file locking is attempted. 
#define BUFFER_MAX 256  ///< Maximum size for I2C read-write buffer.

/// Class for chips with Inter-Integrated Circuit interface (I2C).

/// The constructor _I2Chip()_ takes name tag, device file for reading
/// and writing serial data, 

class I2Chip
{
    std::string name;   ///< name tag for chip
    std::string i2cdev; ///< device file to read and write serial data
    uint16_t address; ///< I2C chip address
    int error;       ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

  public:
    /// Construct I2Chips object.
    I2Chip();

    /// Construct I2Chips object with parameters needed to talk to chip.
    I2Chip(std::string name, std::string i2cdev, uint16_t address);

    virtual ~I2Chip();

    /// Get I2C chip name tag.
    std::string GetName() { return name; }

    /// Get I2C chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Set I2C chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set I2C chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Read one byte from I2C address and return unsigned value.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to read
    /// from slave and -5 more or less data received than expected.
    uint8_t I2cReadUInt8(uint16_t address, uint8_t *buffer, int & error);

    /// Read two bytes from I2C address and return unsigned value.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// The first byte in read buffer is the most significant byte in conversion
    /// to uint16_t data type.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to read
    /// from slave and -5 more or less data received than expected.
    uint16_t I2cReadUInt16(uint16_t address, uint8_t *buffer, int & error);

    /// Read four bytes from I2C address and return unsigned value.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// The first byte in read buffer is the most significant byte in conversion
    /// to uint32_t data type.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to read
    /// from slave and -5 more or less data received than expected.
    uint32_t I2cReadUInt32(uint16_t address, uint8_t *buffer, int & error);

    /// Read N bytes from I2C address and return values in buffer.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to read
    /// from slave and -5 more or less data received than expected.
    void I2cReadBytes(int Nbytes, uint16_t address, uint8_t *buffer, int & error);

    /// Write one byte to I2C address.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to write 
    /// to slave and -5 more or less data transfered than expected.
    void I2cWriteUInt8(uint8_t data, uint16_t address, uint8_t *buffer, int & error);

    /// Write data to 8-bit register on I2C address.

    /// The chip I2C address and pointer to buffer are given as parameters
    /// with 8-bit register pointer and 8-bit data.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to write 
    /// to slave and -5 more or less data transfered than expected.
    void I2cWriteRegisterUInt8(uint8_t reg, uint8_t data, uint16_t address, uint8_t *buffer, int & error);

    /// Write data to 16-bit register on I2C address.

    /// The chip I2C address and pointer to buffer are given as parameters
    /// with 8-bit register pointer and 16-bit data.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to write 
    /// to slave and -5 more or less data transfered than expected.
    void I2cWriteRegisterUInt16(uint8_t reg, uint16_t data, uint16_t address, uint8_t *buffer, int & error);

    /// Write N bytes from buffer to I2C address.

    /// The chip I2C address and pointer to buffer are given as parameters.
    /// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
    /// -3 unable to get bus access to talk to slave, -4 unable to read
    /// from slave and -5 more or less data transfered than expected.
    void I2cWriteBytes(int Nbytes, uint16_t address, uint8_t *buffer, int & error);

};

#endif
