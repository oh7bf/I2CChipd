/**************************************************************************
 * 
 * Lis3mdl class definitions and constructor. Base class is I2Chip. 
 *       
 * Copyright (C) 2021 - 2022 Jaakko Koivuniemi.
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
 * Fri Sep 10 13:40:47 CDT 2021
 * Edit: Fri Apr 22 19:05:55 CDT 2022
 *
 * Jaakko Koivuniemi
 **/

#ifndef _LIS3MDL_HPP
#define _LIS3MDL_HPP

#include "I2Chip.hpp"

#define LIS3MDL_WHO_AM_I   0x0F
#define LIS3MDL_CTRL_REG1  0x20
#define LIS3MDL_CTRL_REG2  0x21
#define LIS3MDL_CTRL_REG3  0x22
#define LIS3MDL_CTRL_REG4  0x23
#define LIS3MDL_CTRL_REG5  0x24
#define LIS3MDL_STATUS_REG 0x27
#define LIS3MDL_OUT_X_L    0x28
#define LIS3MDL_OUT_X_H    0x29
#define LIS3MDL_OUT_Y_L    0x2A
#define LIS3MDL_OUT_Y_H    0x2B
#define LIS3MDL_OUT_Z_L    0x2C
#define LIS3MDL_OUT_Z_H    0x2D
#define LIS3MDL_TEMP_OUT_L 0x2E
#define LIS3MDL_TEMP_OUT_H 0x2F
#define LIS3MDL_INT_CFG    0x30
#define LIS3MDL_INT_SRC    0x31
#define LIS3MDL_INT_THS_L  0x32
#define LIS3MDL_INT_THS_H  0x33
#define LIS3MDL_MULTI_RW   0x80

/// Class for Lis3mdl inherited from I2Chip base class. 

/// The constructor _Lis3mdl_ sets name tag, device file name and chip address
/// used in data transfer. 
class Lis3mdl : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    int16_t outX = 0; ///< last reading from OUT_X_L and OUT_X_H
    int16_t outY = 0; ///< last reading from OUT_Y_L and OUT_Y_H
    int16_t outZ = 0; ///< last reading from OUT_Z_L and OUT_Z_H
    int16_t temp = 0; ///< last reading from TEMP_OUT_L and TEMP_OUT_H

    double Gain = 6842;    ///< gain@16-bit [LSB/G] last setting  
    double Bx;    ///< Bx[uT] from last reading 
    double By;    ///< By[uT] from last reading 
    double Bz;    ///< Bz[uT] from last reading 
    double T;     ///< T[C] from last reading

   public:
    /// Construct Lis3mdl object with parameters.
    Lis3mdl(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Lis3mdl object with parameters using default address.
    Lis3mdl(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x1C)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Lis3mdl();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get magnetic field value Bx[G] from last reading. 
    double GetBx() { return Bx; }

    /// Get magnetic field value By[G] from last reading. 
    double GetBy() { return By; }

    /// Get magnetic field value Bz[G] from last reading. 
    double GetBz() { return Bz; }

    /// Get temperature value T[C] from last reading.
    double GetT() { return T; }

    /// Get outX from last reading. 
    int16_t GetOutX() { return outX; }

    /// Get outY from last reading. 
    int16_t GetOutY() { return outY; }

    /// Get outZ from last reading. 
    int16_t GetOutZ() { return outZ; }

    /// Get temp from last reading.
    int16_t GetTemp() { return temp; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set I2C bus address 0x1C.
    void SetAddress0x1C() {this->address = 0x1C; }

    /// Set I2C bus address 0x1E.
    void SetAddress0x1E() {this->address = 0x1E; }

    /// Enable temperature sensor.
    void TempEnable();

    /// Disable temperature sensor.
    void TempDisable();

    /// Get X and Y axes operative mode 0 - 3.
    uint8_t GetXYOpMode();

    /// Set X and Y axes operative mode 0 - 3.
    void SetXYOpMode(uint8_t OpModeXY);

    /// Get output data rate 0 - 7.
    uint8_t GetDataRate();

    /// Set output data rate 0 - 7.
    void SetDataRate(uint8_t DataRate);

    /// Enable fast data rate.
    void FastDataEnable();

    /// Disable fast data rate.
    void FastDataDisable();

    /// Enable self test.
    void SelfTestEnable();

    /// Disable self test.
    void SelfTestDisable();

    /// Get full scale configuration 0 - 3.
    uint8_t GetFullScale();

    /// Set full scale configuration 0 - 3.
    void SetFullScale(uint8_t FullScale);

    /// Reboot memory content.
    void Reboot();

    /// Soft reset.
    void SoftReset();

    /// Enable low-power mode.
    void LowPowerEnable();

    /// Disable low-power mode.
    void LowPowerDisable();

    /// Get operating mode.
    uint8_t GetOpMode();

    /// Set operating mode.
    void SetOpMode(uint8_t OpMode);

    /// Power down.
    void PowerDown();

    /// Continuous conversion mode.
    void ContinuousMode();

    /// Single conversion mode.
    void SingleConversionMode(); 

    /// Get Z-axis operation mode 0 - 3.
    uint8_t GetZOpMode();

    /// Set Z-axis operation mode 0 - 3.
    void SetZOpMode(uint8_t ZOpMode);

    /// Set Big Endian mode.
    void BigEndian();

    /// Set Little Endian mode.
    void LittleEndian();

    /// Enable fast read.
    void FastReadEnable();

    /// Disable fast read.
    void FastReadDisable();

    /// Block data update for magnetic data.
    void BlockDataEnable();

    /// Disable block data update for magnetic data.
    void BlockDataDisable();

    /// Get status register.
    uint8_t GetStatus();

    /// Get interrupt configuration register.
    uint8_t GetIntConfig();

    /// Set interrupt configuration register.
    void SetIntConfig(uint8_t IntConf);

    /// Get interrupt source register.
    uint8_t GetIntSource();

    /// Set interrupt source register.
    void SetIntSource(uint8_t IntSource);

    /// Get interrupt threshold 0 - 65535.
    uint16_t GetIntThreshold();

    /// Set interrupt threshold 0 - 65535.
    void SetIntThreshold(uint16_t IntThreshold);

    /// Read chip WHO_AM_I register and return True if 0x3D
    bool WhoAmI();

    /// Read chip OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L and OUT_Z_H registers and return true if success.
    bool ReadB();

    /// Read chip OUT_X_L and OUT_X_H registers and return true if success.
    bool ReadBx();

    /// Read chip OUT_Y_L and OUT_Y_H registers and return true if success.
    bool ReadBy();

    /// Read chip OUT_Z_L and OUT_Z_H registers and return true if success.
    bool ReadBz();

    /// Read chip STATUS_REG and test if bit ZYXDA bit is set.
    bool NewDataXYZ();

    /// Read chip STATUS_REG and test if bit ZYXOR bit is set.
    bool OverRunXYZ();

//    /// Self-test procedure. Return true if success.
//    bool SelfTest();
};

#endif
