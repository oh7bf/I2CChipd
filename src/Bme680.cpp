/**************************************************************************
 * 
 * Bme680 class member functions for configuration and reading with I2C. 
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
 * Thu Jul  9 15:23:16 CDT 2020
 * Edit: Wed Jul 15 09:08:40 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#include "Bme680.hpp"

using namespace std;

Bme680::~Bme680() { };


/// Pressure in Pascal from last measurement. 
double Bme680::GetPressure() 
{ 
  double p = Pressure;
  p /= 1.0; 
  return  p; 
} 

/// Temperature in Celsius from last measurement. 
double Bme680::GetTemperature() 
{ 
  double T = Temperature;
  T /= 100.0;
  return T; 
}

/// Humidity in percent from last measurement. 
double Bme680::GetHumidity() 
{ 
  double H = Humidity;
  H /= 1000.0;
  return H; 
}

/// Resistance in Ohm from last measurement. 
double Bme680::GetResistance() 
{ 
  double R = Resistance;
  R /= 1.0;
  return R; 
}

// Get ID register value.
uint8_t Bme680::GetID()
{
  uint8_t ID = 0;

  I2Chip::I2cWriteUInt8(BME680_ID_REG, address, buffer, error);
  ID = I2Chip::I2cReadUInt8(address, buffer, error);

  return ID;
}

// Get configuration register value.
uint8_t Bme680::GetConfig()
{
  uint8_t config = 0;

  I2Chip::I2cWriteUInt8(BME680_CONFIG_REG, address, buffer, error);
  config = I2Chip::I2cReadUInt8(address, buffer, error);

  return config;
}


// Get control measurement register value.
uint8_t Bme680::GetControlMeasurement()
{
  uint8_t ctrlmeas = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_MEAS_REG, address, buffer, error);
  ctrlmeas = I2Chip::I2cReadUInt8(address, buffer, error);

  return ctrlmeas;
} 

// Get humidity control register value.
uint8_t Bme680::GetHumidityControl()
{
  uint8_t ctrlhum = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_HUM_REG, address, buffer, error);
  ctrlhum = I2Chip::I2cReadUInt8(address, buffer, error);

  return ctrlhum;
}

// Get gas control register 1 value.
uint8_t Bme680::GetGasControl1()
{
  uint8_t gasctrl1 = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS1_REG, address, buffer, error);
  gasctrl1 = I2Chip::I2cReadUInt8(address, buffer, error);

  return gasctrl1;
}

// Get gas control register 0 value.
uint8_t Bme680::GetGasControl0()
{
  uint8_t gasctrl0 = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS0_REG, address, buffer, error);
  gasctrl0 = I2Chip::I2cReadUInt8(address, buffer, error);

  return gasctrl0;
}

// Get gas control register 0 value.
uint8_t Bme680::GetMeasStatus()
{
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BME680_MEAS_STATUS0_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  return status;
}

// Get two mode bits 0 - 3.
uint8_t Bme680::GetMode()
{
  uint8_t ctrl_meas = 0;

  ctrl_meas = Bme680::GetControlMeasurement();
  ctrl_meas &= 0x03;

  return ctrl_meas;
}

// Get gas measurement index 0 - 15.
uint8_t Bme680::GetGasMeasIndex()
{
  uint8_t status = 0;
  status = Bme680::GetMeasStatus();

  status &= 0x0F;

  return status;
}

// Set pointer register.
void Bme680::SetPointer(uint8_t PointerReg)
{
  I2Chip::I2cWriteUInt8(PointerReg, address, buffer, error);
}

// Set control measurement register value.
void Bme680::SetControlMeasurement(uint8_t CtrlMeas)
{
  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_MEAS_REG, CtrlMeas, address, buffer, error);
}

// Set configuration register value.
void Bme680::SetConfig(uint8_t Config)
{
  I2Chip::I2cWriteRegisterUInt8(BME680_CONFIG_REG, Config, address, buffer, error);
}

// Set three humidity, temperature and pressure oversampling bits with 0 - 7.
void Bme680::SetOverSample(uint8_t HOverSample, uint8_t TOverSample, uint8_t POverSample)
{
  uint8_t ctrl_hum = 0, ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_HUM_REG, address, buffer, error);
  ctrl_hum = I2Chip::I2cReadUInt8(address, buffer, error);

  HOverSample &= 0x07;
  ctrl_hum &= 0xF8;
  ctrl_hum |= HOverSample;

  I2Chip::I2cWriteUInt8(BME680_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);

  TOverSample &= 0x07;
  TOverSample = TOverSample << 5;

  POverSample &= 0x07;
  POverSample = POverSample << 2;

  ctrl_meas &= 0x03;
  ctrl_meas |= TOverSample;
  ctrl_meas |= POverSample;

  buffer[ 0 ] = BME680_CTRL_HUM_REG;
  buffer[ 1 ] = ctrl_hum;

  buffer[ 2 ] = BME680_CTRL_MEAS_REG;
  buffer[ 3 ] = ctrl_meas;

  I2Chip::I2cWriteBytes(4, address, buffer, error);
}

// Set two mode bits with 0 - 3.
void Bme680::SetMode(uint8_t Mode)
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Mode &= 0x03;
  ctrl_meas &= 0xFC;
  ctrl_meas |= Mode;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);

}

// Set four heater profile selection bits with 0 - 9.
void Bme680::SetHeaterProfile(uint8_t Profile)
{
  uint8_t ctrl_gas1 = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS1_REG, address, buffer, error);
  ctrl_gas1 = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Profile &= 0x0F;
  ctrl_gas1 &= 0xF0;
  ctrl_gas1 |= Profile;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_GAS1_REG, ctrl_gas1, address, buffer, error);
  
}

// Set gas wait time profile 0 - 9.
void Bme680::SetGasWaitTime(uint8_t Profile, uint8_t time)
{
  uint8_t reg = BME680_GAS_WAIT_REG + Profile;;

  I2Chip::I2cWriteRegisterUInt8(reg, time, address, buffer, error);
}

// Set gas heater resistor profile 0 - 9.
void Bme680::SetGasHeatResistor(uint8_t Profile, uint8_t R)
{
  uint8_t reg = BME680_GAS_RES_HEAT_REG + Profile;;

  I2Chip::I2cWriteRegisterUInt8(reg, R, address, buffer, error);
}

// Set gas heater current profile 0 - 9.
void Bme680::SetGasHeatCurrent(uint8_t Profile, uint8_t I)
{
  uint8_t reg = BME680_GAS_IDAC_HEAT_REG + Profile;;

  I2Chip::I2cWriteRegisterUInt8(reg, I, address, buffer, error);
}

// Set gas heater temperature profile 0 - 9.
void Bme680::SetGasHeatTemperature(uint8_t Profile, int Tamb, int T)
{
  uint8_t reg = BME680_GAS_RES_HEAT_REG + Profile;;
  uint8_t heatres = 0;
  int32_t var1 = 0, var2 = 0, var3 = 0, var4 = 0, var5 = 0;
  int32_t heatresx100 = 0; 

  var1 = (((int32_t)Tamb * par_g3) / 10 ) << 8;
  var2 = (par_g1 + 784) * (((((par_g2 + 154009) * T * 5) / 100) + 3276800) / 10);
  var3 = var1 + (var2 >> 1);
  var4 = ( var3 / (res_heat_range + 4 ) );
  var5 = (131 * res_heat_val) + 65536;
  heatresx100 = (int32_t)((( var4 / var5 ) - 250 ) * 34 );
  heatres = (uint8_t)(( heatresx100 + 50) / 100);

  fprintf(stderr, SD_DEBUG "res_heat_%d = %d\n", Profile, heatres);
  
  I2Chip::I2cWriteRegisterUInt8(reg, heatres, address, buffer, error);
}

// Run gas conversions.
void Bme680::RunGas()
{
  uint8_t ctrl_gas1 = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS1_REG, address, buffer, error);
  ctrl_gas1 = I2Chip::I2cReadUInt8(address, buffer, error);
  
  ctrl_gas1 |= 0x10;;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_GAS1_REG, ctrl_gas1, address, buffer, error);
  
}

// Do not run gas conversions.
void Bme680::NoGas()
{
  uint8_t ctrl_gas1 = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS1_REG, address, buffer, error);
  ctrl_gas1 = I2Chip::I2cReadUInt8(address, buffer, error);
  
  ctrl_gas1 &= 0xEF;;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_GAS1_REG, ctrl_gas1, address, buffer, error);
}

// Set three filter bits with 0 - 7.
void Bme680::SetFilter(uint8_t Filter)
{
  uint8_t config = 0;

  I2Chip::I2cWriteUInt8(BME680_CONFIG_REG, address, buffer, error);
  config = I2Chip::I2cReadUInt8(address, buffer, error);
  
  Filter &= 0x07;
  Filter = Filter << 2;
  config &= 0xE3;
  config |= Filter;

  I2Chip::I2cWriteRegisterUInt8(BME680_CONFIG_REG, config, address, buffer, error);
}

// New data is available.
bool Bme680::IsNewData()
{
  bool newdata = false;
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BME680_MEAS_STATUS0_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  if( (status & 0x80) == 0x80 ) newdata = true; else newdata = false;

  return newdata;
}

// Gas measurement going on.
bool Bme680::IsGasMeasuring()
{
  bool measuring = false;
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BME680_MEAS_STATUS0_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  if( (status & 0x40) == 0x40 ) measuring = true; else measuring = false;

  return measuring;
}

// Measurement going on.
bool Bme680::IsMeasuring()
{
  bool measuring = false;
  uint8_t status = 0;

  I2Chip::I2cWriteUInt8(BME680_MEAS_STATUS0_REG, address, buffer, error);
  status = I2Chip::I2cReadUInt8(address, buffer, error);

  if( (status & 0x20) == 0x20 ) measuring = true; else measuring = false;

  return measuring;
}

// Power on reset.
void Bme680::Reset()
{
  I2Chip::I2cWriteRegisterUInt8(BME680_RESET_REG, 0xB6, address, buffer, error);
}

// Sleep Mode[1:0] = 00.
void Bme680::Sleep()
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);
  
  ctrl_meas &= 0xFC;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Forced Mode[1:0] = 01.
void Bme680::Forced()
{
  uint8_t ctrl_meas = 0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_MEAS_REG, address, buffer, error);
  ctrl_meas = I2Chip::I2cReadUInt8(address, buffer, error);

  ctrl_meas &= 0xFD;
  ctrl_meas |= 0x01;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_MEAS_REG, ctrl_meas, address, buffer, error);
}

// Turn on current injected to heater.
void Bme680::HeaterOn()
{
  uint8_t ctrl_gas0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS0_REG, address, buffer, error);

  ctrl_gas0 = I2Chip::I2cReadUInt8(address, buffer, error);

  ctrl_gas0 &= 0xF7;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_GAS0_REG, ctrl_gas0, address, buffer, error);
}

// Turn off current injected to heater.
void Bme680::HeaterOff()
{
  uint8_t ctrl_gas0;

  I2Chip::I2cWriteUInt8(BME680_CTRL_GAS0_REG, address, buffer, error);

  ctrl_gas0 = I2Chip::I2cReadUInt8(address, buffer, error);
      
  ctrl_gas0 |= 0x08;

  I2Chip::I2cWriteRegisterUInt8(BME680_CTRL_GAS0_REG, ctrl_gas0, address, buffer, error);
}

// Read chip calibration data and return true if success.
bool Bme680::GetCalibration()
{
  I2Chip::I2cWriteUInt8(BME680_PAR_T2_REG, address, buffer, error);

  if( error != 0 )
  {
    return false;
  }
  else
  {
    I2Chip::I2cReadBytes(23, address, buffer, error);

    if( error != 0 )
    {
      return false;
    }
    else
    {
      par_t2 = (int16_t)( (buffer[ 1 ] << 8) | buffer[ 0 ] );
      par_t3 = (int8_t)buffer[ 2 ];
      par_p1 = (uint16_t)( (buffer[ 5 ] << 8) | buffer[ 4 ] );
      par_p2 = (int16_t)( (buffer[ 7 ] << 8) | buffer[ 6 ] );
      par_p3 = (int8_t)buffer[ 8 ];
      par_p4 = (int16_t)( (buffer[ 11 ] << 8) | buffer[ 10 ] );
      par_p5 = (int16_t)( (buffer[ 13 ] << 8) | buffer[ 12 ] );
      par_p6 = (int8_t)buffer[ 15 ];
      par_p7 = (int8_t)buffer[ 14 ];
      par_p8 = (int16_t)( (buffer[ 19 ] << 8) | buffer[ 18 ] );
      par_p9 = (int16_t)( (buffer[ 21 ] << 8) | buffer[ 20 ] );
      par_p10 = (uint8_t)buffer[ 22 ];

      I2Chip::I2cWriteUInt8(BME680_PAR_H1H2_REG, address, buffer, error);

      if( error != 0 )
      {
        return false;
      }
      else
      {
        I2Chip::I2cReadBytes(14, address, buffer, error);

        if( error != 0 )
        {
          return false;
        }
        else
        {
          par_h1 = (uint16_t)( buffer[ 2 ] << 8 | buffer[ 1 ] );
          par_h2 = (uint16_t)( (buffer[ 0 ] << 8) | buffer[ 1 ] );
          par_h3 = (int8_t)buffer[ 3 ];
          par_h4 = (int8_t)buffer[ 4 ];
          par_h5 = (int8_t)buffer[ 5 ];
          par_h6 = (uint8_t)buffer[ 6 ];
          par_h7 = (int8_t)buffer[ 7 ];
          par_t1 = (int16_t)( (buffer[ 9 ] << 8) | buffer[ 8 ] );
          par_g2 = (int16_t)( (buffer[ 11 ] << 8) | buffer[ 10 ] );
          par_g1 = (int8_t)buffer[ 12 ];
	  par_g2 = (int8_t)buffer[ 13 ];

          I2Chip::I2cWriteUInt8(BME680_RANGE_SWITCHING_ERROR_REG, address, buffer, error);
          if( error != 0 )
          {
            return false;
          }
          else
          {
            I2Chip::I2cReadBytes(1, address, buffer, error);

            if( error != 0 )
	    {
              return false;
            }
            else
            {
              range_sw_error = ( (int8_t)buffer[ 0 ] ) / 16;

              I2Chip::I2cWriteUInt8(BME680_RES_HEAT_RANGE_REG, address, buffer, error);
              if( error != 0 )
              {
                return false;
              }
              else
              {
                I2Chip::I2cReadBytes(1, address, buffer, error);

                if( error != 0 )
                {
                  return false;
                }
                else
                {
                  res_heat_range = (uint8_t)(buffer[ 0 ] >> 4);

                  I2Chip::I2cWriteUInt8(BME680_RES_HEAT_VAL_REG, address, buffer, error);

                  if( error != 0 )
                  {
                    return false;
                  }
                  else
                  {
                    I2Chip::I2cReadBytes(1, address, buffer, error);

                    if( error != 0 )
                    {
                      return false;
                    }
                    else
                    {
                      res_heat_val  = (int8_t)buffer[ 0 ];
                    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }

  fprintf(stderr, SD_DEBUG "par_t1 = %d\n", par_t1);
  fprintf(stderr, SD_DEBUG "par_t2 = %d\n", par_t2);
  fprintf(stderr, SD_DEBUG "par_t3 = %d\n", par_t3);
  fprintf(stderr, SD_DEBUG "par_p1 = %d\n", par_p1);
  fprintf(stderr, SD_DEBUG "par_p2 = %d\n", par_p2);
  fprintf(stderr, SD_DEBUG "par_p3 = %d\n", par_p3);
  fprintf(stderr, SD_DEBUG "par_p4 = %d\n", par_p4);
  fprintf(stderr, SD_DEBUG "par_p5 = %d\n", par_p5);
  fprintf(stderr, SD_DEBUG "par_p6 = %d\n", par_p6);
  fprintf(stderr, SD_DEBUG "par_p7 = %d\n", par_p7);
  fprintf(stderr, SD_DEBUG "par_p8 = %d\n", par_p8);
  fprintf(stderr, SD_DEBUG "par_p9 = %d\n", par_p9);
  fprintf(stderr, SD_DEBUG "par_p10 = %d\n", par_p10);
  fprintf(stderr, SD_DEBUG "par_h1 = %d\n", par_h1);
  fprintf(stderr, SD_DEBUG "par_h2 = %d\n", par_h2);
  fprintf(stderr, SD_DEBUG "par_h3 = %d\n", par_h3);
  fprintf(stderr, SD_DEBUG "par_h4 = %d\n", par_h4);
  fprintf(stderr, SD_DEBUG "par_h5 = %d\n", par_h5);
  fprintf(stderr, SD_DEBUG "par_h6 = %d\n", par_h6);
  fprintf(stderr, SD_DEBUG "par_h7 = %d\n", par_h7);
  fprintf(stderr, SD_DEBUG "range_sw_error = %d\n", range_sw_error);
  fprintf(stderr, SD_DEBUG "res_heat_range = %d\n", res_heat_range);
  fprintf(stderr, SD_DEBUG "res_heat_val = %d\n", res_heat_val);

  return true;
}

// Read chip temperature, pressure, humidity and resistance registers and do conversion.
int Bme680::GetTPHG()
{
//  uint32_t tadc = 0;
  int64_t tvar1 = 0, tvar2 = 0, tvar3 = 0;
  int32_t p =0, pvar1 = 0, pvar2 = 0, pvar3 = 0;
//  uint32_t padc = 0;
//  uint16_t hadc = 0;
  int32_t hvar1 = 0, hvar2 = 0, hvar3 = 0, hvar4 = 0, hvar5 = 0, hvar6 = 0;
  int32_t tscaled = 0, h = 0;
//  uint16_t gadc = 0;
  uint8_t grange = 0;
  int8_t range_sw_error = 0;
  int64_t gvar1 = 0;
  uint64_t gvar2 = 0;

  int32_t tfine;

  // gas range constants for resistance calculation
  const uint32_t a1[16] = {2147483647, 2147483647, 2147483647, 2147483647, 2147483647, 2126008810, 2147483647, 2130303777, 2147483647, 2147483647, 2143188679, 2136746228, 2147483647, 2126008810, 2147483647, 2147483647};

  const uint32_t a2[16] = {4096000000, 2048000000, 1024000000, 512000000, 255744255, 127110228, 64000000, 32258064, 16016016, 8000000, 4000000, 2000000, 1000000, 500000, 250000, 125000};

  I2Chip::I2cWriteUInt8(BME680_PRESS_MSB_REG, address, buffer, error);
  if( error != 0 )
  {
    return error;
  }
  else
  {
    I2Chip::I2cReadBytes(8, address, buffer, error);
    if( error != 0 )
    {
      return error;
    }
    else
    {
      tadc =  (uint32_t)( buffer[ 3 ] << 12 );
      tadc |= (uint32_t)( buffer[ 4 ] << 4 ); 
      tadc |= (uint32_t)( buffer[ 5 ] >> 4 ); 

      fprintf(stderr, SD_DEBUG "tadc = %d\n", tadc);

      tvar1 = ( (int32_t)tadc >> 3 ) - ( (int32_t)par_t1 << 1);
      tvar2 = (tvar1 * (int32_t)par_t2 ) >> 11;
      tvar3 = ((((tvar1 >> 1) * (tvar1 >> 1)) >> 12) * ((int32_t)par_t3 << 4)) >> 14;

      tfine = (int32_t) (tvar2 + tvar3);
      Temperature = (int16_t)( (tfine * 5 + 128) >> 8); 

      padc =  (uint32_t)( buffer[ 0 ] << 12 );
      padc |= (uint32_t)( buffer[ 1 ] << 4 ); 
      padc |= (uint32_t)( buffer[ 2 ] >> 4 ); 

      fprintf(stderr, SD_DEBUG "padc = %d\n", padc);

      pvar1 = ((int32_t)tfine >> 1) - 64000;
      pvar2 = ((((pvar1 >> 2) * (pvar1 >> 2)) >> 11) * (int32_t)par_p6) >> 2;
      pvar2 = pvar2 + ( (pvar1 * (int32_t)par_p5) << 1 );
      pvar2 = (pvar2 >> 2) + ( (int32_t)par_p4 << 16 );
      pvar1 = (((((pvar1 >> 2) * (pvar1 >> 2)) >> 13) * ((int32_t)par_p3 << 5)) >> 3 ) + ( ( pvar1 * (int32_t)par_p2) >> 1);
      pvar1 = pvar1 >> 18;
      pvar1 = ((32768 + pvar1) * (int32_t)par_p1) >> 15;
      p = 1048576 - padc;
      p = (int32_t)((p - (pvar2 >> 12)) * ((uint32_t)3125));
      if( p >= ( 1 << 30 ) ) 
        p = ( (p / pvar1 ) << 1);
      else
        p = ( (p << 1)/ pvar1 );
      
      pvar1 = ((int32_t)par_p9 * (int32_t)(((p >> 3) * (p >> 3)) >> 13 )) >> 12;
      pvar2 = ((int32_t)(p >> 2) * (int32_t)par_p8) >> 13;
      pvar3 = ((int32_t)(p >> 8) * (int32_t)(p >> 8) * (int32_t)(p >> 8) * (int32_t)par_p10) >> 17;
      p = (int32_t)p + ((pvar1 + pvar2 + pvar3 + ((int32_t)par_p7 << 7 )) >> 4);
      Pressure = (uint32_t)p;

      hadc =  (uint16_t)( buffer[ 6 ] << 8 );
      hadc |= (uint16_t)( buffer[ 7 ] ); 

      fprintf(stderr, SD_DEBUG "hadc = %d\n", hadc);

      tscaled = (int32_t)Temperature;
      hvar1 = (int32_t)hadc - (int32_t)((int32_t)par_h1 << 4 ) - (((tscaled * (int32_t)par_h3) / ((int32_t)100)) >> 1);
      hvar2 = ((int32_t)par_h2 * (((tscaled * (int32_t)par_h4) / ((int32_t)100)) + (((tscaled * ((tscaled * (int32_t)par_h5)/((int32_t)100))) >> 6) / ((int32_t)100)) + ((int32_t)(1 << 14)))) >> 10;
      hvar3 = hvar1 * hvar2;
      hvar4 = (((int32_t)par_h6 << 7) + ((tscaled * (int32_t)par_h7) / ((int32_t)100))) >> 4;
      hvar5 = ((hvar3 >> 14) * (hvar3 >> 14)) >> 10;
      hvar6 = (hvar4 * hvar5) >> 1;
      h = (hvar3 + hvar6) >> 12;
      h = (((hvar3 + hvar6) >> 10) * ((int32_t) 1000)) >> 12;
      Humidity = (uint32_t)h;

      I2Chip::I2cWriteUInt8(BME680_GAS_R_MSB_REG, address, buffer, error);
      if( error != 0 )
      {
        return error;
      }
      else
      {
        I2Chip::I2cReadBytes(2, address, buffer, error);
        if( error != 0 )
        {
          return error;
        }
        else
        {
          gadc =  (uint16_t)( buffer[ 0 ] << 2 );
          gadc |= (uint16_t)( buffer[ 1 ] >> 6 ); 
          grange = (uint8_t)( buffer[ 1 ] & 0x0F );

          fprintf(stderr, SD_DEBUG "gadc = %d, grange = %d\n", gadc, grange);

          if( (buffer[ 9 ] & 0x20 ) == 0x20 ) gas_valid = true; 
          else gas_valid = false;

          if( gas_valid ) fprintf(stderr, SD_DEBUG "Gas conversion valid\n");
          else fprintf(stderr, SD_DEBUG "Gas conversion not valid\n");

          if( (buffer[ 9 ] & 0x10 ) == 0x10 ) heat_stab = true; 
          else heat_stab = false;

          if( heat_stab ) fprintf(stderr, SD_DEBUG "Heater stable\n");
          else fprintf(stderr, SD_DEBUG "Heater not stable\n");

          gvar1 = (int64_t)(((1340 + ( 5 * (int64_t)range_sw_error ) ) * ((int64_t)a1[ grange ] ) ) >> 16 );
          gvar2 = (int64_t)( gadc << 15 ) - (int64_t)( 1 << 24) + gvar1;

          Resistance = (uint32_t)((((int64_t)(a2[ grange ] * (int64_t)gvar1) >> 9 ) + (gvar2 >> 1) ) / gvar2);

    	}
      }
    }
  }
  return 0;
}

