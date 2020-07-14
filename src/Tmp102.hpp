/**************************************************************************
 * 
 * Tmp102 class definitions and constructor. Base class is I2Chip. 
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
 * Fri Jul  3 20:33:30 CDT 2020
 * Edit: Mon Jul 13 19:21:01 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#ifndef _TMP102_HPP
#define _TMP102_HPP

#include "I2Chip.hpp"

#define TMP102_TEMP_REG 0x00
#define TMP102_CONFIG_REG 0x01
#define TMP102_TEMP_LOW_REG 0x02
#define TMP102_TEMP_HIGH_REG 0x03

/// Class for Tmp102 inherited from I2Chip base class. 

/// The constructor _Tmp102_ sets name tag, device file name and chip address
/// used in data transfer. 
class Tmp102 : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    double Temperature;    ///< temperature from last reading

   public:
    /// Construct Tmp102 object with parameters.
    Tmp102(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Tmp102 object with parameters.
    Tmp102(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x48)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Tmp102();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get temperature value in Celcius from last reading. 
    double GetTemperature() { return Temperature; }

    /// Get high limit register value.
    uint16_t GetHighLimit();

    /// Get low limit register value.
    uint16_t GetLowLimit(); 

    /// Get configuration register value.
    uint16_t GetConfig(); 

    /// Get ALERT pin polarity to 0 or 1.
    int GetPolarity();

    /// Get fault queue value 0 - 3 meaning 1, 2, 4 or 6.
    uint8_t GetFaultQueue();

    /// Get conversion rate 0 - 3 meaning 0.25, 1, 4 or 8 Hz.
    int GetConversionRate();

    /// Get Alert bit.
    int GetAlertBit();

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

    /// Set high limit register value.
    void SetHighLimit(uint16_t THigh);

    /// Set low limit register value.
    void SetLowLimit(uint16_t TLow); 

    /// Set configuration register value.
    void SetConfig(uint16_t Config); 

    /// Shutdown mode to save power.
    void Shutdown();

    /// Continuous conversion mode.
    void Continuous();

    /// Start comparator mode.
    void ComparatorMode(); 

    /// Start interrupt mode.
    void InterruptMode(); 

    /// Set ALERT pin polarity to 0 or 1.
    void SetPolarity(int Pol);

    /// Set fault queue value 0 - 3 meaning 1, 2, 4 or 6.
    void SetFaultQueue(uint16_t FaultQueue);

    /// Set conversion rate 0 - 3 meaning 0.25, 1, 4 or 8 Hz.
    void SetConversionRate(int ConversionRate);

    /// In Shutdown Mode start single temperature conversion.
    void OneShot(); 

    /// Extended mode.
    void ExtendedMode();

    /// Normal mode.
    void NormalMode();

    /// Read chip temperature register and return true if success.
    bool ReadTemperature();

};

#endif
