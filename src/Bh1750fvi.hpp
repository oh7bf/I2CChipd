/**************************************************************************
 * 
 * Bh1750fvi class definitions and constructor. Base class is I2Chip. 
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
 * Sun 08 Aug 2021 03:13:39 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _BH1750FVI_HPP
#define _BH1750FVI_HPP

#include "I2Chip.hpp"

#define BH1750FVI_POWER_DOWN 0x00
#define BH1750FVI_POWER_ON 0x01
#define BH1750FVI_RESET 0x07
#define BH1750FVI_CONT_HIGH_RES_MODE 0x10
#define BH1750FVI_CONT_HIGH_RES_MODE2 0x11
#define BH1750FVI_CONT_LOW_RES_MODE 0x13
#define BH1750FVI_ONE_TIME_HIGH_RES_MODE 0x20
#define BH1750FVI_ONE_TIME_HIGH_RES_MODE2 0x21
#define BH1750FVI_ONE_TIME_LOW_RES_MODE 0x23
#define BH1750FVI_CHANGE_MEAS_TIME_HIGH 0x40
#define BH1750FVI_CHANGE_MEAS_TIME_LOW 0x60

/// Class for Bh1750fvi inherited from I2Chip base class. 

/// The constructor _Bh1750fvi_ sets name tag, device file name and chip address
/// used in data transfer. 
class Bh1750fvi : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    uint8_t MTRegister = 69;    ///< MTreg last setting
    double Illuminance;    ///< illuminance from last reading
    bool mode2 = false;    ///< was high resolution mode 2 used?

   public:
    /// Construct Bh1750fvi object with parameters.
    Bh1750fvi(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Bh1750fvi object with parameters using default address.
    Bh1750fvi(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x23)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Bh1750fvi();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get illuminance value in lux from last reading. 
    double GetIlluminance() { return Illuminance; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set I2C bus address 0x23.
    void SetAddress0x23() {this->address = 0x23; }

    /// Set I2C bus address 0x5C.
    void SetAddress0x5C() {this->address = 0x5C; }

    /// Power down.
    void PowerDown();

    /// Power on.
    void PowerOn();

    /// Reset.
    void Reset();

    /// Continuous high resolution mode.
    void ContHighResMode();

    /// Continuous high resolution mode 2.
    void ContHighResMode2();

    /// Continuous low resolution mode.
    void ContLowResMode();

    /// One time high resolution mode.
    void OneTimeHighResMode(); 

    /// One time high resolution mode 2.
    void OneTimeHighResMode2(); 

    /// One time low resolution mode.
    void OneTimeLowResMode(); 

    /// Set measurement time.
    void SetMeasurementTime(uint8_t MTreg);

    /// Read chip illuminance register and return true if success.
    bool ReadIlluminance();

};

#endif
