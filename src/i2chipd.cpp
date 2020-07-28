/**************************************************************************
 * 
 * Read chips with I2C interface. 
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
 * Fri Jul  3 20:16:26 CDT 2020
 * Edit: Mon Jul 27 18:41:35 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#include "i2chipd.hpp"
#include <systemd/sd-daemon.h>
#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <iomanip>

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
  const int version = 20200727; // program version
  
  string i2cdev = "/dev/i2c-1";
  string datadir = "/var/lib/i2chipd/";
  string sqlitedb = "/var/lib/i2chipd/i2chipd.db";

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

  // chip objects to read, use nullptr if not wired
  Tmp102 *tmp102[ 4 ];
  if( true  ) tmp102[ 0 ] = new Tmp102("T1", i2cdev); else tmp102[ 0 ] = nullptr;
  if( false ) tmp102[ 1 ] = new Tmp102("T2", i2cdev); else tmp102[ 1 ] = nullptr;
  if( false ) tmp102[ 2 ] = new Tmp102("T3", i2cdev); else tmp102[ 2 ] = nullptr;
  if( false ) tmp102[ 3 ] = new Tmp102("T4", i2cdev); else tmp102[ 3 ] = nullptr;

  Htu21d *htu21d;
  if( true ) htu21d = new Htu21d("TH1", i2cdev); else htu21d = nullptr;

  Bmp280 *bmp280[ 2 ];
  if( false ) bmp280[ 0 ] = new Bmp280("Tp1", i2cdev); else bmp280[ 0 ] = nullptr;
  if( false ) bmp280[ 1 ] = new Bmp280("Tp2", i2cdev); else bmp280[ 1 ] = nullptr;

  Bme680 *bme680[ 2 ];
  if( false ) bme680[ 0 ] = new Bme680("TpHG1", i2cdev); else bme680[ 0 ] = nullptr;
  if( true  ) bme680[ 1 ] = new Bme680("TpHG2", i2cdev); else bme680[ 1 ] = nullptr;

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

  File *bme680_T_file[ 2 ], *bme680_p_file[ 2 ], *bme680_RH_file[ 2 ], *bme680_R_file[ 2 ];

  bme680_T_file[ 0 ]  = new File(datadir, "bme680_x76_T");
  bme680_T_file[ 1 ]  = new File(datadir, "bme680_x77_T");
  bme680_p_file[ 0 ]  = new File(datadir, "bme680_x76_p");
  bme680_p_file[ 1 ]  = new File(datadir, "bme680_x77_p");
  bme680_RH_file[ 0 ] = new File(datadir, "bme680_x76_RH");
  bme680_RH_file[ 1 ] = new File(datadir, "bme680_x77_RH");
  bme680_R_file[ 0 ]  = new File(datadir, "bme680_x76_R");
  bme680_R_file[ 1 ]  = new File(datadir, "bme680_x77_R");

  // SQLite objects to store values in database table
  SQLite *tmp102_db  = new SQLite(sqlitedb, "tmp102", "insert into tmp102 (name,temperature) values (?,?)");
  SQLite *htu21d_db = new SQLite(sqlitedb, "htu21d", "insert into htu21d (name,temperature,humidity) values (?,?,?)");
  SQLite *bmp280_db  = new SQLite(sqlitedb, "bmp280", "insert into bmp280 (name,temperature,pressure) values (?,?,?)");
  SQLite *bme680_db  = new SQLite(sqlitedb, "bme680", "insert into bme680 (name,temperature,humidity,pressure,resistance,gasvalid,stable) values (?,?,?,?,?,?,?)");

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

  double T = 0, RH = 0, p = 0, R = 0;
  char Valid = 'N', Stable = 'N';
  double dbl_array[ 10 ];
  int int_array[ 10 ];
  while( cont )
  {
    for(int i = 0; i < 4; i++)
    {
      if( tmp102[ i ] )
      {	      
        tmp102[ i ]->ReadTemperature();
        T = tmp102[ i ]->GetTemperature();
        tmp102_file[ i ]->Write( T );
        dbl_array[ 0 ] = T;
        tmp102_db->Insert(tmp102[ i ]->GetName(), 1, dbl_array, sqlite_err );
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);

        fprintf(stderr, SD_INFO "%s = %f C\n", tmp102[ i ]->GetName().c_str(), T);
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
          htu21d_RH_file->Write( RH );

          dbl_array[ 0 ] = T;
          dbl_array[ 1 ] = RH;

          htu21d_db->Insert(htu21d->GetName(), 2, dbl_array, sqlite_err );
          if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);
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

        bmp280_T_file[ i ]->Write( T );
        bmp280_p_file[ i ]->Write( p );

        dbl_array[ 0 ] = T;
        dbl_array[ 1 ] = p;

        bmp280_db->Insert(bmp280[ i ]->GetName(), 2, dbl_array, sqlite_err);
        if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database: %d\n", sqlite_err);
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
        RH = bme680[ i ]->GetHumidity();
        p = bme680[ i ]->GetPressure();
        R = bme680[ i ]->GetResistance();

        bme680_T_file[ i ]->Write( T );
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

        Tamb = (int8_t)T;

        if( bme680[ i ]->GasValid() ) Valid = 'Y'; else Valid = 'N';
        if( bme680[ i ]->HeaterStable() ) Stable = 'Y'; else Stable = 'N';

        fprintf(stderr, SD_INFO "%s = %f C, %f %%, %f Pa, %f Ohm, %c, %c\n", bme680[ 1 ]->GetName().c_str(), T, RH, p, R, Valid, Stable);

        fprintf(stderr, SD_INFO "profile 0: 0x%02x pulse, ambient %d C, target %d C\n", bme680_GasWaitTime[ i ], Tamb, bme680_T0[ i ]);
        bme680[ i ]->SetGasHeatTemperature(0, Tamb, bme680_T0[ i ]); // adjust ambient temperature 
      }
    }

    sleep( readinterval );
  }

  for(int i = 0; i < 4; i++) if( tmp102[ i ] ) delete tmp102[ i ];
  for(int i = 0; i < 4; i++) delete tmp102_file[ i ];
  delete tmp102_db;

  if( htu21d ) delete htu21d;
  delete htu21d_T_file;
  delete htu21d_RH_file;
  delete htu21d_db;

  for(int i = 0; i < 2; i++) if( bmp280[ i ] ) delete bmp280[ i ];
  for(int i = 0; i < 2; i++) 
  {
    delete bmp280_T_file[ i ];
    delete bmp280_p_file[ i ];
  }
  delete bmp280_db;

  for(int i = 0; i < 2; i++) if( bme680[ i ] ) delete bme680[ i ];
  for(int i = 0; i < 2; i++)
  {
    delete bme680_T_file[ i ];
    delete bme680_RH_file[ i ];
    delete bme680_p_file[ i ];
    delete bme680_R_file[ i ];
  }
  delete bme680_db;

  return 0;
};

