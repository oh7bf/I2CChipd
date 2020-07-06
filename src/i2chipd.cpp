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
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "i2chipd.hpp"
#include <systemd/sd-daemon.h>
#include "signal.h"
#include <unistd.h>
#include <iostream>

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
  string name = "T1";
  string i2cdev = "/dev/i2c-1";
  int readinterval = 10;

  signal(SIGTERM, &shutdown);
  signal(SIGHUP, &reload);

  Tmp102 chip = Tmp102(name, i2cdev);

  chip.SetAddress0x48();
  cout << "-- " << chip.GetName();
  cout << " " << chip.GetDevice();
  cout << " " << chip.GetAddress() << "\n";
 
  chip.SetPointer(TMP102_TEMP_REG);

  while( cont )
  {
    cout << chip.GetName() << " = " << chip.GetTemperature();
    cout << " C, error = " << chip.GetError() << "\n";
 
    sleep( readinterval );
  }

  return 0;
};

