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
 * Edit: Tue Jul 14 20:22:43 CDT 2020
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
  string i2cdev = "/dev/i2c-1";
  string datadir = "/var/lib/i2chipd/";
  string sqlitedb = "/var/lib/i2chipd/i2chipd.db";

  int readinterval = 300;

  signal(SIGTERM, &shutdown);
  signal(SIGHUP, &reload);

  Tmp102 tmp102 = Tmp102("T1", i2cdev);
  tmp102.SetAddress0x48();

  fprintf(stderr, SD_INFO "%s %s %d\n", tmp102.GetName().c_str(), tmp102.GetDevice().c_str(), tmp102.GetAddress() );

  tmp102.SetPointer(TMP102_TEMP_REG);

//  cout << "-- " << tmp102.GetName();
//  cout << " " << tmp102.GetDevice();
//  cout << " " << tmp102.GetAddress() << "\n";

  Bme680 bme680 = Bme680("TpHG1", i2cdev);
  bme680.SetAddress0x77();

  fprintf(stderr, SD_INFO "%s %s %d\n", bme680.GetName().c_str(), bme680.GetDevice().c_str(), bme680.GetAddress() );

//  cout << "-- " << bme680.GetName();
//  cout << " " << bme680.GetDevice();
//  cout << " " << bme680.GetAddress() << "\n";

  fprintf(stderr, SD_DEBUG "read BME680 calibration data\n");
  if( !bme680.GetCalibration() )
  {
    fprintf(stderr, SD_ERR "problem reading BME680 calibration data, quit now\n");
    return -1;
  }

  fprintf(stderr, SD_INFO "1 x H, 1 x T and 16 x p oversampling\n");
  bme680.SetOverSample( 1, 2, 5);

  fprintf(stderr, SD_INFO "filter 1\n");
  bme680.SetFilter( 1 );

  fprintf(stderr, SD_INFO "profile 0: 100 ms pulse, ambient 30 C, target 300 C\n");
  bme680.SetGasWaitTime(0, 0x59); // 100 ms
  bme680.SetGasHeatTemperature(0, 30, 300); // 30 C ambient, 300 C target
  bme680.RunGas();
  bme680.HeaterOn();
  bme680.SetHeaterProfile( 0 );

  File *tmp102_x48 = new File(datadir, "tmp102_x48");
  File *bme680_x77_T = new File(datadir, "bme680_x77_T");
  File *bme680_x77_p = new File(datadir, "bme680_x77_p");
  File *bme680_x77_RH = new File(datadir, "bme680_x77_RH");
  File *bme680_x77_R = new File(datadir, "bme680_x77_R");

  int sqlite_err = 0;
  SQLite *tmp102_x48_db  = new SQLite(sqlitedb, "tmp102", "insert into tmp102 (name,temperature) values (?,?)");
//  cout << "-- SQLite file: " << tmp102_x48_db->GetFile() << "\n";
//  cout << "-- SQLite table: " << tmp102_x48_db->GetTable() << "\n";
//  cout << "-- SQLite datetime(): " << tmp102_x48_db->GetDateTime( sqlite_err ) << "\n";
//  cout << "-- SQLite error: " << sqlite_err << "\n";

  SQLite *bme680_x77_db  = new SQLite(sqlitedb, "bme680", "insert into bme680 (name,temperature,humidity,pressure,resistance,gasvalid,stable) values (?,?,?,?,?,?,?)");

  double T = 0, RH = 0, p = 0, R = 0;
  char Valid = 'N', Stable = 'N';
  double dbl_array[ 10 ];
  int int_array[ 10 ];
  while( cont )
  {
    tmp102.ReadTemperature();
    T = tmp102.GetTemperature();
    tmp102_x48->Write( T );
    dbl_array[ 0 ] = T;
    tmp102_x48_db->Insert("T1", 1, dbl_array, sqlite_err );
    if( sqlite_err != SQLITE_OK ) fprintf(stderr, SD_ERR "error writing SQLite database\n");

    fprintf(stderr, SD_INFO "%s = %f C\n", tmp102.GetName().c_str(), T);

    bme680.Forced();
    usleep( 200000 ); // 200 ms
    bme680.GetTPHG();

    T = bme680.GetTemperature();
    RH = bme680.GetHumidity();
    p = bme680.GetPressure();
    R = bme680.GetResistance();

    bme680_x77_T->Write( T );
    bme680_x77_RH->Write( RH );
    bme680_x77_p->Write( p );
    bme680_x77_R->Write( R );

    dbl_array[ 0 ] = T;
    dbl_array[ 1 ] = RH;
    dbl_array[ 2 ] = p;
    dbl_array[ 3 ] = R;
    int_array[ 0 ] = (int)bme680.GasValid();
    int_array[ 1 ] = (int)bme680.HeaterStable();

    bme680_x77_db->Insert("TpHG1", 4, dbl_array, 2, int_array, sqlite_err);

//    cout << bme680.GetName();
//    cout << " = ";
//    cout << std::setw( 5 ) << std::setprecision( 2 ) << T;
//    cout << " C, RH = ";
//    cout << std::setw( 5 ) << std::setprecision( 1 ) << RH;
//    cout << std::setprecision( 0 );
//    cout << " %, p = " << p;
//    cout << " Pa, R = " << R;
//    cout << " ohm";
    if( bme680.GasValid() ) Valid = 'Y'; else Valid = 'N';
    if( bme680.HeaterStable() ) Stable = 'Y'; else Stable = 'N';
//    cout << "\n";

    fprintf(stderr, SD_INFO "%s = %f C, %f %%, %f Pa, %f Ohm, %c, %c\n", bme680.GetName().c_str(), T, RH, p, R, Valid, Stable);

    sleep( readinterval );
  }

  delete tmp102_x48;
  delete bme680_x77_T;
  delete bme680_x77_RH;
  delete bme680_x77_p;
  delete bme680_x77_R;

  delete tmp102_x48_db;
  delete bme680_x77_db;

  return 0;
};

