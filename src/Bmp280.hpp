/**************************************************************************
 * 
 * Bmp280 class definitions and constructor. Base class is I2Chip. 
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
 * Tue 07 Jul 2020 11:08:43 AM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _BMP280_HPP
#define _BMP280_HPP

#include "I2Chip.hpp"

#define BMP280_DIG_T1_REG 0x88
#define BMP280_DIG_T2_REG 0x8A
#define BMP280_DIG_T3_REG 0x8C
#define BMP280_DIG_P1_REG 0x8E
#define BMP280_DIG_P2_REG 0x90
#define BMP280_DIG_P3_REG 0x92
#define BMP280_DIG_P4_REG 0x94
#define BMP280_DIG_P5_REG 0x96
#define BMP280_DIG_P6_REG 0x98
#define BMP280_DIG_P7_REG 0x9A
#define BMP280_DIG_P8_REG 0x9C
#define BMP280_DIG_P9_REG 0x9E

#define BMP280_TEMP_XLSB_REG 0xFC
#define BMP280_TEMP_LSB_REG 0xFB
#define BMP280_TEMP_MSB_REG 0xFA
#define BMP280_PRESS_XLSB_REG 0xF9
#define BMP280_PRESS_LSB_REG 0xF8
#define BMP280_PRESS_MSB_REG 0xF7
#define BMP280_CONFIG_REG 0xF5
#define BMP280_CTRL_MEAS_REG 0xF4
#define BMP280_STATUS_REG 0xF3
#define BMP280_RESET_REG 0xE0
#define BMP280_ID_REG 0xD0

/// Class for Bmp280 inherited from I2Chip base class. 

/// The constructor _Bmp280_ sets name tag, device file name and chip address
/// used in data transfer. 
class Bmp280 : public I2Chip 
{
    std::string name;       ///< name tag for chip
    std::string i2cdev;     ///< device file for writing and reading serial data

    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    /// calibration data from chip
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    /// temperature parameter from last measurement used in pressure calculation
//    int32_t tfine;

   public:
    /// Construct Bmp280 object with parameters.
    Bmp280(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address) 
    {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
    };

    /// Construct Bmp280 object with parameters.
    Bmp280(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x77) 
    {
      this->name = name;
      this->i2cdev = i2cdev;
    };

    virtual ~Bmp280();

    /// temperature parameter from last measurement used in pressure calculation
    int32_t tfine;

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get ID register value.
    uint8_t GetID();

    /// Get status register value.
    uint8_t GetStatus(); 

    /// Get control measurement register value.
    uint8_t GetControlMeasurement(); 

    /// Get configuration register value.
    uint8_t GetConfig();

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set I2C bus address 0x76.
    void SetAddress0x76() {this->address = 0x76; }

    /// Set I2C bus address 0x77.
    void SetAddress0x77() {this->address = 0x77; }

    /// Set pointer register.
    void SetPointer(uint8_t PointerReg);

    /// Set control measurement register value.
    void SetControlMeasurement(uint8_t CtrlMeas);

    /// Set configuration register value.
    void SetConfig(uint8_t Config); 

    /// Set three temperature oversampling bits with 0 - 7.
    void SetTOverSample(uint8_t TOverSample);

    /// Set three pressure oversampling bits with 0 - 7.
    void SetPOverSample(uint8_t POverSample);

    /// Set two power mode bits with 0 - 3.
    void SetPowerMode(uint8_t PowerMode);

    /// Set three inactive duration bits with 0 - 7.
    void SetStandby(uint8_t Standby);

    /// Set three filter bits with 0 - 7.
    void SetFilter(uint8_t Filter);

    /// Measurement going on.
    bool IsMeasuring();

    /// Update going on.
    bool IsUpdate();

    /// Power on reset.
    void Reset();

    /// Sleep mode.
    void Sleep();

    /// Normal mode.
    void Normal();

    /// Forced mode.
    void Forced();

    /// Read chip calibration data and return true if success.
    bool GetCalibration();

    /// Read chip temperature register and return value in integer Celcius. 
    int32_t GetTemperature();

    /// Read chip pressure register and return value in Pascal. 
    uint32_t GetPressure();

};

#endif
