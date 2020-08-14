/**************************************************************************
 * 
 * Ads1015 class definitions and constructor. Base class is I2Chip. 
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
 * Sat Aug  8 19:34:10 CDT 2020
 * Edit: Thu 13 Aug 2020 07:44:35 PM CDT
 *
 * Jaakko Koivuniemi
 **/

#ifndef _ADS1015_HPP
#define _ADS1015_HPP

#include "I2Chip.hpp"

#define ADS1015_CONV_REG 0x00
#define ADS1015_CONFIG_REG 0x01
#define ADS1015_LOW_THRESH_REG 0x02
#define ADS1015_HIGH_THRESH_REG 0x03

/// Class for Ads1015 inherited from I2Chip base class. 

/// The constructor _Ads1015_ sets name tag, device file name and chip address
/// used in data transfer. 
class Ads1015 : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    double Voltage;    ///< voltage from last reading
    uint16_t pga;      ///< programmable gain from last setting
    int16_t adc;       ///< conversion register value from last reading

   public:
    /// Construct Ads1015 object with parameters.
    Ads1015(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Ads1015 object with parameters.
    Ads1015(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x49)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Ads1015();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get voltage from last reading. 
    double GetVoltage() { return Voltage; }

    /// Get programmable gain amplifier setting 0 - 7.
    uint16_t GetPGA(); 

    /// Get input multiplexer setting 0 - 7.
    uint16_t GetMux();

    /// Get high threshold register value.
    int16_t GetHighThreshold();

    /// Get low threshold register value.
    int16_t GetLowThreshold(); 

    /// Get configuration register value.
    uint16_t GetConfig(); 

    /// Get comparator comparator mode 0 or 1.
    uint16_t GetCompMode();

    /// Get comparator pin polarity to 0 or 1.
    uint16_t GetCompPolarity();

    /// Get comparator latching setting 0 or 1.
    uint16_t GetCompLatching();

    /// Get comparator queue value 0 - 3.
    uint16_t GetCompQueue();

    /// Get data rate setting 0 - 7.
    uint16_t GetDataRate();

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set I2C bus address 0x48.
    void SetAddress0x48() {this->address = 0x48; }

    /// Set I2C bus address 0x49.
    void SetAddress0x49() {this->address = 0x49; }

    /// Set I2C bus address 0x4A.
    void SetAddress0x4A() {this->address = 0x4A; }

    /// Set I2C bus address 0x4B.
    void SetAddress0x4B() {this->address = 0x4B; }

    /// Set pointer register.
    void SetPointer(uint8_t PointerReg);

    /// Set high threshold register value.
    void SetHighThreshold(int16_t HighThreshold);

    /// Set low limit register value.
    void SetLowThreshold(int16_t LowThreshold); 

    /// Set configuration register value.
    void SetConfig(uint16_t Config); 

    /// Set programmable gain amplifier 0 - 7.
    void SetPGA(uint16_t PGA);

    /// Set input multiplexer 0 - 7.
    void SetMux(uint16_t Mux);

    /// Set comparator pin polarity to 0 or 1.
    void SetCompPolarity(uint16_t CompPolarity);

    /// Set comparator latching setting 0 or 1.
    void SetCompLatching(uint16_t CompLatching);

    /// Set comparator mode setting 0 or 1.
    void SetCompMode(uint16_t CompMode);

    /// Set comparator queue value 0 - 3.
    void SetCompQueue(uint16_t CompQueue);

    /// Set data rate setting 0 - 7.
    void SetDataRate(uint16_t DataRate);

    /// Powerdown mode to save power.
    void Powerdown();

    /// Continuous conversion mode.
    void Continuous();

    /// In Powerdown Mode start single conversion.
    void SingleConversion(); 

    /// Read chip conversion register and return true if success.
    bool ReadConversion();

};

#endif
