/**************************************************************************
 * 
 * Lis2mdl class definitions and constructor. Base class is I2Chip. 
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
 * Fri 25 Mar 2022 05:38:47 PM CET
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _LIS2MDL_HPP
#define _LIS2MDL_HPP

#include "I2Chip.hpp"

#define LIS2MDL_OFFSET_X_REG_L  0x45
#define LIS2MDL_OFFSET_X_REG_H  0x46
#define LIS2MDL_OFFSET_Y_REG_L  0x47
#define LIS2MDL_OFFSET_Y_REG_H  0x48
#define LIS2MDL_OFFSET_Z_REG_L  0x49
#define LIS2MDL_OFFSET_Z_REG_H  0x4A
#define LIS2MDL_WHO_AM_I        0x4F
#define LIS2MDL_CFG_REG_A       0x60
#define LIS2MDL_CFG_REG_B       0x61
#define LIS2MDL_CFG_REG_C       0x62
#define LIS2MDL_INT_CTRL_REG    0x63
#define LIS2MDL_INT_SOURCE_REG  0x64
#define LIS2MDL_INT_THS_L_REG   0x65
#define LIS2MDL_INT_THS_H_REG   0x66
#define LIS2MDL_STATUS_REG      0x67
#define LIS2MDL_OUTX_L_REG      0x68
#define LIS2MDL_OUTX_H_REG      0x69
#define LIS2MDL_OUTY_L_REG      0x6A
#define LIS2MDL_OUTY_H_REG      0x6B
#define LIS2MDL_OUTZ_L_REG      0x6C
#define LIS2MDL_OUTZ_H_REG      0x6D
#define LIS2MDL_TEMP_OUT_L_REG  0x6E
#define LIS2MDL_TEMP_OUT_H_REG  0x6F

#define LIS2MDL_MULTI_RW   0x80

/// Class for Lis2mdl inherited from I2Chip base class. 

/// The constructor _Lis2mdl_ sets name tag, device file name and chip address
/// used in data transfer. 
class Lis2mdl : public I2Chip 
{
    std::string name;       ///< name tag for chip

    std::string i2cdev;     ///< device file for writing and reading serial data
    uint16_t address = 0x1E;///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    int16_t outX = 0; ///< last reading from OUTX_L_REG and OUTX_H_REG
    int16_t outY = 0; ///< last reading from OUTY_L_REG and OUTY_H_REG
    int16_t outZ = 0; ///< last reading from OUTZ_L_REG and OUTZ_H_REG
    double Gain = 666.66;    ///< gain@16-bit [LSB/G]
    double Bx;    ///< Bx[uT] from last reading 
    double By;    ///< By[uT] from last reading 
    double Bz;    ///< Bz[uT] from last reading 

   public:
    /// Construct Lis2mdl object with parameters using default address.
    Lis2mdl(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x1E)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Lis2mdl();

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

    /// Get outX from last reading. 
    uint16_t GetOutX() { return outX; }

    /// Get outY from last reading. 
    uint16_t GetOutY() { return outY; }

    /// Get outZ from last reading. 
    uint16_t GetOutZ() { return outZ; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Get X offset.
    int16_t GetXOffset();

    /// Set X offset.
    void SetXOffset(int16_t offset);

    /// Get Y offset.
    int16_t GetYOffset();

    /// Set Y offset.
    void SetYOffset(int16_t offset);

    /// Get Z offset.
    int16_t GetZOffset();

    /// Set Z offset.
    void SetZOffset(int16_t offset);

    /// Read chip WHO_AM_I register and return True if 0x40
    bool WhoAmI();

    /// Enable magnetometer temperature compensation.
    void TempCompEnable();

    /// Disable magnetometer temperature compensation.
    void TempCompDisable();

    /// Reboot memory content.
    void Reboot();

    /// Soft reset.
    void SoftReset();

    /// Enable low-power mode.
    void LowPowerEnable();

    /// Disable low-power mode.
    void LowPowerDisable();

    /// Get output data rate 0 - 3.
    uint8_t GetDataRate();

    /// Set output data rate 0 - 3.
    void SetDataRate(uint8_t DataRate);

    /// Get operating mode.
    uint8_t GetOpMode();

    /// Set operating mode.
    void SetOpMode(uint8_t OpMode);

    /// Idle mode.
    void IdleMode(); 

    /// Single conversion mode.
    void SingleMode(); 

    /// Continuous conversion mode.
    void ContinuousMode(); 

    /// Enable offset cancellation in single measurement mode.
    void OffsetCancelSingleEnable();

    /// Disable offset cancellation in single measurement mode.
    void OffsetCancelSingleDisable();

    /// Enable offset cancellation.
    void OffsetCancelEnable();

    /// Disable offset cancellation.
    void OffsetCancelDisable();

    /// Enable low-pass filter.
    void LowPassEnable();
 
    /// Disable low-pass filter.
    void LowPassDisable();
 
    /// Block data update for magnetic data.
    void BlockDataEnable();

    /// Disable block data update for magnetic data.
    void BlockDataDisable();

    /// Set Big Endian mode.
    void BigEndian();

    /// Set Little Endian mode.
    void LittleEndian();

    /// Get status register.
    uint8_t GetStatus();

    /// Get interrupt configuration register.
    uint8_t GetIntConfig();

    /// Set interrupt configuration register.
    void SetIntConfig(uint8_t IntConf);

    /// Get interrupt source register.
    uint8_t GetIntSource();

    /// Get interrupt threshold 0 - 65535.
    uint16_t GetIntThreshold();

    /// Set interrupt threshold 0 - 65535.
    void SetIntThreshold(uint16_t IntThreshold);

    /// Read chip OUTX_L_REG, OUTX_H_REG, OUTY_L_REG, OUTY_H_REG, OUTZ_L_REG and OUTZ_H_REG registers and return true if success.
    bool ReadB();

    /// Read chip OUTX_L_REG and OUTX_H_REG registers and return true if success.
    bool ReadBx();

    /// Read chip OUTY_L_REG and OUTY_H_REG registers and return true if success.
    bool ReadBy();

    /// Read chip OUTZ_L_REG and OUTZ_H_REG registers and return true if success.
    bool ReadBz();

    /// Read chip STATUS_REG and test if bit ZYXDA bit is set.
    bool NewDataXYZ();

    /// Read chip STATUS_REG and test if bit ZYXOR bit is set.
    bool OverRunXYZ();

    /// Enable self-test.
    void SelfTestEnable();

    /// Disable self-test.
    void SelfTestDisable();

    /// Self-test procedure. Return true if success.
//    bool SelfTest();
};

#endif
