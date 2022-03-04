/**************************************************************************
 * 
 * Lis3dh class definitions and constructor. Base class is I2Chip. 
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
 * Fri Feb 25 16:10:43 CST 2022
 * Edit:
 *
 *
 * Jaakko Koivuniemi
 **/

#ifndef _LIS3DH_HPP
#define _LIS3DH_HPP

#include "I2Chip.hpp"

#define LIS3DH_STATUS_REG_AUX 0x07
#define LIS3DH_OUT_ADC1_L     0x08
#define LIS3DH_OUT_ADC1_H     0x09
#define LIS3DH_OUT_ADC2_L     0x0A
#define LIS3DH_OUT_ADC2_H     0x0B
#define LIS3DH_OUT_ADC3_L     0x0C
#define LIS3DH_OUT_ADC3_H     0x0D
#define LIS3DH_WHO_AM_I       0x0F
#define LIS3DH_CTRL_REG0      0x1E
#define LIS3DH_TEMP_CFG_REG   0x1F
#define LIS3DH_CTRL_REG1      0x20
#define LIS3DH_CTRL_REG2      0x21
#define LIS3DH_CTRL_REG3      0x22
#define LIS3DH_CTRL_REG4      0x23
#define LIS3DH_CTRL_REG5      0x24
#define LIS3DH_CTRL_REG6      0x25
#define LIS3DH_REFERENCE      0x26
#define LIS3DH_STATUS_REG     0x27
#define LIS3DH_OUT_X_L        0x28
#define LIS3DH_OUT_X_H        0x29
#define LIS3DH_OUT_Y_L        0x2A
#define LIS3DH_OUT_Y_H        0x2B
#define LIS3DH_OUT_Z_L        0x2C
#define LIS3DH_OUT_Z_H        0x2D
#define LIS3DH_FIFO_CTRL_REG  0x2E
#define LIS3DH_FIFO_SRC_REG   0x2F
#define LIS3DH_INT1_CFG       0x30
#define LIS3DH_INT1_SRC       0x31
#define LIS3DH_INT1_THS       0x32
#define LIS3DH_INT1_DURATION  0x33
#define LIS3DH_INT2_CFG       0x34
#define LIS3DH_INT2_SRC       0x35
#define LIS3DH_INT2_THS       0x36
#define LIS3DH_INT2_DURATION  0x37
#define LIS3DH_CLICK_CFG      0x38
#define LIS3DH_CLICK_SRC      0x39
#define LIS3DH_CLICK_THS      0x3A
#define LIS3DH_TIME_LIMIT     0x3B
#define LIS3DH_TIME_LATENCY   0x3C
#define LIS3DH_TIME_WINDOW    0x3D
#define LIS3DH_ACT_THS        0x3E
#define LIS3DH_ACT_DUR        0x3F
#define LIS3DH_MULTI_RW       0x80

/// Class for Lis3dh inherited from I2Chip base class. 

