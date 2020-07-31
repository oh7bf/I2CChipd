/**************************************************************************
 * 
 * Max31865 class definitions and constructor. Base class is SPIChip. 
 *       
 * Copyright (C) 2018 - 2020 Jaakko Koivuniemi.
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
 * Sat  3 Nov 20:21:27 CDT 2018
 * Edit: Sun  4 Nov 19:28:55 CST 2018
 *
 * Jaakko Koivuniemi
 **/

#ifndef _MAX31865_HPP
#define _MAX31865_HPP

#include "SPIChip.hpp"

#define MAX31865_MODE SPI_CPHA
#define MAX31865_BITS 8
#define MAX31865_DELAY 0
#define MAX31865_CONFIG_READ 0x00
#define MAX31865_CONFIG_WRITE 0x80
#define MAX31865_RTD_READ 0x01
#define MAX31865_RTDLSB_READ 0x02
#define MAX31865_HIGHFAULT_READ 0x03
#define MAX31865_HIGHFAULT_WRITE 0x83
#define MAX31865_LOWFAULT_READ 0x05
#define MAX31865_LOWFAULT_WRITE 0x85
#define MAX31865_FAULTSTATUS_READ 0x07
#define MAX31865_BIAS_ON 0x80
#define MAX31865_BIAS_OFF 0x00
#define MAX31865_ONE_SHOT_BIAS_ON 0xA0
#define MAX31865_AUTO_FAULT_DETECTION 0x84

/// Class for Max31865 inherited from SPIChip base class. 

/// The constructor _Max31865_ sets name tag, device file name and clock
/// frequency used in data transfer. The reference resistor scales 
/// the A/D-conversion result to measured resistance value. 
class Max31865 : public SPIChip 
{
    std::string name;       ///< name tag for chip
    std::string device;     ///< device file for writing and reading serial data 
    uint32_t clock;         ///< clock rate [Hz]
    double   Rref;          ///< reference resistor for A/D-converter [ohm]

  public:
    /// Construct Max31865 object with parameters to measure temperature.
    Max31865(std::string name, std::string device, uint32_t clock, double Rref)        : SPIChip(name, device, MAX31865_MODE, MAX31865_BITS, clock, 0) 
   {
      this->name = name;
      this->device = device;
      this->clock = clock;
      this->Rref = Rref;
   };

    virtual ~Max31865();

    /// Get chip name tag.
    std::string GetName() { return name; }

    /// Get chip device file name.
    std::string GetDevice() { return device; }

    /// Get clock frequency used to transfer serial data.
    uint32_t GetClock() { return clock; }

    /// Get high fault value.
    uint16_t GetHighFault();

    /// Get low fault value.
    uint16_t GetLowFault(); 

    /// Test if least significant bit in RTD is set indicating fault.
    bool IsFault();

    /// Get fault status byte.
    uint8_t GetFaultStatusByte(); 

    /// Get reference resistance value used in calculations.
    double   GetRref() { return Rref; }

    /// Set chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set chip device file name.
    void SetDevice(std::string device) { this->device = device; }

    /// Set clock frequency used to transfer serial data.
    void SetClock(uint32_t clock) { this->clock = clock; }

    /// Set reference resistance value used in calculations.
    void SetRref(double Rref) { this->Rref = Rref; }

    /// Set high fault value.
    void SetHighFault(uint16_t highfault);

    /// Set low fault value.
    void SetLowFault(uint16_t lowfault);

    /// Set bias on bit D7=1 in configuration register.
    void BiasOn();

    /// Set bias off bit D7=0 in configuration register.
    void BiasOff(); 

    /// Start one shot measurement by setting bit D5=1 in configuration register.
    void OneShot(); 

    /// Fault detection with automatic delay.
    void FaultDetection();

    /// Fault detection cycle 1.
    void FaultDetectionCycle1();

    /// Fault detection cycle 2.
    void FaultDetectionCycle2();

    /// Use 3-wire mode by setting bit D4=1 in configuration register.
    void ThreeWire(); 

    /// Use 2/4-wire mode by setting bit D4=0 in configuration register.
    void TwoFourWire(); 

    /// Clear fault status by writing one to bit D1.
    void FaultStatusClear(); 

    /// Use 50 Hz filter by writing one to bit D0 in configuration register.
    void Filter50Hz(); 

    /// Use 60 Hz filter by writing zero to bit D0 in configuration register.
    void Filter60Hz(); 

    /// Read A/D-converter value RTD and scale to resistance.
    double GetResistance();

    /// Read A/D-converter value RTD and calculate temperature in Celcius. 
    double GetTemperature();

};

#endif
