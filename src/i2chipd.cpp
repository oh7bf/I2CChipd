/**************************************************************************
 * 
 * Read chips with I2C interface. 
 *       
 * Copyright (C) 2020 - 2023 Jaakko Koivuniemi.
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
 * Fri Jul  3 20:16:26 CDT 2020
 * Edit: Fri  3 Nov 17:22:47 CDT 2023
 *
 * Jaakko Koivuniemi
 **/

#include "i2chipd.hpp"
#include <systemd/sd-daemon.h>
#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

#ifdef USE_DIM_LIBS
#include <dis.hxx>
#endif

using namespace std;

bool cont = true;

void shutdown(int sig)
{
  fprintf(stderr, SD_WARNING "SIGTERM received, shut down\n");
  cont = false;
}

void reload(int sig)
{
  fprintf(stderr, SD_WARNING "SIGHUP received (to implement: reload configuration)\n");
}


/// i2chipd program to read I2C chips at regular intervals 

/// Run the program with `i2chipd 2> /dev/null` on shell. Printing
/// to standard error stream is for _systemd_ logging with _systemd-journald_
/// and includes different log levels defined in `sd-daemon.h`.
int main()
{
  const int version = 20231103; // program version
  
  string i2cdev = "/dev/i2c-1";

  string spidev00 = "/dev/spidev0.0";
  string spidev01 = "/dev/spidev0.1";
  string spidev02 = "/dev/spidev0.2";
  string spidev03 = "/dev/spidev0.3";
  string spidev04 = "/dev/spidev0.4";
  string spidev05 = "/dev/spidev0.5";
  string spidev06 = "/dev/spidev0.6";
  string spidev07 = "/dev/spidev0.7";

  string datadir = "/tmp/";
  string sqlitedb = "/var/lib/i2chipd/i2chipd.db";
  string dimserver = "";
  string dimdns = "localhost";
  int readinterval = 120;
  int sqlite_err = 0;

  signal(SIGTERM, &shutdown);
  signal(SIGHUP, &reload);

  fprintf(stderr, SD_INFO "i2chipd v. %d\n", version );

  // configuration settings for chips
  const uint8_t bmp280_TOverSample[ 2 ] = {1, 1};
  const uint8_t bmp280_POverSample[ 2 ] = {1, 1};

  const uint8_t bme680_HOverSample[ 2 ] = {1, 1};
  const uint8_t bme680_TOverSample[ 2 ] = {2, 2};
  const uint8_t bme680_POverSample[ 2 ] = {5, 5};
  const uint8_t bme680_Filter[ 2 ] = {1, 1};
  // profile 0 target temperature for BME680
  const uint16_t bme680_T0[ 2 ] = {300, 300};
  int8_t Tamb = 25; // ambient temperature for BME680 [C]
  const uint8_t bme680_GasWaitTime[ 2 ] = {0x59, 0x59}; // 0x59 = 100 ms

  bool tmp102x48 = false, tmp102x49 = false, tmp102x4A = false, tmp102x4B = false;
  bool htu21dx = false;
  bool bmp280x76 = false, bmp280x77 = false;
  bool bme680x76 = false, bme680x77 = false;
  bool bh1750fvix23 = false, bh1750fvix5C = false;
  bool lis2mdlx1E = false;
  bool lis3mdlx1C = false, lis3mdlx1E = false;
  bool lis3dhx18 = false, lis3dhx19 = false;
  bool max31865_00 = false, max31865_01 = false;
  bool max31865_02 = false, max31865_03 = false;
  bool max31865_04 = false, max31865_05 = false;
  bool max31865_06 = false, max31865_07 = false;
  bool pca9535x20 = false, pca9535x21 = false;
  bool pca9535x22 = false, pca9535x23 = false;
  bool pca9535x24 = false, pca9535x25 = false;
  bool pca9535x26 = false, pca9535x27 = false;

  std::size_t pos;
  std::string line ("");
  ifstream confile;
  confile.open("/etc/i2chipd_conf");
  if( !confile.is_open() )
  {
    fprintf(stderr, SD_ERR "failed to open /etc/i2chipd_conf, quit now\n");
    return -1;
  }
  else
  {
    while( getline( confile, line) )
    {
      if( !line.empty() )
      {
        if( line.at( 0 ) != '#' )
        {
          if( line.find("TMP102_x48") != std::string::npos ) tmp102x48 = true;
          if( line.find("TMP102_x49") != std::string::npos ) tmp102x49 = true;
          if( line.find("TMP102_x4A") != std::string::npos ) tmp102x4A = true;
          if( line.find("TMP102_x4B") != std::string::npos ) tmp102x4B = true;
          if( line.find("HTU21D") != std::string::npos ) htu21dx = true;
          if( line.find("BMP280_x76") != std::string::npos ) bmp280x76 = true;
          if( line.find("BMP280_x77") != std::string::npos ) bmp280x77 = true;
          if( line.find("BME680_x76") != std::string::npos ) bme680x76 = true;
          if( line.find("BME680_x77") != std::string::npos ) bme680x77 = true;
          if( line.find("BH1750FVI_x23") != std::string::npos ) bh1750fvix23 = true;
          if( line.find("BH1750FVI_x5C") != std::string::npos ) bh1750fvix5C = true;
          if( line.find("LIS3DH_x18") != std::string::npos ) lis3dhx18 = true;
          if( line.find("LIS3DH_x19") != std::string::npos ) lis3dhx19 = true;
          if( line.find("LIS2MDL_x1E") != std::string::npos ) lis2mdlx1E = true;
          if( line.find("LIS3MDL_x1C") != std::string::npos ) lis3mdlx1C = true;
          if( line.find("LIS3MDL_x1E") != std::string::npos ) lis3mdlx1E = true;
          if( line.find("MAX31865_00") != std::string::npos ) max31865_00 = true;
          if( line.find("MAX31865_01") != std::string::npos ) max31865_01 = true;
          if( line.find("MAX31865_02") != std::string::npos ) max31865_02 = true;
          if( line.find("MAX31865_03") != std::string::npos ) max31865_03 = true;
          if( line.find("MAX31865_04") != std::string::npos ) max31865_04 = true;
          if( line.find("MAX31865_05") != std::string::npos ) max31865_05 = true;
          if( line.find("MAX31865_06") != std::string::npos ) max31865_06 = true;
          if( line.find("MAX31865_07") != std::string::npos ) max31865_07 = true;
          if( line.find("PCA9535_x20") != std::string::npos ) pca9535x20 = true;
          if( line.find("PCA9535_x21") != std::string::npos ) pca9535x21 = true;
          if( line.find("PCA9535_x22") != std::string::npos ) pca9535x22 = true;
          if( line.find("PCA9535_x23") != std::string::npos ) pca9535x23 = true;
          if( line.find("PCA9535_x24") != std::string::npos ) pca9535x24 = true;
          if( line.find("PCA9535_x25") != std::string::npos ) pca9535x25 = true;
          if( line.find("PCA9535_x26") != std::string::npos ) pca9535x26 = true;
          if( line.find("PCA9535_x27") != std::string::npos ) pca9535x27 = true;

          pos = line.find("READINT");
          if( pos != std::string::npos ) 
          {
            readinterval = atoi( line.substr(pos+7, line.length() - pos - 7 ).c_str() );
            fprintf(stderr, SD_INFO "read interval %d s\n", readinterval );
          }

#ifdef USE_DIM_LIBS
	  pos = line.find("DIMSERVER");
          if( pos != std::string::npos ) 
          {
            dimserver = line.substr(pos+10, line.length() - pos - 10 ).c_str();
            fprintf(stderr, SD_INFO "DIM server %s\n", dimserver.c_str() );
          }

          pos = line.find("DIMDNS");
          if( pos != std::string::npos ) 
          {
            dimdns = line.substr(pos+7, line.length() - pos - 7 ).c_str();
            fprintf(stderr, SD_INFO "DIM name server %s\n", dimdns.c_str() );
          }
#endif

	}
      }
    } 
    confile.close();
  }

    // chip objects to read, use nullptr if not wired
  Tmp102 *tmp102[ 4 ];

  if( tmp102x48 ) tmp102[ 0 ] = new Tmp102("T1", i2cdev); else tmp102[ 0 ] = nullptr;
  if( tmp102x49 ) tmp102[ 1 ] = new Tmp102("T2", i2cdev); else tmp102[ 1 ] = nullptr;
  if( tmp102x4A ) tmp102[ 2 ] = new Tmp102("T3", i2cdev); else tmp102[ 2 ] = nullptr;
  if( tmp102x4B ) tmp102[ 3 ] = new Tmp102("T4", i2cdev); else tmp102[ 3 ] = nullptr;

  Htu21d *htu21d;
  if( htu21dx ) htu21d = new Htu21d("TH1", i2cdev); else htu21d = nullptr;

  Bmp280 *bmp280[ 2 ];
  if( bmp280x76 ) bmp280[ 0 ] = new Bmp280("Tp1", i2cdev); else bmp280[ 0 ] = nullptr;
  if( bmp280x77 ) bmp280[ 1 ] = new Bmp280("Tp2", i2cdev); else bmp280[ 1 ] = nullptr;

  Bme680 *bme680[ 2 ];
  if( bme680x76 ) bme680[ 0 ] = new Bme680("TpHG1", i2cdev); else bme680[ 0 ] = nullptr;
  if( bme680x77 ) bme680[ 1 ] = new Bme680("TpHG2", i2cdev); else bme680[ 1 ] = nullptr;

  Bh1750fvi *bh1750fvi[ 2 ];
  if( bh1750fvix23 ) bh1750fvi[ 0 ] = new Bh1750fvi("Ev1", i2cdev); else bh1750fvi[ 0 ] = nullptr;
  if( bh1750fvix5C ) bh1750fvi[ 1 ] = new Bh1750fvi("Ev2", i2cdev); else bh1750fvi[ 1 ] = nullptr;

  Lis2mdl *lis2mdl;
  if( lis2mdlx1E ) lis2mdl = new Lis2mdl("B0", i2cdev); else lis2mdl = nullptr;

  Lis3mdl *lis3mdl[ 2 ];
  if( lis3mdlx1C ) lis3mdl[ 0 ] = new Lis3mdl("B1", i2cdev); else lis3mdl[ 0 ] = nullptr;
  if( lis3mdlx1E ) lis3mdl[ 1 ] = new Lis3mdl("B2", i2cdev); else lis3mdl[ 1 ] = nullptr;

  Lis3dh *lis3dh[ 2 ];
  if( lis3dhx18 ) lis3dh[ 0 ] = new Lis3dh("g1", i2cdev); else lis3dh[ 0 ] = nullptr;
  if( lis3dhx19 ) lis3dh[ 1 ] = new Lis3dh("g2", i2cdev); else lis3dh[ 1 ] = nullptr;

  Max31865 *max31865[ 8 ];
  if( max31865_00 ) max31865[ 0 ] = new Max31865("TDR1", spidev00, 500000, 430);
  else max31865[ 0 ] = nullptr;
  if( max31865_01 ) max31865[ 1 ] = new Max31865("TDR2", spidev01, 500000, 430);
  else max31865[ 1 ] = nullptr;
  if( max31865_02 ) max31865[ 2 ] = new Max31865("TDR3", spidev02, 500000, 430);
  else max31865[ 2 ] = nullptr;
  if( max31865_03 ) max31865[ 3 ] = new Max31865("TDR4", spidev03, 500000, 430);
  else max31865[ 3 ] = nullptr;
  if( max31865_04 ) max31865[ 4 ] = new Max31865("TDR5", spidev04, 500000, 430);
  else max31865[ 4 ] = nullptr;
  if( max31865_05 ) max31865[ 5 ] = new Max31865("TDR6", spidev05, 500000, 430);
  else max31865[ 5 ] = nullptr;
  if( max31865_06 ) max31865[ 6 ] = new Max31865("TDR7", spidev06, 500000, 430);
  else max31865[ 6 ] = nullptr;
  if( max31865_07 ) max31865[ 7 ] = new Max31865("TDR8", spidev07, 500000, 430);
  else max31865[ 7 ] = nullptr;

  Pca9535 *pca9535[ 8 ];

  if( pca9535x20 ) pca9535[ 0 ] = new Pca9535("IO1", i2cdev); else pca9535[ 0 ] = nullptr;
  if( pca9535x21 ) pca9535[ 1 ] = new Pca9535("IO2", i2cdev); else pca9535[ 1 ] = nullptr;
  if( pca9535x22 ) pca9535[ 2 ] = new Pca9535("IO3", i2cdev); else pca9535[ 2 ] = nullptr;
  if( pca9535x23 ) pca9535[ 3 ] = new Pca9535("IO4", i2cdev); else pca9535[ 3 ] = nullptr;
  if( pca9535x24 ) pca9535[ 4 ] = new Pca9535("IO5", i2cdev); else pca9535[ 4 ] = nullptr;
  if( pca9535x25 ) pca9535[ 5 ] = new Pca9535("IO6", i2cdev); else pca9535[ 5 ] = nullptr;
  if( pca9535x26 ) pca9535[ 6 ] = new Pca9535("IO7", i2cdev); else pca9535[ 6 ] = nullptr;
  if( pca9535x27 ) pca9535[ 7 ] = new Pca9535("IO8", i2cdev); else pca9535[ 7 ] = nullptr;

  // data files to write most recent value
  File *tmp102_file[ 4 ];
  tmp102_file[ 0 ] = new File(datadir, "tmp102_x48");
  tmp102_file[ 1 ] = new File(datadir, "tmp102_x49");
  tmp102_file[ 2 ] = new File(datadir, "tmp102_x4A");
  tmp102_file[ 3 ] = new File(datadir, "tmp102_x4B");

  File *htu21d_T_file = new File(datadir, "htu21d_T");
  File *htu21d_RH_file = new File(datadir, "htu21d_RH");

  File *bmp280_T_file[ 2 ], *bmp280_p_file[ 2 ];
  bmp280_T_file[ 0 ] = new File(datadir, "bmp280_x76_T");
  bmp280_T_file[ 1 ] = new File(datadir, "bmp280_x77_T");
  bmp280_p_file[ 0 ] = new File(datadir, "bmp280_x76_p");
  bmp280_p_file[ 1 ] = new File(datadir, "bmp280_x77_p");

  File *bme680_T_file[ 2 ], *bme680_TF_file[ 2 ], *bme680_p_file[ 2 ], *bme680_RH_file[ 2 ], *bme680_R_file[ 2 ];
  bme680_T_file[ 0 ]  = new File(datadir, "bme680_x76_T");
  bme680_T_file[ 1 ]  = new File(datadir, "bme680_x77_T");
  bme680_TF_file[ 0 ]  = new File(datadir, "bme680_x76_TF");
  bme680_TF_file[ 1 ]  = new File(datadir, "bme680_x77_TF");
  bme680_p_file[ 0 ]  = new File(datadir, "bme680_x76_p");
  bme680_p_file[ 1 ]  = new File(datadir, "bme680_x77_p");
  bme680_RH_file[ 0 ] = new File(datadir, "bme680_x76_RH");
  bme680_RH_file[ 1 ] = new File(datadir, "bme680_x77_RH");
  bme680_R_file[ 0 ]  = new File(datadir, "bme680_x76_R");
  bme680_R_file[ 1 ]  = new File(datadir, "bme680_x77_R");

  File *bh1750fvi_Ev_file[ 2 ];
  bh1750fvi_Ev_file[ 0 ] = new File(datadir, "bh1750fvi_x23_Ev");
  bh1750fvi_Ev_file[ 1 ] = new File(datadir, "bh1750fvi_x5C_Ev");

  File *lis3dh_gx_file[ 2 ], *lis3dh_gy_file[ 2 ], *lis3dh_gz_file[ 2 ], *lis3dh_adc1_file[ 2 ], *lis3dh_adc2_file[ 2 ], *lis3dh_adc3_file[ 2 ];
  lis3dh_gx_file[ 0 ] = new File(datadir, "lis3dh_x18_gx");
  lis3dh_gx_file[ 1 ] = new File(datadir, "lis3dh_x19_gx");
  lis3dh_gy_file[ 0 ] = new File(datadir, "lis3dh_x18_gy");
  lis3dh_gy_file[ 1 ] = new File(datadir, "lis3dh_x19_gy");
  lis3dh_gz_file[ 0 ] = new File(datadir, "lis3dh_x18_gz");
  lis3dh_gz_file[ 1 ] = new File(datadir, "lis3dh_x19_gz");
  lis3dh_adc1_file[ 0 ] = new File(datadir, "lis3dh_x18_adc1");
  lis3dh_adc1_file[ 1 ] = new File(datadir, "lis3dh_x19_adc1");
  lis3dh_adc2_file[ 0 ] = new File(datadir, "lis3dh_x18_adc2");
  lis3dh_adc2_file[ 1 ] = new File(datadir, "lis3dh_x19_adc2");
  lis3dh_adc3_file[ 0 ] = new File(datadir, "lis3dh_x18_adc3");
  lis3dh_adc3_file[ 1 ] = new File(datadir, "lis3dh_x19_adc3");

  File *lis2mdl_Bx_file, *lis2mdl_By_file, *lis2mdl_Bz_file, *lis2mdl_T_file;
  lis2mdl_Bx_file = new File(datadir, "lis2mdl_x1E_Bx");
  lis2mdl_By_file = new File(datadir, "lis2mdl_x1E_By");
  lis2mdl_Bz_file = new File(datadir, "lis2mdl_x1E_Bz");
  lis2mdl_T_file = new File(datadir, "lis2mdl_x1E_T");

  File *lis3mdl_Bx_file[ 2 ], *lis3mdl_By_file[ 2 ], *lis3mdl_Bz_file[ 2 ], *lis3mdl_T_file[ 2 ];
  lis3mdl_Bx_file[ 0 ] = new File(datadir, "lis3mdl_x1C_Bx");
  lis3mdl_Bx_file[ 1 ] = new File(datadir, "lis3mdl_x1E_Bx");
  lis3mdl_By_file[ 0 ] = new File(datadir, "lis3mdl_x1C_By");
  lis3mdl_By_file[ 1 ] = new File(datadir, "lis3mdl_x1E_By");
  lis3mdl_Bz_file[ 0 ] = new File(datadir, "lis3mdl_x1C_Bz");
  lis3mdl_Bz_file[ 1 ] = new File(datadir, "lis3mdl_x1E_Bz");
  lis3mdl_T_file[ 0 ] = new File(datadir, "lis3mdl_x1C_T");
  lis3mdl_T_file[ 1 ] = new File(datadir, "lis3mdl_x1E_T");

  File *max31865_T_file[ 8 ], *max31865_R_file[ 8 ], *max31865_F_file[ 8 ];
  max31865_T_file[ 0 ] = new File(datadir, "max31865_00_T");
  max31865_T_file[ 1 ] = new File(datadir, "max31865_01_T");
  max31865_R_file[ 0 ] = new File(datadir, "max31865_00_R");
  max31865_R_file[ 1 ] = new File(datadir, "max31865_01_R");
  max31865_F_file[ 0 ] = new File(datadir, "max31865_00_F");
  max31865_F_file[ 1 ] = new File(datadir, "max31865_01_F");

  max31865_T_file[ 2 ] = new File(datadir, "max31865_02_T");
  max31865_T_file[ 3 ] = new File(datadir, "max31865_03_T");
  max31865_R_file[ 2 ] = new File(datadir, "max31865_02_R");
  max31865_R_file[ 3 ] = new File(datadir, "max31865_03_R");
  max31865_F_file[ 2 ] = new File(datadir, "max31865_02_F");
  max31865_F_file[ 3 ] = new File(datadir, "max31865_03_F");

  max31865_T_file[ 4 ] = new File(datadir, "max31865_04_T");
  max31865_T_file[ 5 ] = new File(datadir, "max31865_05_T");
  max31865_R_file[ 4 ] = new File(datadir, "max31865_04_R");
  max31865_R_file[ 5 ] = new File(datadir, "max31865_05_R");
  max31865_F_file[ 4 ] = new File(datadir, "max31865_04_F");
  max31865_F_file[ 5 ] = new File(datadir, "max31865_05_F");

  max31865_T_file[ 6 ] = new File(datadir, "max31865_06_T");
  max31865_T_file[ 7 ] = new File(datadir, "max31865_07_T");
  max31865_R_file[ 6 ] = new File(datadir, "max31865_06_R");
  max31865_R_file[ 7 ] = new File(datadir, "max31865_07_R");
  max31865_F_file[ 6 ] = new File(datadir, "max31865_06_F");
  max31865_F_file[ 7 ] = new File(datadir, "max31865_07_F");

  File *pca9535_inputs_file[ 8 ], *pca9535_outputs_file[ 8 ], *pca9535_inversions_file[ 8 ], *pca9535_port_configs_file[ 8 ];
  pca9535_inputs_file[ 0 ] = new File(datadir, "pca9535x20_inputs");
  pca9535_outputs_file[ 0 ] = new File(datadir, "pca9535x20_outputs");
  pca9535_inversions_file[ 0 ] = new File(datadir, "pca9535x20_inversions");
  pca9535_port_configs_file[ 0 ] = new File(datadir, "pca9535x20_port_configs");

  pca9535_inputs_file[ 1 ] = new File(datadir, "pca9535x21_inputs");
  pca9535_outputs_file[ 1 ] = new File(datadir, "pca9535x21_outputs");
  pca9535_inversions_file[ 1 ] = new File(datadir, "pca9535x21_inversions");
  pca9535_port_configs_file[ 1 ] = new File(datadir, "pca9535x21_port_configs");

  pca9535_inputs_file[ 2 ] = new File(datadir, "pca9535x22_inputs");
  pca9535_outputs_file[ 2 ] = new File(datadir, "pca9535x22_outputs");
  pca9535_inversions_file[ 2 ] = new File(datadir, "pca9535x22_inversions");
  pca9535_port_configs_file[ 2 ] = new File(datadir, "pca9535x22_port_configs");

  pca9535_inputs_file[ 3 ] = new File(datadir, "pca9535x23_inputs");
  pca9535_outputs_file[ 3 ] = new File(datadir, "pca9535x23_outputs");
  pca9535_inversions_file[ 3 ] = new File(datadir, "pca9535x23_inversions");
  pca9535_port_configs_file[ 3 ] = new File(datadir, "pca9535x23_port_configs");

  pca9535_inputs_file[ 4 ] = new File(datadir, "pca9535x24_inputs");
  pca9535_outputs_file[ 4 ] = new File(datadir, "pca9535x24_outputs");
  pca9535_inversions_file[ 4 ] = new File(datadir, "pca9535x24_inversions");
  pca9535_port_configs_file[ 4 ] = new File(datadir, "pca9535x24_port_configs");

  pca9535_inputs_file[ 5 ] = new File(datadir, "pca9535x25_inputs");
  pca9535_outputs_file[ 5 ] = new File(datadir, "pca9535x25_outputs");
  pca9535_inversions_file[ 5 ] = new File(datadir, "pca9535x25_inversions");
  pca9535_port_configs_file[ 5 ] = new File(datadir, "pca9535x25_port_configs");

  pca9535_inputs_file[ 6 ] = new File(datadir, "pca9535x26_inputs");
  pca9535_outputs_file[ 6 ] = new File(datadir, "pca9535x26_outputs");
  pca9535_inversions_file[ 6 ] = new File(datadir, "pca9535x26_inversions");
  pca9535_port_configs_file[ 6 ] = new File(datadir, "pca9535x26_port_configs");

  pca9535_inputs_file[ 7 ] = new File(datadir, "pca9535x27_inputs");
  pca9535_outputs_file[ 7 ] = new File(datadir, "pca9535x27_outputs");
  pca9535_inversions_file[ 7 ] = new File(datadir, "pca9535x27_inversions");
  pca9535_port_configs_file[ 7 ] = new File(datadir, "pca9535x27_port_configs");

  // SQLite objects to store values in database table
  SQLite *tmp102_db  = new SQLite(sqlitedb, "tmp102", "insert into tmp102 (name,temperature) values (?,?)");
  SQLite *htu21d_db = new SQLite(sqlitedb, "htu21d", "insert into htu21d (name,temperature,humidity) values (?,?,?)");
  SQLite *bmp280_db  = new SQLite(sqlitedb, "bmp280", "insert into bmp280 (name,temperature,pressure) values (?,?,?)");
  SQLite *bme680_db  = new SQLite(sqlitedb, "bme680", "insert into bme680 (name,temperature,humidity,pressure,resistance,gasvalid,stable) values (?,?,?,?,?,?,?)");
  SQLite *bh1750fvi_db  = new SQLite(sqlitedb, "bh1750fvi", "insert into bh1750fvi (name,illuminance) values (?,?)");
  SQLite *lis3dh_db  = new SQLite(sqlitedb, "lis3dh", "insert into lis3dh(name,gxmin,gx,gxmax,gymin,gy,gymax,gzmin,gz,gzmax,adc1,adc2,adc3,odr) values (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
  SQLite *lis2mdl_db  = new SQLite(sqlitedb, "lis2mdl", "insert into lis2mdl(name,Bx,By,Bz,temperature) values (?,?,?,?,?)");
  SQLite *lis3mdl_db  = new SQLite(sqlitedb, "lis3mdl", "insert into lis3mdl(name,Bx,By,Bz,temperature) values (?,?,?,?,?)");
  SQLite *max31865_db  = new SQLite(sqlitedb, "max31865", "insert into max31865 (name,temperature,resistance,fault) values (?,?,?,?)");

  SQLite *pca9535_db = new SQLite(sqlitedb, "pca9535", "insert into pca9535 (name,inputs,outputs,inversions,portconfigs) values (?,?,?,?,?)");

// DIM services
#ifdef USE_DIM_LIBS
  struct bme680d
  {
    double T = 0;
    double RH = 0;
    double p = 0;
    double R = 0;
  };

  bme680d bme680x76data, bme680x77data;
  DimService *bme680x76Dim, *bme680x77Dim;

  struct bmp280d
  {
    double T = 0;
    double p = 0;
  };

  bmp280d bmp280x76data, bmp280x77data;
  DimService *bmp280x76Dim, *bmp280x77Dim;

  struct bh1750fvid
  {
    double Ev = 0;
  };

  bh1750fvid bh1750fvix23data, bh1750fvix5Cdata;
  DimService *bh1750fvix23Dim, *bh1750fvix5CDim;

  struct htu21dd
  {
    double T = 0;
    double RH = 0;
  };

  htu21dd htu21data;
  DimService *htu21Dim;

  struct lis2mdld
  {
    double Bx = 0;
    double By = 0;
    double Bz = 0;
    double T = 0;
  };

  lis2mdld lis2mdlx1Edata;
  DimService *lis2mdlx1EDim;

  struct lis3mdld
  {
    double Bx = 0;
    double By = 0;
    double Bz = 0;
    double T = 0;
  };

  lis3mdld lis3mdlx1Cdata, lis3mdlx1Edata;
  DimService *lis3mdlx1CDim, *lis3mdlx1EDim;

  struct lis3dhd
  {
    double gxmin = 0;
    double gx = 0;
    double gxmax = 0;
    double gymin = 0;
    double gy = 0;
    double gymax = 0;
    double gzmin = 0;
    double gz = 0;
    double gzmax = 0;
    int adc1 = 0;
    int adc2 = 0;
    int adc3 = 0;
    int odr = 0;
  };

  lis3dhd lis3dhx18data, lis3dhx19data;
  DimService *lis3dhx18Dim, *lis3dhx19Dim;

  struct max31865d
  {
    double T = 0;
    double R = 0;
    int F = 0;
  };

  max31865d max31865d00data, max31865d01data, max31865d02data, max31865d03data, max31865d04data, max31865d05data, max31865d06data, max31865d07data;
  DimService *max31865d00Dim, *max31865d01Dim, *max31865d02Dim, *max31865d03Dim, *max31865d04Dim, *max31865d05Dim, *max31865d06Dim, *max31865d07Dim;

  struct tmp102d
  {
    double T = 0;
  };

  tmp102d tmp102x48data, tmp102x49data, tmp102x4Adata, tmp102x4Bdata;
  DimService *tmp102x48Dim, *tmp102x49Dim, *tmp102x4ADim, *tmp102x4BDim;

  struct pca9535d
  {
    int inputs = 0;
    int outputs = 0;
    int inversions = 0;
    int portconfigs = 0;
  };

  pca9535d pca9535x20data, pca9535x21data, pca9535x22data, pca9535x23data, pca9535x24data, pca9535x25data, pca9535x26data, pca9535x27data;
  DimService *pca9535x20Dim, *pca9535x21Dim, *pca9535x22Dim, *pca9535x23Dim, *pca9535x24Dim, *pca9535x25Dim, *pca9535x26Dim, *pca9535x27Dim;


  if( bmp280x76 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bmp280x76\n");
    bmp280x76Dim = new DimService( (dimserver + "/bmp280x76").c_str(), "D:2", &bmp280x76data, sizeof( bmp280x76data ) );
  }
  else
  {
    bmp280x76Dim = nullptr; 
  }

  if( bmp280x77 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bmp280x77\n");
    bmp280x77Dim = new DimService( (dimserver + "/bmp280x77").c_str(), "D:2", &bmp280x77data, sizeof( bmp280x77data ) );
  }
  else
  {
    bmp280x77Dim = nullptr; 
  }

  if( bme680x76 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bme680x76\n");
    bme680x76Dim = new DimService( (dimserver + "/bme680x76").c_str(), "D:4", &bme680x76data, sizeof( bme680x76data ) );
  }
  else
  {
    bme680x76Dim = nullptr; 
  }

  if( bme680x77 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bme680x77\n");
    bme680x77Dim = new DimService( (dimserver + "/bme680x77").c_str(), "D:4", &bme680x77data, sizeof( bme680x77data ) );
  }
  else
  {
    bme680x77Dim = nullptr; 
  }

  if( bh1750fvix23 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bh1750fvix23\n");
    bh1750fvix23Dim = new DimService( (dimserver + "/bh1750fvix23").c_str(), "D:1", &bh1750fvix23data, sizeof( bh1750fvix23data ) );
  }
  else
  {
    bh1750fvix23Dim = nullptr;
  }

  if( bh1750fvix5C )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service bh1750fvix5C\n");
    bh1750fvix5CDim = new DimService( (dimserver + "/bh1750fvix5C").c_str(), "D:1", &bh1750fvix5Cdata, sizeof( bh1750fvix5Cdata ) );
  }
  else
  {
    bh1750fvix5CDim = nullptr;
  }

  if( htu21dx )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service htu21dx\n");
    htu21Dim = new DimService( (dimserver + "/htu21dx").c_str(), "D:2", &htu21data, sizeof( htu21data ) );
  }
  else
  {
    htu21Dim = nullptr;
  }

  if( lis2mdlx1E )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service lis2mdlx1E\n");
    lis2mdlx1EDim = new DimService( (dimserver + "/lis2mdlx1E").c_str(), "D:4", &lis2mdlx1Edata, sizeof( lis2mdlx1Edata ) );
  }
  else
  {
    lis2mdlx1EDim = nullptr;
  }

  if( lis3mdlx1C )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service lis3mdlx1C\n");
    lis3mdlx1CDim = new DimService( (dimserver + "/lis3mdlx1C").c_str(), "D:4", &lis3mdlx1Cdata, sizeof( lis3mdlx1Cdata ) );
  }
  else
  {
    lis3mdlx1CDim = nullptr;
  }

  if( lis3mdlx1E )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service lis3mdlx1E\n");
    lis3mdlx1EDim = new DimService( (dimserver + "/lis3mdlx1E").c_str(), "D:4", &lis3mdlx1Edata, sizeof( lis3mdlx1Edata ) );
  }
  else
  {
    lis3mdlx1EDim = nullptr;
  }

  if( lis3dhx18 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service lis3dhx18\n");
    lis3dhx18Dim = new DimService( (dimserver + "/lis3dhx18").c_str(), "D:9;I:4", &lis3dhx18data, sizeof( lis3dhx18data ) );
  }
  else
  {
    lis3dhx18Dim = nullptr;
  }

  if( lis3dhx19 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service lis3dhx19\n");
    lis3dhx19Dim = new DimService( (dimserver + "/lis3dhx19").c_str(), "D:9;I:4", &lis3dhx19data, sizeof( lis3dhx19data ) );
  }
  else
  {
    lis3dhx19Dim = nullptr;
  }

  if( max31865_00 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d00\n");
    max31865d00Dim = new DimService( (dimserver + "/max31865d00").c_str(), "D:2;I:1", &max31865d00data, sizeof( max31865d00data ) );
  }
  else
  {
    max31865d00Dim = nullptr;
  }

  if( max31865_01 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d01\n");
    max31865d01Dim = new DimService( (dimserver + "/max31865d01").c_str(), "D:2;I:1", &max31865d01data, sizeof( max31865d01data ) );
  }
  else
  {
    max31865d01Dim = nullptr;
  }

  if( max31865_02 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d02\n");
    max31865d02Dim = new DimService( (dimserver + "/max31865d02").c_str(), "D:2;I:1", &max31865d02data, sizeof( max31865d02data ) );
  }
  else
  {
    max31865d02Dim = nullptr;
  }

  if( max31865_03 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d03\n");
    max31865d03Dim = new DimService( (dimserver + "/max31865d03").c_str(), "D:2;I:1", &max31865d03data, sizeof( max31865d03data ) );
  }
  else
  {
    max31865d03Dim = nullptr;
  }

  if( max31865_04 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d04\n");
    max31865d04Dim = new DimService( (dimserver + "/max31865d04").c_str(), "D:2;I:1", &max31865d04data, sizeof( max31865d04data ) );
  }
  else
  {
    max31865d04Dim = nullptr;
  }

  if( max31865_05 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d05\n");
    max31865d05Dim = new DimService( (dimserver + "/max31865d05").c_str(), "D:2;I:1", &max31865d05data, sizeof( max31865d05data ) );
  }
  else
  {
    max31865d05Dim = nullptr;
  }

  if( max31865_06 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d06\n");
    max31865d06Dim = new DimService( (dimserver + "/max31865d06").c_str(), "D:2;I:1", &max31865d06data, sizeof( max31865d06data ) );
  }
  else
  {
    max31865d06Dim = nullptr;
  }

  if( max31865_07 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service max31865d07\n");
    max31865d07Dim = new DimService( (dimserver + "/max31865d07").c_str(), "D:2;I:1", &max31865d07data, sizeof( max31865d07data ) );
  }
  else
  {
    max31865d07Dim = nullptr;
  }

  if( tmp102x48 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service tmp102x48\n");
    tmp102x48Dim = new DimService( (dimserver + "/tmp102x48").c_str(), "D:1", &tmp102x48data, sizeof( tmp102x48data ) );
  }
  else
  {
    tmp102x48Dim = nullptr;
  }

  if( tmp102x49 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service tmp102x49\n");
    tmp102x49Dim = new DimService( (dimserver + "/tmp102x49").c_str(), "D:1", &tmp102x49data, sizeof( tmp102x49data ) );
  }
  else
  {
    tmp102x49Dim = nullptr;
  }

  if( tmp102x4A )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service tmp102x4A\n");
    tmp102x4ADim = new DimService( (dimserver + "/tmp102x4A").c_str(), "D:1", &tmp102x4Adata, sizeof( tmp102x4Adata ) );
  }
  else
  {
    tmp102x4ADim = nullptr;
  }

  if( tmp102x4B )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service tmp102x4B\n");
    tmp102x4BDim = new DimService( (dimserver + "/tmp102x4B").c_str(), "D:1", &tmp102x4Bdata, sizeof( tmp102x4Bdata ) );
  }
  else
  {
    tmp102x4BDim = nullptr;
  }

  if( pca9535x20 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x20\n");
    pca9535x20Dim = new DimService( (dimserver + "/pca9535x20").c_str(), "I:4", &pca9535x20data, sizeof( pca9535x20data ) );
  }
  else
  {
    pca9535x20Dim = nullptr;
  }

  if( pca9535x21 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x21\n");
    pca9535x21Dim = new DimService( (dimserver + "/pca9535x21").c_str(), "I:4", &pca9535x21data, sizeof( pca9535x21data ) );
  }
  else
  {
    pca9535x21Dim = nullptr;
  }

  if( pca9535x22 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x22\n");
    pca9535x22Dim = new DimService( (dimserver + "/pca9535x22").c_str(), "I:4", &pca9535x22data, sizeof( pca9535x22data ) );
  }
  else
  {
    pca9535x22Dim = nullptr;
  }

  if( pca9535x23 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x23\n");
    pca9535x23Dim = new DimService( (dimserver + "/pca9535x23").c_str(), "I:4", &pca9535x23data, sizeof( pca9535x23data ) );
  }
  else
  {
    pca9535x23Dim = nullptr;
  }

  if( pca9535x24 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x24\n");
    pca9535x24Dim = new DimService( (dimserver + "/pca9535x24").c_str(), "I:4", &pca9535x24data, sizeof( pca9535x24data ) );
  }
  else
  {
    pca9535x24Dim = nullptr;
  }

  if( pca9535x25 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x25\n");
    pca9535x25Dim = new DimService( (dimserver + "/pca9535x25").c_str(), "I:4", &pca9535x25data, sizeof( pca9535x25data ) );
  }
  else
  {
    pca9535x25Dim = nullptr;
  }

  if( pca9535x26 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x26\n");
    pca9535x26Dim = new DimService( (dimserver + "/pca9535x26").c_str(), "I:4", &pca9535x26data, sizeof( pca9535x26data ) );
  }
  else
  {
    pca9535x26Dim = nullptr;
  }

  if( pca9535x27 )
  {
    fprintf(stderr, SD_DEBUG "Create DIM service pca9535x27\n");
    pca9535x27Dim = new DimService( (dimserver + "/pca9535x27").c_str(), "I:4", &pca9535x27data, sizeof( pca9535x27data ) );
  }
  else
  {
    pca9535x27Dim = nullptr;
  }

  // start DIM server
  bool dimruns = false;
  if( dimserver != "" )
  {
    DimServer::setDnsNode( dimdns.c_str() );
    DimServer::start( dimserver.c_str() );
    dimruns = true;
  }