/// The constructor _Lis3dh_ sets name tag, device file name and chip address
/// used in data transfer. 
class Lis3dh : public I2Chip 
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
    uint16_t Adc1 = 0; ///< last reading from OUT_ADC1_L and OUT_ADC1_H
    uint16_t Adc2 = 0; ///< last reading from OUT_ADC2_L and OUT_ADC2_H
    uint16_t Adc3 = 0; ///< last reading from OUT_ADC3_L and OUT_ADC3_H
    int FS = 2;    ///< Full-scale 2, 4, 8 or 16 g last setting  
    int So = 4; ///< Operation mode sensitivity [mg/digit] last setting  
    double gx;    ///< g-force x from last reading 
    double gy;    ///< g-force y from last reading 
    double gz;    ///< g-force z from last reading 

   public:
    /// Construct Lis3dh object with parameters.
    Lis3dh(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address)
   {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
   };

    /// Construct Lis3dh object with parameters using default address.
    Lis3dh(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x18)
   {
      this->name = name;
      this->i2cdev = i2cdev;
   };

    virtual ~Lis3dh();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Get g-force x from last reading. 
    double Getgx() { return gx; }

    /// Get g-force y from last reading. 
    double Getgy() { return gy; }

    /// Get g-force z from last reading. 
    double Getgz() { return gz; }

    /// Get outX from last reading. 
    int16_t GetOutX() { return outX; }

    /// Get outY from last reading. 
    int16_t GetOutY() { return outY; }

    /// Get outZ from last reading. 
    int16_t GetOutZ() { return outZ; }

    /// Get ADC1 from last reading. 
    uint16_t GetAdc1() { return Adc1; }

    /// Get ADC2 from last reading. 
    uint16_t GetAdc2() { return Adc2; }

    /// Get ADC3 from last reading. 
    uint16_t GetAdc3() { return Adc3; }

    /// Get FS (full-scale) constant.
    int GetFS() { return FS; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string i2cdev) { this->i2cdev = i2cdev; }

    /// Set I2C bus address 0x18.
    void SetAddress0x18() {this->address = 0x18; }

    /// Set I2C bus address 0x19.
    void SetAddress0x19() {this->address = 0x19; }

    /// Read chip WHO_AM_I register and return True if 0x33
    bool WhoAmI();

    /// Get status register.
    uint8_t GetStatus();

    /// Get reference register.
    uint8_t GetReference();

    /// Get CTRL_REG1 register.
    uint8_t GetCtrlReg1();

    /// Read chip OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L and OUT_Z_H registers and return true if success.
    bool Readg();

    /// Read chip FIFO and return number of values 0 - 32.
    int8_t ReadFifo();

    /// Read chip OUT_ADC1_L, OUT_ADC1_H, OUT_ADC2_L, OUT_ADC2_H, OUT_ADC3_L and OUT_ADC3_H registers and return true if success.
    bool ReadAdc();

    /// Enable temperature sensor.
    void TempEnable();

    /// Disable temperature sensor.
    void TempDisable();

    /// Enable ADC.
    void ADCEnable();

    /// Disable ADC.
    void ADCDisable();

    /// Get output data rate 0 - 9.
    uint8_t GetDataRate();

    /// Set output data rate 0 - 9.
    void SetDataRate(uint8_t DataRate);

    /// X-axis enable.
    void XEnable();

    /// X-axis disable.
    void XDisable();

    /// Y-axis enable.
    void YEnable();

    /// Y-axis disable.
    void YDisable();

    /// Z-axis enable.
    void ZEnable();

    /// Z-axis disable.
    void ZDisable();

    /// Get high-pass filter mode 0 - 3.
    uint8_t GetHpm();

    /// Set high-pass filter mode 0 - 3.
    void SetHpm(uint8_t Hpm);

    /// Get high-pass filter cutoff frequency 0 - 3.
    uint8_t GetHpcf();

    /// Set high-pass filter cutoff frequency 0 - 3.
    void SetHpcf(uint8_t Hpcf);

    /// Enable high-pass filter for click.
    void HpClickEnable();

    /// Disable high-pass filter for click.
    void HpClickDisable();

    /// Bypass internal filter.
    void FdBypass();

    /// Internal filter data to output register and FIFO.
    void FdSelect();

    /// Enable high-pass filter AOI function on interrupt 1.
    void HpIa1Enable(); 

    /// Bypass high-pass filter AOI function on interrupt 1.
    void HpIa1Bypass(); 

    /// Enable high-pass filter AOI function on interrupt 2.
    void HpIa2Enable(); 

    /// Bypass high-pass filter AOI function on interrupt 2.
    void HpIa2Bypass(); 

    /// Block data update for g-force data.
    void BlockDataEnable();

    /// Disable block data update for g-force data.
    void BlockDataDisable();

    /// Get full scale configuration 0 - 3.
    uint8_t GetFullScale();

    /// Set full scale configuration 0 - 3.
    void SetFullScale(uint8_t FullScale);

    /// Set Big Endian mode.
    void BigEndian();

    /// Set Little Endian mode.
    void LittleEndian();

    /// Set low-power mode with 8-bit data output.
    void LowPowerMode();

    /// Set normal mode with 10-bit data output.
    void NormalMode();

    /// Set high resolution mode with 12-bit data output.
    void HighResolutionMode();

    /// Set self-test mode 0 - 3.
    void SetSelfTest(uint8_t STMode);

    /// Reboot memory content.
    void Boot();

    /// Enable FIFO.
    void FifoEnable();

    /// Disable FIFO.
    void FifoDisable();

    /// Get FIFO mode 0 -3.
    uint8_t GetFifoMode();

    /// Set FIFO mode 0 -3.
    void SetFifoMode(uint8_t FifoMode);

    /// Stream-to-FIFO mode change from pin INT1.
    void StreamFifoInt1();

    /// Stream-to-FIFO mode change from pin INT2.
    void StreamFifoInt2();

    /// Get FIFO watermark level 0 - 31.
    uint8_t GetWatermark();

    /// Set FIFO watermark level 0 - 31.
    void SetWatermark(uint8_t Wtm);

    /// Has FIFO watermark level been reached?
    bool Watermark();

    /// FIFO overrun?
    bool Overrun();

    /// FIFO empty?
    bool Empty();

    /// Get current number of unread samples in FIFO 0 - 31.
    uint8_t Samples();

    /// Read chip STATUS_REG and test if bit ZYXDA bit is set.
    bool NewDataXYZ();

    /// Read chip STATUS_REG and test if bit ZYXOR bit is set.
    bool OverRunXYZ();

    /// Click interrupt on INT1.
    void ClickInt1Enable();

    /// Disable click interrupt on INT1.
    void ClickInt1Disable();

    /// IA1 interrupt on INT1.
    void Ia1Int1Enable();

    /// Disable IA1 interrupt on INT1.
    void Ia1Int1Disable();

    /// IA2 interrupt on INT1.
    void Ia2Int1Enable();

    /// Disable IA2 interrupt on INT1.
    void Ia2Int1Disable();

    /// ZYXDA interrupt on INT1.
    void ZYXDaInt1Enable();

    /// Disable ZYXDA interrupt on INT1.
    void ZYXDaInt1Disable();

    /// 321DA interrupt on INT1.
    void Da321Int1Enable();

    /// Disable 321DA interrupt on INT1.
    void Da321Int1Disable();

    /// FIFO watermark interrupt on INT1.
    void WtmInt1Enable();

    /// Disable FIFO watermark interrupt on INT1.
    void WtmInt1Disable();

    /// FIFO overrun interrupt on INT1.
    void OverrunInt1Enable();

    /// Disable FIFO overrun interrupt on INT1.
    void OverrunInt1Disable();

    /// Click interrupt on INT2.
    void ClickInt2Enable();

    /// Disable click interrupt on INT2.
    void ClickInt2Disable();

    /// IA1 interrupt on INT2.
    void Ia1Int2Enable();

    /// Disable IA1 interrupt on INT2.
    void Ia1Int2Disable();

    /// IA2 interrupt on INT2.
    void Ia2Int2Enable();

    /// Disable IA2 interrupt on INT2.
    void Ia2Int2Disable();

    /// Boot on INT2 pin.
    void Int2BootEnable();

    /// Disable Boot on INT2 pin.
    void Int2BootDisable();

    /// Activity interrupt on INT2 pin.
    void ActivityInt2Enable();

    /// Disable activity interrupt on INT2 pin.
    void ActivityInt2Disable();

    /// Set INT1 and INT2 pin polarity to active high.
    void IntActiveHigh();

    /// Set INT1 and INT2 pin polarity to active low.
    void IntActiveLow();

    /// Enable interrupt double tap on Z-axis.
    void DoubleTapZEnable();

    /// Disable interrupt double tap on Z-axis.
    void DoubleTapZDisable();

    /// Enable interrupt single tap on Z-axis.
    void SingleTapZEnable();

    /// Disable interrupt single tap on Z-axis.
    void SingleTapZDisable();

    /// Enable interrupt double tap on Y-axis.
    void DoubleTapYEnable();

    /// Disable interrupt double tap on Y-axis.
    void DoubleTapYDisable();

    /// Enable interrupt single tap on Y-axis.
    void SingleTapYEnable();

    /// Disable interrupt single tap on Y-axis.
    void SingleTapYDisable();

    /// Enable interrupt double tap on X-axis.
    void DoubleTapXEnable();

    /// Disable interrupt double tap on X-axis.
    void DoubleTapXDisable();

    /// Enable interrupt single tap on X-axis.
    void SingleTapXEnable();

    /// Disable interrupt single tap on X-axis.
    void SingleTapXDisable();

    /// Click interrupt active.
    bool ClickInt();

    /// Double-click detection enabled.
    bool DoubleClick();

    /// Single-click detection enabled.
    bool SingleClick();

    /// Click sign.
    bool ClickSign();

    /// Z high event has occured.
    bool ClickZ();

    /// Y high event has occured.
    bool ClickY();

    /// X high event has occured.
    bool ClickX();

    /// Get click threshold 0 - 127.
    uint8_t GetClickThs();

    /// Set click threshold 0 - 127.
    void SetClickThs(uint8_t Ths);

    /// Latch click interrupt.
    void ClickLatchEnable();

    /// Disable click interrupt latching.
    void ClickLatchDisable();

    /// Set click time limit 0 - 127.
    void SetClickTlimit(uint8_t Tli);

    /// Set click time latency 0 - 255.
    void SetClickTlatency(uint8_t Tla);

    /// Set click time window 0 - 255.
    void SetClickTwindow(uint8_t Tw);

    /// Get activity threshold 0 - 127.
    uint8_t GetActThreshold();

    /// Set activity threshold 0 - 127.
    void SetActThreshold(uint8_t Ths);

    /// Get activity duration 0 - 255.
    uint8_t GetActDuration();

    /// Set activity duration 0 - 255.
    void SetActDuration(uint8_t Dur);

    /// Self-test procedure. Return true if success.
    bool SelfTest();
};

#endif
