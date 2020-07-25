/**************************************************************************
 * 
 * Htu21d class definitions and constructor. Base class is I2Chip. 
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
 * Wed Jul 15 15:06:22 CDT 2020
 * Edit: Fri Jul 24 19:21:15 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#ifndef _HTU21D_HPP
#define _HTU21D_HPP

#include "I2Chip.hpp"

#define HTU21D_ADDRESS 0x40
#define HTU21D_WRITE_USER_REG 0xE6
#define HTU21D_READ_USER_REG 0xE7
#define HTU21D_TRIG_TEMP_MEAS 0xF3
#define HTU21D_TRIG_HUM_MEAS 0xF5
#define HTU21D_SOFT_RESET 0xFE

/// Class for Htu21d inherited from I2Chip base class. 

/// The constructor _Htu21d_ sets name tag and device file name.
class Htu21d : public I2Chip 
{
    std::string name;       ///< name tag for chip
    std::string i2cdev;     ///< device file for writing and reading serial data
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    /// raw temperature and humidity ADC values and CRC received
    uint16_t tadc;
    uint16_t hadc;
    uint8_t  checksum;

    /// CRC valid flag
    bool crc;

    /// temperature in Celsius from last convenversion
    double Temperature;

    /// humidity in percent from last conversion
    double Humidity;

   public:
    /// Construct Htu21d object with parameters.
    Htu21d(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, HTU21D_ADDRESS)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Htu21d();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get temperature value in Celcius from last reading. 
    double GetTemperature() { return Temperature; }

    /// Get humidity value in percent from last reading. 
    double GetHumidity() { return Humidity; }

    /// Raw temperature register value.
    uint16_t GetTReg() { return tadc; }

    /// Raw humidity register value.
    uint16_t GetHReg() { return hadc; }

    /// Checksum value.
    uint8_t GetChecksum() { return checksum; }
    
    /// Return CRC valid flag from last reading.
    bool IsCRCValid() { return crc; }

    /// Read user register value.
    uint8_t GetUserRegister();

    /// Start on-chip heater.
    void HeaterOn(); 

    /// Stop on-chip heater.
    void HeaterOff(); 

    /// Soft reset.
    void Reset(); 

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set two resolution bits with 0 - 3.
    void SetResolution(uint8_t Resolution);

    /// Trigger temperature measurement.
    void TriggerTemperature();

    /// Trigger humidity measurement.
    void TriggerHumidity();

    /// Read chip temperature register and return true if success.
    bool ReadTemperature();

    /// Read chip humidity register and return true if success.
    bool ReadHumidity();

    /// Return true if supply voltage VDD < 2.25 V.
    bool IsEndBattery();

};

#endif