#endif

  // chip initializations
  for( int i = 0; i < 4; i++)
  {
    if( tmp102[ i ] )
    {
      switch( i )
      {
        case 0:
          tmp102[ 0 ]->SetAddress0x48();
	  break;
	case 1:
          tmp102[ 1 ]->SetAddress0x49();
	  break;
	case 2:
          tmp102[ 2 ]->SetAddress0x4A();
	  break;
        case 3:
          tmp102[ 3 ]->SetAddress0x4B();
	  break;
	default:
          cout << "-- program bug\n";
          return -1;
      }

      fprintf(stderr, SD_INFO "%s %s %d\n", tmp102[ i ]->GetName().c_str(), tmp102[ i ]->GetDevice().c_str(), tmp102[ i ]->GetAddress() );

      tmp102[ i ]->SetPointer(TMP102_TEMP_REG);
  
      fprintf(stderr, SD_DEBUG "SQLite file: %s\n", tmp102_db->GetFile().c_str() );
      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", tmp102_db->GetTable().c_str() );
      fprintf(stderr, SD_DEBUG "SQLite datetime(): %s\n", tmp102_db->GetDateTime( sqlite_err ).c_str() );
      fprintf(stderr, SD_DEBUG "SQLite error: %d\n", sqlite_err );
    }
  }

  if( htu21d )
  {
    fprintf(stderr, SD_INFO "%s %s\n", htu21d->GetName().c_str(), htu21d->GetDevice().c_str() );
    fprintf(stderr, SD_DEBUG "SQLite table: %s\n", htu21d_db->GetTable().c_str() );

  }

  for( int i = 0; i < 2; i++)
  {
    if( bmp280[ i ] )
    {
      if( i == 0 ) bmp280[ i ]->SetAddress0x76(); 
      else bmp280[ i ]->SetAddress0x77();

      fprintf(stderr, SD_INFO "%s %s %d\n", bmp280[ i ]->GetName().c_str(), bmp280[ i ]->GetDevice().c_str(), bmp280[ i ]->GetAddress() );

      fprintf(stderr, SD_DEBUG "read BMP280 calibration data\n");
      if( !bmp280[ i ]->GetCalibration() )
      {
        fprintf(stderr, SD_ERR "problem reading BMP280 calibration data, quit now\n");
        return -1;
      }

      fprintf(stderr, SD_INFO "%d T and %d p oversampling\n", bmp280_TOverSample[ i ], bmp280_POverSample[ i ]);
      bmp280[ i ]->SetTOverSample( bmp280_TOverSample[ i ] );
      bmp280[ i ]->SetPOverSample( bmp280_POverSample[ i ] );

      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", bmp280_db->GetTable().c_str() );

    }
  }

  for( int i = 0; i < 2; i++)
  {
    if( bme680[ i ] )
    {
      if( i == 0 ) bme680[ i ]->SetAddress0x76();
      else bme680[ i ]->SetAddress0x77();

      fprintf(stderr, SD_INFO "%s %s %d\n", bme680[ i ]->GetName().c_str(), bme680[ i ]->GetDevice().c_str(), bme680[ i ]->GetAddress() );

      fprintf(stderr, SD_DEBUG "read BME680 calibration data\n");
      if( !bme680[ i ]->GetCalibration() )
      {
        fprintf(stderr, SD_ERR "problem reading BME680 calibration data, quit now\n");
        return -1;
      }

      fprintf(stderr, SD_INFO "%d H, %d T and %d p oversampling\n", bme680_HOverSample[ i ], bme680_TOverSample[ i ], bme680_POverSample[ i ] );
      bme680[ i ]->SetOverSample( bme680_HOverSample[ i ], bme680_TOverSample[ i ], bme680_POverSample[ i ] );

      fprintf(stderr, SD_INFO "filter %d\n", bme680_Filter[ i ] );
      bme680[ i ]->SetFilter( bme680_Filter[ i ] );

      fprintf(stderr, SD_INFO "profile 0: 0x%02x pulse, ambient %d C, target %d C\n", bme680_GasWaitTime[ i ], Tamb, bme680_T0[ i ]);
      bme680[ i ]->SetGasWaitTime(0, bme680_GasWaitTime[ i ]); 
      bme680[ i ]->SetGasHeatTemperature(0, Tamb, bme680_T0[ i ]); // 
      bme680[ i ]->RunGas();
      bme680[ i ]->HeaterOn();
      bme680[ i ]->SetHeaterProfile( 0 );

      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", bme680_db->GetTable().c_str() );
    }
  }

  for( int i = 0; i < 2; i++)
  {
    if( bh1750fvi[ i ] )
    {
      if( i == 0 ) bh1750fvi[ i ]->SetAddress0x23(); 
      else bh1750fvi[ i ]->SetAddress0x5C();

      fprintf(stderr, SD_INFO "%s %s %d\n", bh1750fvi[ i ]->GetName().c_str(), bh1750fvi[ i ]->GetDevice().c_str(), bh1750fvi[ i ]->GetAddress() );

      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", bh1750fvi_db->GetTable().c_str() );
    }
  }    
    
  for( int i = 0; i < 2; i++)
  {
    if( lis3dh[ i ] )
    {
      if( i == 0 ) lis3dh[ i ]->SetAddress0x18(); 
      else lis3dh[ i ]->SetAddress0x19();

      if( lis3dh[ i ]->WhoAmI() )
      {
        fprintf(stderr, SD_INFO "%s %s %d\n", lis3dh[ i ]->GetName().c_str(), lis3dh[ i ]->GetDevice().c_str(), lis3dh[ i ]->GetAddress() );
        fprintf(stderr, SD_DEBUG "SQLite table: %s\n", lis3dh_db->GetTable().c_str() );
        fprintf(stderr, SD_INFO "Reboot memory content\n");
        lis3dh[ i ]->Boot( );
        usleep( 10000 );

        lis3dh[ i ]->SetDataRate( 0 );

        fprintf(stderr, SD_INFO "Enable X, Y and Z\n");
        lis3dh[ i ]->XEnable();
        lis3dh[ i ]->YEnable();
        lis3dh[ i ]->ZEnable();

        fprintf(stderr, SD_INFO "Set data rate 25 Hz\n");
        lis3dh[ i ]->SetDataRate( 3 );

        fprintf(stderr, SD_INFO "Set block data update\n");
        lis3dh[ i ]->BlockDataEnable();

        fprintf(stderr, SD_INFO "Start normal mode\n");
        lis3dh[ i ]->NormalMode();

        fprintf(stderr, SD_INFO "Enable ADC\n");
        lis3dh[ i ]->ADCEnable();

        fprintf(stderr, SD_INFO "Enable temperature measurement\n");
        lis3dh[ i ]->TempEnable();

        fprintf(stderr, SD_INFO "Enable FIFO\n");
        lis3dh[ i ]->FifoEnable();

	fprintf(stderr, SD_INFO "Set FIFO mode 2 Stream\n");
        lis3dh[ i ]->SetFifoMode( 2 );
      }
      else
      {
        fprintf(stderr, SD_ERR "LIS3DH not found from i2c bus address 0x%2X, drop from reading loop\n", lis3dh[ i ]->GetAddress() );
        lis3dh[ i ] = nullptr;
      }
    }
  }

  if( lis2mdl )
  {
    if( lis2mdl->WhoAmI() )
    {
      fprintf(stderr, SD_INFO "Enable temperature compensation\n");
      lis2mdl->TempCompEnable();
      fprintf(stderr, SD_INFO "Enable block data\n");
      lis2mdl->BlockDataEnable();

      fprintf(stderr, SD_INFO "%s %s %d\n", lis2mdl->GetName().c_str(), lis2mdl->GetDevice().c_str(), lis2mdl->GetAddress() );
      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", lis2mdl_db->GetTable().c_str() );
      fprintf(stderr, SD_INFO "Operation mode %d\n", lis2mdl->GetOpMode());
      fprintf(stderr, SD_INFO "Data rate %d\n", lis2mdl->GetDataRate());
      fprintf(stderr, SD_INFO "X offset register %d\n", lis2mdl->GetXOffset());
      fprintf(stderr, SD_INFO "Y offset register %d\n", lis2mdl->GetYOffset());
      fprintf(stderr, SD_INFO "Z offset register %d\n", lis2mdl->GetZOffset());
    }
    else
    {
      fprintf(stderr, SD_ERR "LIS2MDL not found from i2c bus address 0x1E, drop from reading loop\n");
      lis2mdl = nullptr;
    }
  }

  for( int i = 0; i < 2; i++)
  {
    if( lis3mdl[ i ] )
    {
      if( i == 0 ) lis3mdl[ i ]->SetAddress0x1C(); 
      else lis3mdl[ i ]->SetAddress0x1E();

      if( lis3mdl[ i ]->WhoAmI() )
      {
        lis3mdl[ i ]->SetXYOpMode( 3 );
        lis3mdl[ i ]->SetZOpMode( 3 );
        lis3mdl[ i ]->SetFullScale( 0 );
        lis3mdl[ i ]->FastReadEnable();
        lis3mdl[ i ]->TempEnable();

        fprintf(stderr, SD_INFO "%s %s %d\n", lis3mdl[ i ]->GetName().c_str(), lis3mdl[ i ]->GetDevice().c_str(), lis3mdl[ i ]->GetAddress() );
        fprintf(stderr, SD_DEBUG "SQLite table: %s\n", lis3mdl_db->GetTable().c_str() );
        fprintf(stderr, SD_INFO "XY operation mode %d\n", lis3mdl[ i ]->GetXYOpMode());
        fprintf(stderr, SD_INFO "Z operation mode %d\n", lis3mdl[ i ]->GetZOpMode());
        fprintf(stderr, SD_INFO "Full scale %d\n", lis3mdl[ i ]->GetFullScale());
        fprintf(stderr, SD_INFO "Fast read enabled\n");
        fprintf(stderr, SD_INFO "Enable temperature sensor\n");
      }
      else
      {
        fprintf(stderr, SD_ERR "LIS3MDL not found from i2c bus address 0x%2X, drop from reading loop\n", lis3mdl[ i ]->GetAddress() );
        lis3mdl[ i ] = nullptr;
      }
    }
  }    

  for( int i = 0; i < 8; i++)
  {
    if( max31865[ i ] )
    {
      max31865[ i ]->SetLowFault( 400 );
      max31865[ i ]->SetHighFault( 40000 );
      max31865[ i ]->BiasOn();

      fprintf(stderr, SD_INFO "%s %s\n", max31865[ i ]->GetName().c_str(), max31865[ i ]->GetDevice().c_str() );
      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", max31865_db->GetTable().c_str() );
      fprintf(stderr, SD_INFO "low fault %d high fault %d\n", max31865[ i ]->GetLowFault(), max31865[ i ]->GetHighFault() );
    }
  }

  for( int i = 0; i < 8; i++)
  {
    if( pca9535[ i ] )
    {
      fprintf(stderr, SD_INFO "%s %s\n", pca9535[ i ]->GetName().c_str(), pca9535[ i ]->GetDevice().c_str() );
      fprintf(stderr, SD_DEBUG "SQLite table: %s\n", pca9535_db->GetTable().c_str() );
    }
  }

  int inputs = 0, outputs = 0, inversions = 0, portconfigs = 0;
  double T = 0, TF = 0, RH = 0, p = 0, R = 0, Ev = 0;
  double Bx = 0, By = 0, Bz = 0;
  double gx = 0, gy = 0, gz = 0;
  double gxmin = 0, gymin = 0, gzmin = 0;
  double gxmax = 0, gymax = 0, gzmax = 0;
  int adc1 = 0, adc2 = 0, adc3 = 0;
  uint8_t samples = 0;
  int16_t *fifoX = new int16_t[ 32 ]; 
  int16_t *fifoY = new int16_t[ 32 ]; 
  int16_t *fifoZ = new int16_t[ 32 ]; 
  int16_t xmedian = 0, xmin = 0, xmax = 0, ymedian = 0, ymin = 0, ymax =0, zmedian = 0, zmin = 0, zmax =0;
  uint8_t ODR = 0;
  char Valid = 'N', Stable = 'N';
  int F = 0;
  double dbl_array[ 12 ];
  int int_array[ 10 ];
  int j = 0;
  while( cont )
  {
    for(int i = 0; i < 4; i++)
    {
      if( tmp102[ i ] )
      {	      
        tmp102[ i ]->ReadTemperature();
        T = tmp102[ i ]->GetTemperature();

        fprintf(stderr, SD_INFO "%s = %f C\n", tmp102[ i ]->GetName().c_str(), T);
	tmp102_file[ i ]->Write( T );
        dbl_array[ 0 ] = T;
        tmp102_db->Insert(tmp102[ i ]->GetName(), 1, dbl_array, sqlite_err );
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	if( dimruns )
        {
          if( i == 0 )
          {
            fprintf(stderr, SD_DEBUG "Update DIM service tmp102x48\n");
            tmp102x48data.T = T;
            tmp102x48Dim->updateService();
          }
          else if( i == 1 )
          {
            fprintf(stderr, SD_DEBUG "Update DIM service tmp102x49\n");
            tmp102x49data.T = T;
            tmp102x49Dim->updateService();
          }
          else if( i == 2 )
          {
            fprintf(stderr, SD_DEBUG "Update DIM service tmp102x4A\n");
            tmp102x4Adata.T = T;
            tmp102x4ADim->updateService();
          }
          else if( i == 3 )
          {
            fprintf(stderr, SD_DEBUG "Update DIM service tmp102x4B\n");
            tmp102x4Bdata.T = T;
            tmp102x4BDim->updateService();
          }
	}
#endif
      }
    }

    if( htu21d )
    {
      htu21d->TriggerTemperature();
      usleep( 50000 ); // 50 ms delay
      if( htu21d->ReadTemperature() )
      {
        T = htu21d->GetTemperature();
        htu21d_T_file->Write( T );

        htu21d->TriggerHumidity();
        usleep( 50000 ); // 50 ms delay

        if( htu21d->ReadHumidity() )
        {
          RH = htu21d->GetHumidity();
          fprintf(stderr, SD_INFO "%s = %f C, %f %%\n", htu21d->GetName().c_str(), T, RH);

	  htu21d_RH_file->Write( RH );

          dbl_array[ 0 ] = T;
          dbl_array[ 1 ] = RH;

          htu21d_db->Insert(htu21d->GetName(), 2, dbl_array, sqlite_err );
          if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
          if( dimruns )
          {
            htu21data.T = T;
            htu21data.RH = RH;
            htu21Dim->updateService();
          }
#endif
	}
      }
    }

    for(int i = 0; i < 2; i++)
    {
      if( bmp280[ i ] )
      {
        bmp280[ i ]->Forced();
        usleep( 10000 ); // 10 ms
        bmp280[ i ]->Measure();

	T = bmp280[ i ]->GetTemperature();
	p = bmp280[ i ]->GetPressure();

        fprintf(stderr, SD_INFO "%s = %f C, %f Pa\n", bmp280[ i ]->GetName().c_str(), T, p);

        bmp280_T_file[ i ]->Write( T );
        bmp280_p_file[ i ]->Write( p );

        dbl_array[ 0 ] = T;
        dbl_array[ 1 ] = p;

        bmp280_db->Insert(bmp280[ i ]->GetName(), 2, dbl_array, sqlite_err);
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	if( dimruns )
        {
          if( i == 0 )
          {
            bmp280x76data.T = T;
            bmp280x76data.p = p;
            bmp280x76Dim->updateService();
          }
          else
	  {
            bmp280x77data.T = T;
            bmp280x77data.p = p;
            bmp280x77Dim->updateService();
          }
        }
#endif
      }
    }

    for(int i = 0; i < 2; i++)
    {
      if( bme680[ i ] )
      {
        bme680[ i ]->Forced();
        usleep( 200000 ); // 200 ms
        bme680[ i ]->GetTPHG();

        T = bme680[ i ]->GetTemperature();
        TF = 9.0 * T / 5.0 + 32.0;
	RH = bme680[ i ]->GetHumidity();
        p = bme680[ i ]->GetPressure();
        R = bme680[ i ]->GetResistance();

        if( bme680[ i ]->GasValid() ) Valid = 'Y'; else Valid = 'N';
        if( bme680[ i ]->HeaterStable() ) Stable = 'Y'; else Stable = 'N';

        fprintf(stderr, SD_INFO "%s = %f C, %f %%, %f Pa, %f ohm, %c, %c\n", bme680[ i ]->GetName().c_str(), T, RH, p, R, Valid, Stable);

        bme680_T_file[ i ]->Write( T );
        bme680_TF_file[ i ]->Write( TF );
	bme680_RH_file[ i ]->Write( RH );
        bme680_p_file[ i ]->Write( p );
        bme680_R_file[ i ]->Write( R );

        dbl_array[ 0 ] = T;
        dbl_array[ 1 ] = RH;
        dbl_array[ 2 ] = p;
        dbl_array[ 3 ] = R;
        int_array[ 0 ] = (int)bme680[ i ]->GasValid();
        int_array[ 1 ] = (int)bme680[ i ]->HeaterStable();

        bme680_db->Insert(bme680[ i ]->GetName(), 4, dbl_array, 2, int_array, sqlite_err);
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	if( i == 0 && dimruns )
        {
          fprintf(stderr, SD_DEBUG "Update DIM service bme680x76\n");
          bme680x76data.T = T;
          bme680x76data.RH = RH;
          bme680x76data.p = p;
          bme680x76data.R = R;
          bme680x76Dim->updateService();
        }
	else if( dimruns )
        {
          fprintf(stderr, SD_DEBUG "Update DIM service bme680x77\n");
          bme680x77data.T = T;
          bme680x77data.RH = RH;
          bme680x77data.p = p;
          bme680x77data.R = R;
          bme680x77Dim->updateService();
        }
#endif
        Tamb = (int8_t)T;

        fprintf(stderr, SD_INFO "profile 0: 0x%02x pulse, ambient %d C, target %d C\n", bme680_GasWaitTime[ i ], Tamb, bme680_T0[ i ]);
        bme680[ i ]->SetGasHeatTemperature(0, Tamb, bme680_T0[ i ]); // adjust ambient temperature 
      }
    }

    for(int i = 0; i < 2; i++)
    {
      if( bh1750fvi[ i ] )
      {
        bh1750fvi[ i ]->OneTimeHighResMode();
        usleep( 700000 ); // 700 ms

        if( bh1750fvi[ i ]->ReadIlluminance() )
        {
          Ev = bh1750fvi[ i ]->GetIlluminance();

          fprintf(stderr, SD_INFO "%s = %f lx\n", bh1750fvi[ i ]->GetName().c_str(), Ev);

          bh1750fvi_Ev_file[ i ]->Write( Ev );

          dbl_array[ 0 ] = Ev;

          bh1750fvi_db->Insert(bh1750fvi[ i ]->GetName(), 1, dbl_array, sqlite_err);
          if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	  if( i == 0 && dimruns )
          {
            bh1750fvix23data.Ev = Ev;
            bh1750fvix23Dim->updateService();
	  }
	  else if( dimruns )
          {
            bh1750fvix5Cdata.Ev = Ev;
            bh1750fvix5CDim->updateService();
          }
#endif
	}
      }
    }

    for(int i = 0; i < 2; i++)
    {
      if( lis3dh[ i ] )
      {
        j = 0;
        while( !lis3dh[ i ]->NewDataXYZ() && j < 2000 )
	{
          usleep( 1000 ); // sleep 1 ms
          j++;
        }
    
        if( j < 2000 )
        {
          samples = lis3dh[ i ]->ReadFifo();
          fprintf(stderr, SD_INFO "%s FIFO has %d samples\n", lis3dh[ i ]->GetName().c_str(), samples );
          ODR = lis3dh[ i ]->GetDataRate();	    

          if( samples > 0 )
          {
            gxmin = 0;
            gymin = 0;
            gzmin = 0;
      
            gxmax = 0;
            gymax = 0;
            gzmax = 0;

	    if( samples == 32 )
            {
              fifoX = lis3dh[ i ]->GetFifoX();
              fifoY = lis3dh[ i ]->GetFifoY();
              fifoZ = lis3dh[ i ]->GetFifoZ();

              sort( fifoX, fifoX + samples );
              sort( fifoY, fifoY + samples );
              sort( fifoZ, fifoZ + samples );

              xmedian = ( fifoX[ 15 ] + fifoX[ 16 ] ) / 2.0;
              xmin = fifoX[ 0 ];
              xmax = fifoX[ 31 ];

              ymedian = ( fifoY[ 15 ] + fifoY[ 16 ] ) / 2.0;
              ymin = fifoY[ 0 ];
              ymax = fifoY[ 31 ];

              zmedian = ( fifoZ[ 15 ] + fifoZ[ 16 ] ) / 2.0;
              zmin = fifoZ[ 0 ];
              zmax = fifoZ[ 31 ];

              fprintf(stderr, SD_INFO "xmin, xmed, xmax = [%d, %d, %d]\n", xmin, xmedian, xmax);
              fprintf(stderr, SD_INFO "ymin, ymed, ymax = [%d, %d, %d]\n", ymin, ymedian, ymax);
              fprintf(stderr, SD_INFO "zmin, zmed, zmax = [%d, %d, %d]\n", zmin, zmedian, zmax);

              gx = lis3dh[ i ]->GetFS() * (double)xmedian / 32768.0;
              gy = lis3dh[ i ]->GetFS() * (double)ymedian / 32768.0;
              gz = lis3dh[ i ]->GetFS() * (double)zmedian / 32768.0;

              gxmin = lis3dh[ i ]->GetFS() * (double)xmin / 32768.0;
              gymin = lis3dh[ i ]->GetFS() * (double)ymin / 32768.0;
              gzmin = lis3dh[ i ]->GetFS() * (double)zmin / 32768.0;

	      gxmax = lis3dh[ i ]->GetFS() * (double)xmax / 32768.0;
              gymax = lis3dh[ i ]->GetFS() * (double)ymax / 32768.0;
              gzmax = lis3dh[ i ]->GetFS() * (double)zmax / 32768.0;

              fprintf(stderr, SD_INFO "%s median gx = %f, gy = %f, gz = %f with ODR %d\n", lis3dh[ i ]->GetName().c_str(), gx, gy, gz, ODR);
	    }
	    else 
            {
              gx = lis3dh[ i ]->GetFS() * (double)fifoX[ 0 ] / 32768.0;
              gy = lis3dh[ i ]->GetFS() * (double)fifoY[ 0 ] / 32768.0;
              gz = lis3dh[ i ]->GetFS() * (double)fifoZ[ 0 ] / 32768.0;
          
              fprintf(stderr, SD_INFO "%s gx = %f, gy = %f, gz = %f\n", lis3dh[ i ]->GetName().c_str(), gx, gy, gz);
            }  

	    lis3dh_gx_file[ i ]->Write( gx );
            lis3dh_gy_file[ i ]->Write( gy );
            lis3dh_gz_file[ i ]->Write( gz );

            if( lis3dh[ i ]->ReadAdc() )
            {
              adc1 = lis3dh[ i ]->GetAdc1();
              adc2 = lis3dh[ i ]->GetAdc2();
              adc3 = lis3dh[ i ]->GetAdc3();

              fprintf(stderr, SD_INFO "%s adc1 = %d, adc2 = %d, adc3 = %d\n", lis3dh[ i ]->GetName().c_str(), adc1, adc2, adc3);

              lis3dh_adc1_file[ i ]->Write( adc1 );
              lis3dh_adc2_file[ i ]->Write( adc2 );
              lis3dh_adc3_file[ i ]->Write( adc3 );
	    }

            dbl_array[ 0 ] = gxmin;
            dbl_array[ 1 ] = gx;
            dbl_array[ 2 ] = gxmax;

	    dbl_array[ 3 ] = gymin;
            dbl_array[ 4 ] = gy;
            dbl_array[ 5 ] = gymax;

	    dbl_array[ 6 ] = gzmin;
            dbl_array[ 7 ] = gz;
            dbl_array[ 8 ] = gzmax;

	    int_array[ 0 ] = adc1;
            int_array[ 1 ] = adc2;
            int_array[ 2 ] = adc3;

	    int_array[ 3 ] = ODR;

            lis3dh_db->Insert(lis3dh[ i ]->GetName(), 9, dbl_array, 4, int_array, sqlite_err);
            if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	     if( i == 0 && dimruns )
             {
               lis3dhx18data.gxmin = gxmin;
               lis3dhx18data.gx = gx;
               lis3dhx18data.gxmax = gxmax;
               lis3dhx18data.gymin = gymin;
               lis3dhx18data.gy = gy;
               lis3dhx18data.gymax = gymax;
               lis3dhx18data.gzmin = gzmin;
               lis3dhx18data.gz = gz;
               lis3dhx18data.gzmax = gzmax;

	       lis3dhx18data.adc1 = adc1;
               lis3dhx18data.adc2 = adc2;
               lis3dhx18data.adc3 = adc3;
               lis3dhx18data.odr = ODR;

	       lis3dhx18Dim->updateService();
             }
             else if( dimruns )
	     {
               lis3dhx19data.gxmin = gxmin;
               lis3dhx19data.gx = gx;
               lis3dhx19data.gxmax = gxmax;
               lis3dhx19data.gymin = gymin;
               lis3dhx19data.gy = gy;
               lis3dhx19data.gymax = gymax;
               lis3dhx19data.gzmin = gzmin;
               lis3dhx19data.gz = gz;
               lis3dhx19data.gzmax = gzmax;

	       lis3dhx19data.adc1 = adc1;
               lis3dhx19data.adc2 = adc2;
               lis3dhx19data.adc3 = adc3;
               lis3dhx19data.odr = ODR;

	       lis3dhx19Dim->updateService();
	     }
#endif
	  }
          else
          {
            fprintf(stderr, SD_NOTICE "%s error reading g-force %d\n", lis3dh[ i ]->GetName().c_str(), lis3dh[ i ]->GetError() );
          }
        }
        else
        {
          fprintf(stderr, SD_NOTICE "%s reading timeout\n", lis3dh[ i ]->GetName().c_str());
        }
      }	
    }
    
    if( lis2mdl )
    {
      fprintf(stderr, SD_INFO "%s start single measurement mode\n", lis2mdl->GetName().c_str() );
      lis2mdl->SingleMode();

      j = 0;
      while( !lis2mdl->NewDataXYZ() && j < 1000 )
      {
        usleep( 100 ); // 100 us
	j++;
      }
  
      if( lis2mdl->NewDataXYZ() )
      {
        if( lis2mdl->OverRunXYZ() )
        {
          fprintf(stderr, SD_NOTICE "%s reading overrun\n", lis2mdl->GetName().c_str());
        }
	else
        {
          if( lis2mdl->ReadB() )
          {
            Bx = lis2mdl->GetBx();
            By = lis2mdl->GetBy();
            Bz = lis2mdl->GetBz();
            T = lis2mdl->GetT();

            fprintf(stderr, SD_INFO "%s Bx = %f uT, By = %f uT, Bz = %f uT , T = %f C\n", lis2mdl->GetName().c_str(), Bx, By, Bz, T);

            lis2mdl_Bx_file->Write( Bx );
            lis2mdl_By_file->Write( By );
            lis2mdl_Bz_file->Write( Bz );
            lis2mdl_T_file->Write( T );

            dbl_array[ 0 ] = Bx;
            dbl_array[ 1 ] = By;
            dbl_array[ 2 ] = Bz;
            dbl_array[ 3 ] = T;

            lis2mdl_db->Insert(lis2mdl->GetName(), 4, dbl_array, sqlite_err);

            if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	    if( dimruns )
	    {
              lis2mdlx1Edata.Bx = Bx;
              lis2mdlx1Edata.By = By;
	      lis2mdlx1Edata.Bz = Bz;
	      lis2mdlx1Edata.T = T;
              lis2mdlx1EDim->updateService();
	    }
#endif
	  }
          else
          {
            fprintf(stderr, SD_NOTICE "%s error reading magnetic field %d\n", lis2mdl->GetName().c_str(), lis2mdl->GetError() );
          }
        }
      }
      else
      {
        fprintf(stderr, SD_NOTICE "%s reading timeout\n", lis2mdl->GetName().c_str());
      }
    }

    for(int i = 0; i < 2; i++)
    {
      if( lis3mdl[ i ] )
      {
        lis3mdl[ i ]->ReadB();
        lis3mdl[ i ]->ContinuousMode();
//	lis3mdl[ i ]->SingleConversionMode();

        j = 0;
        while( !lis3mdl[ i ]->NewDataXYZ() && j < 1000 )
        {
          usleep( 100 ); // 100 us
          j++;
        }

        if( lis3mdl[ i ]->NewDataXYZ() )
        {
          if( lis3mdl[ i ]->OverRunXYZ() )
          {
            fprintf(stderr, SD_NOTICE "%s reading overrun\n", lis3mdl[ i ]->GetName().c_str());
          }
          else
          {
            if( lis3mdl[ i ]->ReadB() )
            {
              Bx = lis3mdl[ i ]->GetBx();
              By = lis3mdl[ i ]->GetBy();
              Bz = lis3mdl[ i ]->GetBz();
              T = lis3mdl[ i ]->GetT();

              fprintf(stderr, SD_INFO "%s Bx = %f uT, By = %f uT, Bz = %f uT , T = %f C\n", lis3mdl[ i ]->GetName().c_str(), Bx, By, Bz, T);

              lis3mdl_Bx_file[ i ]->Write( Bx );
              lis3mdl_By_file[ i ]->Write( By );
              lis3mdl_Bz_file[ i ]->Write( Bz );
              lis3mdl_T_file[ i ]->Write( T );

              dbl_array[ 0 ] = Bx;
              dbl_array[ 1 ] = By;
              dbl_array[ 2 ] = Bz;
              dbl_array[ 3 ] = T;

              lis3mdl_db->Insert(lis3mdl[ i ]->GetName(), 4, dbl_array, sqlite_err);
              if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	      if( dimruns && lis3mdlx1C )
  	      {
                lis3mdlx1Cdata.Bx = Bx;
                lis3mdlx1Cdata.By = By;
                lis3mdlx1Cdata.Bz = Bz;
	        lis3mdlx1Cdata.T = T;
                lis3mdlx1CDim->updateService();
              }
              else if( dimruns )
	      {
                lis3mdlx1Edata.Bx = Bx;
                lis3mdlx1Edata.By = By;
                lis3mdlx1Edata.Bz = Bz;
	        lis3mdlx1Edata.T = T;
                lis3mdlx1EDim->updateService();
              }
#endif
	    }
            else
            {
              fprintf(stderr, SD_NOTICE "%s error reading magnetic field %d\n", lis3mdl[ i ]->GetName().c_str(), lis3mdl[ i ]->GetError() );
            }
          }
	}
	else
        {
          fprintf(stderr, SD_NOTICE "%s reading timeout\n", lis3mdl[ i ]->GetName().c_str());
	}
        lis3mdl[ i ]->PowerDown();
        lis3mdl[ i ]->ReadB();
      }
    }

    for(int i = 0; i < 8; i++)
    {
      if( max31865[ i ] )
      {
        max31865[ i ]->OneShot();
        usleep( 100000 ); // 100 ms

	max31865[ i ]->FaultDetection();
        usleep( 1000 ); // 1 ms

	max31865[ i ]->ReadResistance();
	max31865[ i ]->CalcTemperature();

        T = max31865[ i ]->GetTemperature();
        R = max31865[ i ]->GetResistance();
        F = 0;
        if( max31865[ i ]->IsFault() ) F = (int)max31865[ i ]->GetFaultStatusByte();

	fprintf(stderr, SD_INFO "%s = %f C, %f ohm", max31865[ i ]->GetName().c_str(), T, R);
        if( F != 0 ) fprintf(stderr, ", fault = %d", F);
        fprintf(stderr, "\n");

	max31865_T_file[ i ]->Write( T );
	max31865_R_file[ i ]->Write( R );
	max31865_F_file[ i ]->Write( F );

        dbl_array[ 0 ] = T;
        dbl_array[ 1 ] = R;
        int_array[ 0 ] = F;

        max31865_db->Insert(max31865[ i ]->GetName(), 2, dbl_array, 1, int_array, sqlite_err);
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	if( dimruns )
        {
          if( i == 0 )
          {
            max31865d00data.T = T;
            max31865d00data.R = R;
            max31865d00data.F = F;
            max31865d00Dim->updateService();
	  }
          else if( i == 1 )
	  {
            max31865d01data.T = T;
            max31865d01data.R = R;
            max31865d01data.F = F;
            max31865d01Dim->updateService();
	  }
          else if( i == 2 )
	  {
            max31865d02data.T = T;
            max31865d02data.R = R;
            max31865d02data.F = F;
            max31865d02Dim->updateService();
	  }
          else if( i == 3 )
	  {
            max31865d03data.T = T;
            max31865d03data.R = R;
            max31865d03data.F = F;
            max31865d03Dim->updateService();
	  }
          else if( i == 4 )
	  {
            max31865d04data.T = T;
            max31865d04data.R = R;
            max31865d04data.F = F;
            max31865d04Dim->updateService();
	  }
          else if( i == 5 )
	  {
            max31865d05data.T = T;
            max31865d05data.R = R;
            max31865d05data.F = F;
            max31865d05Dim->updateService();
	  }
          else if( i == 6 )
	  {
            max31865d06data.T = T;
            max31865d06data.R = R;
            max31865d06data.F = F;
            max31865d06Dim->updateService();
	  }
          else if( i == 7 )
	  {
            max31865d07data.T = T;
            max31865d07data.R = R;
            max31865d07data.F = F;
            max31865d07Dim->updateService();
	  }
	}
#endif
      }
    }

    for(int i = 0; i < 8; i++)
    {
      if( pca9535[ i ] )
      {
        inputs = pca9535[ i ]->GetInputs();
        outputs = pca9535[ i ]->GetOutputs();
	inversions = pca9535[ i ]->GetPolInversions();
	portconfigs = pca9535[ i ]->GetPortConfigs();

        fprintf(stderr, SD_INFO "%s inputs = %d, outputs = %d, inversions = %d, portconfigs = %d\n", pca9535[ i ]->GetName().c_str(), inputs, outputs, inversions, portconfigs);

        pca9535_inputs_file[ i ]->Write( inputs );
        pca9535_outputs_file[ i ]->Write( outputs );
        pca9535_inversions_file[ i ]->Write( inversions );
        pca9535_port_configs_file[ i ]->Write( portconfigs );

        int_array[ 0 ] = inputs;
        int_array[ 1 ] = outputs;
        int_array[ 2 ] = inversions;
        int_array[ 3 ] = portconfigs;

	pca9535_db->Insert(pca9535[ i ]->GetName(), 4, int_array, sqlite_err);
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

#ifdef USE_DIM_LIBS
	if( dimruns )
        {
          if( i == 0 )
          {
            pca9535x20data.inputs = inputs;
            pca9535x20data.outputs = outputs;
            pca9535x20data.inversions = inversions;
            pca9535x20data.portconfigs = portconfigs;
	    pca9535x20Dim->updateService();
	  }
          else if( i == 1 )
          {
            pca9535x21data.inputs = inputs;
            pca9535x21data.outputs = outputs;
            pca9535x21data.inversions = inversions;
            pca9535x21data.portconfigs = portconfigs;
	    pca9535x21Dim->updateService();
	  }
          else if( i == 2 )
          {
            pca9535x22data.inputs = inputs;
            pca9535x22data.outputs = outputs;
            pca9535x22data.inversions = inversions;
            pca9535x22data.portconfigs = portconfigs;
	    pca9535x22Dim->updateService();
	  }
          else if( i == 3 )
          {
            pca9535x23data.inputs = inputs;
            pca9535x23data.outputs = outputs;
            pca9535x23data.inversions = inversions;
            pca9535x23data.portconfigs = portconfigs;
	    pca9535x23Dim->updateService();
	  }
          else if( i == 4 )
          {
            pca9535x24data.inputs = inputs;
            pca9535x24data.outputs = outputs;
            pca9535x24data.inversions = inversions;
            pca9535x24data.portconfigs = portconfigs;
	    pca9535x24Dim->updateService();
	  }
          else if( i == 5 )
          {
            pca9535x25data.inputs = inputs;
            pca9535x25data.outputs = outputs;
            pca9535x25data.inversions = inversions;
            pca9535x25data.portconfigs = portconfigs;
	    pca9535x25Dim->updateService();
	  }
          else if( i == 6 )
          {
            pca9535x26data.inputs = inputs;
            pca9535x26data.outputs = outputs;
            pca9535x26data.inversions = inversions;
            pca9535x26data.portconfigs = portconfigs;
	    pca9535x26Dim->updateService();
	  }
          else if( i == 7 )
          {
            pca9535x27data.inputs = inputs;
            pca9535x27data.outputs = outputs;
            pca9535x27data.inversions = inversions;
            pca9535x27data.portconfigs = portconfigs;
	    pca9535x27Dim->updateService();
	  }
        }
#endif
      }
    }

    sleep( readinterval );
  }

  return 0;
};

