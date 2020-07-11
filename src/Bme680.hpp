/**************************************************************************
 * 
 * Bme680 class definitions and constructor. Base class is I2Chip. 
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
 * Thu Jul  9 10:59:30 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _BME680_HPP
#define _BME680_HPP

#include "I2Chip.hpp"

#define BME680_PAR_T1_REG 0xE9
#define BME680_PAR_T2_REG 0x8A
#define BME680_PAR_T3_REG 0x8C

#define BME680_PAR_P1_REG 0x8E
#define BME680_PAR_P2_REG 0x90
#define BME680_PAR_P3_REG 0x92
#define BME680_PAR_P4_REG 0x94
#define BME680_PAR_P5_REG 0x96
#define BME680_PAR_P6_REG 0x99
#define BME680_PAR_P7_REG 0x98
#define BME680_PAR_P8_REG 0x9C
#define BME680_PAR_P9_REG 0x9E
#define BME680_PAR_P10_REG 0xA0

#define BME680_PAR_H1H2_REG 0xE1
#define BME680_PAR_H3_REG 0xE4
#define BME680_PAR_H4_REG 0xE5
#define BME680_PAR_H5_REG 0xE6
#define BME680_PAR_H6_REG 0xE7
#define BME680_PAR_H7_REG 0xE8

#define BME680_PAR_G1_REG 0xED
#define BME680_PAR_G2_REG 0xEB
#define BME680_PAR_G3_REG 0xEE

#define BME680_RES_HEAT_RANGE_REG 0x02
#define BME680_RES_HEAT_VAL_REG 0x00

#define BME680_STATUS_REG 0x73
#define BME680_RESET_REG 0xE0
#define BME680_ID_REG 0xD0
#define BME680_CONFIG_REG 0x75
#define BME680_CTRL_MEAS_REG 0x74
#define BME680_CTRL_HUM_REG 0x72
#define BME680_CTRL_GAS1_REG 0x71
#define BME680_CTRL_GAS0_REG 0x70
#define BME680_GAS_WAIT_REG 0x64
#define BME680_GAS_RES_HEAT_REG 0x5A
#define BME680_GAS_IDAC_HEAT_REG 0x50

#define BME680_GAS_R_LSB_REG 0x2B
#define BME680_GAS_R_MSB_REG 0x2A
#define BME680_RANGE_SWITCHING_ERROR_REG 0x04

#define BME680_HUM_LSB_REG 0x26
#define BME680_HUM_MSB_REG 0x25

#define BME680_TEMP_XLSB_REG 0x24
#define BME680_TEMP_LSB_REG 0x23
#define BME680_TEMP_MSB_REG 0x22

#define BME680_PRESS_XLSB_REG 0x21
#define BME680_PRESS_LSB_REG 0x20
#define BME680_PRESS_MSB_REG 0x1F

#define BME680_MEAS_STATUS0_REG 0x1D

/// Class for Bme680 inherited from I2Chip base class. 

/// The constructor _Bme680_ sets name tag, device file name and chip address
/// used in data transfer. 
class Bme680 : public I2Chip 
{
    std::string name;       ///< name tag for chip
    std::string i2cdev;     ///< device file for writing and reading serial data

    uint16_t address;       ///< I2C chip address
    int error;              ///< error flag for I2C communication

    /// buffer to transfer serial data to and from chip
    uint8_t buffer[ BUFFER_MAX ] = { };

    /// calibration data from chip
    uint16_t par_t1;
    int16_t par_t2;
    int8_t  par_t3;

    uint16_t par_p1;
    int16_t par_p2;
    int8_t  par_p3;
    int16_t par_p4;
    int16_t par_p5;
    int8_t  par_p6;
    int8_t  par_p7;
    int16_t par_p8;
    int16_t par_p9;
    uint8_t  par_p10;

    uint16_t par_h1;
    uint16_t par_h2;
    int8_t  par_h3;
    int8_t  par_h4;
    int8_t  par_h5;
    uint8_t  par_h6;
    int8_t  par_h7;

    int8_t  par_g1;
    int16_t par_g2;
    int8_t  par_g3;

    /// temperature in Celsius from last conversion
    int16_t Temperature;

    /// pressure in Pascal from last conversion
    uint32_t Pressure;

    /// humidity in percent
    uint32_t Humidity;

    /// gas resistance in Ohm
    uint32_t Resistance;

   public:
    /// Construct Bme680 object with parameters.
    Bme680(std::string name, std::string i2cdev, uint16_t address) : I2Chip(name, i2cdev, address) 
    {
      this->name = name;
      this->i2cdev = i2cdev;
      this->address = address;
    };

    /// Construct Bme680 object with parameters.
    Bme680(std::string name, std::string i2cdev) : I2Chip(name, i2cdev, 0x77) 
    {
      this->name = name;
      this->i2cdev = i2cdev;
    };

    virtual ~Bme680();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return i2cdev; }

    /// Get I2C chip address in use now.
    uint16_t GetAddress() { return address; }

    /// Get last error number.
    int GetError() { return error; }

    /// Pressure in Pascal from last measurement. 
    double GetPressure();

    /// Temperature in Celsius from last measurement. 
    double GetTemperature();

    /// Humidity in percent from last measurement. 
    double GetHumidity();

    /// Gas resistance in Ohm from last measurement. 
    double GetResistance();

    /// Get ID register value.
    uint8_t GetID();

    /// Get configuration register value.
    uint8_t GetConfig();

    /// Get control measurement register value.
    uint8_t GetControlMeasurement(); 

    /// Get humidity control register value.
    uint8_t GetHumidityControl();

    /// Get gas control register 1 value.
    uint8_t GetGasControl1();

    /// Get gas control register 0 value.
    uint8_t GetGasControl0();

    /// Get meas_status_0 register value.
    uint8_t GetMeasStatus();

    /// Get two power mode bits 0 - 3.
    uint8_t GetMode();

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

    /// Set configuration register value.
    void SetConfig(uint8_t Config); 

    /// Set control measurement register value.
    void SetControlMeasurement(uint8_t CtrlMeas);

    /// Set control humidity register value.
    void SetControlHumidity(uint8_t CtrlHum);

    /// Set control gas 1 register value.
    void SetControlGas1(uint8_t CtrlGas1);

    /// Set control gas 0 register value.
    void SetControlGas0(uint8_t CtrlGas0);

    /// Set three filter bits with 0 - 7.
    void SetFilter(uint8_t Filter);

    /// Set three humidity, temperature and pressure oversampling bits with 0 - 7.
    void SetOverSample(uint8_t HOverSample, uint8_t TOverSample, uint8_t POverSample);

    /// Set two mode bits with 0 - 3.
    void SetMode(uint8_t Mode);

    /// Set four heater profile selection bits with 0 - 9.
    void SetHeaterProfile(uint8_t Profile);

    /// Set gas wait time profile 0 - 9.
    void SetGasWaitTime(uint8_t Profile, uint8_t time);

    /// Set gas heater resistor profile 0 - 9.
    void SetGasHeatResistor(uint8_t Profile, uint8_t R);

    /// Set gas heater current bytes 0 - 9.
    void SetGasHeatCurrent(uint8_t Profile, uint8_t I);

    /// Run gas conversions.
    void RunGas();

    /// Do not run gas conversions.
    void NoGas();

    /// New data is available.
    bool IsNewData();

    /// Gas measuring is going on.
    bool IsGasMeasuring();

    /// Measuring is going on.
    bool IsMeasuring();

    /// Power on reset.
    void Reset();

    /// Sleep mode.
    void Sleep();

    /// Forced mode.
    void Forced();

    /// Turn off current injected to heater.
    void HeaterOff();

    /// Read chip calibration data and return true if success.
    bool GetCalibration();

    /// Read chip temperature, pressure, humidity and resistance registers and convert to Celcius, Pascal, precent and Ohm. Return error code. 
    int GetTPHG();

};

#endif
