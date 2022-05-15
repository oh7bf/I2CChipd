/**************************************************************************
 * 
 * Ltr390uv class definitions and constructor. Base class is I2Chip. 
 *       
 * Copyright (C) 2022 Jaakko Koivuniemi.
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
 * Sun 01 May 2022 01:28:36 PM CDT
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _LTR390UV_HPP
#define _LTR390UV_HPP

#include "I2Chip.hpp"

#define LTR390UV_MAIN_CTRL 0x00
#define LTR390UV_ALS_UVS_MEAS_RATE 0x04
#define LTR390UV_ALS_UVS_GAIN 0x05
#define LTR390UV_PART_ID 0x06
#define LTR390UV_MAIN_STATUS 0x07
#define LTR390UV_ALS_DATA_0 0x0D
#define LTR390UV_ALS_DATA_1 0x0E
#define LTR390UV_ALS_DATA_2 0x0F
#define LTR390UV_UVS_DATA_0 0x10
#define LTR390UV_UVS_DATA_1 0x11
#define LTR390UV_UVS_DATA_2 0x12
#define LTR390UV_INT_CFG 0x19
#define LTR390UV_INT_PTS 0x1A
#define LTR390UV_ALS_UVS_THRES_UP_0 0x21
#define LTR390UV_ALS_UVS_THRES_UP_1 0x22
#define LTR390UV_ALS_UVS_THRES_UP_2 0x23
#define LTR390UV_ALS_UVS_THRES_LOW_0 0x24
#define LTR390UV_ALS_UVS_THRES_LOW_1 0x25
#define LTR390UV_ALS_UVS_THRES_LOW_2 0x26

/// Class for Ltr390uv inherited from I2Chip base class. 

/// The constructor _Ltr390uv_ sets name tag, device file name and chip address
/// used in data transfer. 
class Ltr390uv : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address = 0x53;///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    uint32_t AlsData = 0;   ///< Ambient light data.
    uint32_t UviData = 0;   ///< Ultra-violet light data.
    uint32_t ThrsLow = 0;   ///< UVS/ALS interrupt lower threshold.
    uint32_t ThrsUpper = 0;   ///< UVS/ALS interrupt upper threshold.

    uint8_t AlsGain = 3;    ///< ALS gain last setting.
    double IntTime = 1.0;   ///< Integration time.
    double Wfact = 1.0;     ///< Window factor.
    double UVsensitivity = 2300; ///< Ultraviolet sensitivity.
    double Ambientlight = 0; ///< Abmient light [lux].
    double UVI = 0;         ///< Ultraviolet index.

   public:
    /// Construct Ltr390uv object with parameters using default address.
    Ltr390uv(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x53)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Ltr390uv();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get ambien light value in lux from last reading. 
    double GetAmbientLight() { return Ambientlight; }

    /// Get raw ambien light value from last reading. 
    uint32_t GetAlsData() { return AlsData; }

    /// Get ultra-violet index from last reading. 
    double GetUVI() { return UVI; }

    /// Get raw ultra-violet value from last reading. 
    uint32_t GetUviData() { return UviData; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Get UVS/ALS interrupt lower threshold.
    uint32_t GetThrsLow() { return ThrsLow; }

    /// Get UVS/ALS interrupt upper threshold.
    uint32_t GetThrsUpper() { return ThrsUpper; }

    /// Reset.
    void Reset();

    /// Ambient light mode.
    void AmbientLightMode();

    /// Ultra-violet light mode.
    void UltravioletMode();

    /// ALS/UVS light sensor standby.
    void LightDisable();

    /// ALS/UVS light sensor active.
    void LightEnable();

    /// Get ALS/UVS resolution 0 - 5.
    uint8_t GetResolution();

    /// Set ALS/UVS resolution 0 - 5.
    void SetResolution(uint8_t Resolution);

    /// Get ALS/UVS measurement rate 0 - 7.
    uint8_t GetMeasRate();

    /// Set ALS/UVS measurement rate 0 - 7.
    void SetMeasRate(uint8_t MeasRate);

    /// Get ALS/UVS gain 0 - 4.
    uint8_t GetGain();

    /// Set ALS/UVS gain 0 - 4.
    void SetGain(uint8_t Gain);

    /// Get part ID.
    uint8_t GetID();

    /// Get status register.
    uint8_t GetStatus();

    /// Power-On status.
    bool IsPowerOn();

    /// Interrupt status.
    bool IsInterrupt();

    /// New data available.
    bool NewData();

    /// Read ambient light data and return true if success.
    bool ReadAmbientLight();

    /// Read ultra-violet light data and return true if success.
    bool ReadUltraviolet();

    /// Interrupt from ambient light channel.
    void IntAmbientLight();

    /// Interrupt from ultra-violet light channel.
    void IntUltraviolet();

    /// Light interrupt enabled.
    void IntEnable();

    /// Light interrupt disbaled..
    void IntDisable();

    /// Get interrupt persists 0 - 15.
    uint8_t GetIntPersist();

    /// Set interrupt persists 0 - 15.
    void SetIntPersist(uint8_t IntPersist);

    /// Read interrupt threshold lower value.
    bool ReadThrsLow();

    /// Set interrupt threshold lower value.
    void SetThrsLow(uint32_t ThrsLow);

    /// Read interrupt threshold upper value.
    bool ReadThrsUp();

    /// Set interrupt threshold upper value.
    void SetThrsUp(uint32_t ThrsUp);
};

#endif
