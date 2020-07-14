/**************************************************************************
 * 
 * I2Chip class member functions for configuration and reading chips. 
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
 * Fri Jul  3 15:57:37 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/


#include "I2Chip.hpp"
#include <string.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

/// I2Chip constructor to initialize all parameters.

I2Chip::I2Chip(std::string name, std::string i2cdev, uint16_t address)
{
  this->name = name;
  this->i2cdev = i2cdev;
  this->address = address;
};

I2Chip::~I2Chip() { };

/// I2Chip member function to read one byte from given address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. One byte is transfered from the I2C chip. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
uint8_t I2Chip::I2cReadUInt8(uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  uint8_t rdata = 0;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return 0;
  }

  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

  if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to lock I2C port. %s", message);
    close( fd );
    error = -2;
    return 0;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return 0;
  }

  sprintf(message, "I2C[%02X] read", address);
  fprintf(stderr, SD_DEBUG "%s", message);

  if( read(fd, buffer, 1) != 1 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to read from slave. %s", message);
    close( fd );
    error = -4;
    return 0;
  }
  else
  {
    rdata = buffer[ 0 ];
    sprintf(message, "I2C received [%02X] (%d)", buffer[0], rdata);
    fprintf(stderr, SD_DEBUG "%s", message);
  }

  close( fd );
  error = 0;

  return rdata;
}

/// I2Chip member function to read two bytes from given address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. Two bytes are transfered from the I2C chip. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
uint16_t I2Chip::I2cReadUInt16(uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  uint16_t rdata = 0;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return 0;
  }

  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

  if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to lock I2C port. %s", message);
    close( fd );
    error = -2;
    return 0;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return 0;
  }

  sprintf(message, "I2C[%02X] read", address);
  fprintf(stderr, SD_DEBUG "%s", message);

  if( read(fd, buffer, 2) != 2 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to read from slave. %s", message);
    close( fd );
    error = -4;
    return 0;
  }
  else
  {
    rdata = 256*buffer[ 0 ] + buffer[ 1 ];
    sprintf(message, "I2C received [%02X %02X] (%d)", buffer[0], buffer[1], rdata);
    fprintf(stderr, SD_DEBUG "%s", message);
  }

  close( fd );
  error = 0;

  return rdata;
}

/// I2Chip member function to read four bytes from given address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. Four bytes are transfered from the I2C chip. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
uint32_t I2Chip::I2cReadUInt32(uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  uint32_t rdata = 0;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return 0;
  }
      
  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

  if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to lock I2C port. %s", message);
    close( fd );
    error = -2;
    return 0;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return 0;
  }

  sprintf(message, "I2C[%02X] read", address);
  fprintf(stderr, SD_DEBUG "%s", message);

  if( read(fd, buffer, 4) != 4 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to read from slave. %s", message);
    close( fd );
    error = -4;
    return 0;
  }
  else
  {
    rdata = 16777216 * buffer[ 0 ] + 65536 * buffer[ 1 ] + 256 * buffer[ 2 ] + buffer[ 3 ];
    sprintf(message, "I2C received [%02X %02X %02X %02X] (%d)", buffer[0], buffer[1], buffer[2], buffer[3], rdata);
    fprintf(stderr, SD_DEBUG "%s", message);
  }

  close( fd );
  error = 0;

  return rdata;
}

// Read N bytes from I2C address and return values in buffer.

// The chip I2C address and pointer to buffer are given as parameters.
// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
// -3 unable to get bus access to talk to slave, -4 unable to read
// from slave and -5 more or less data received than expected.
void I2Chip::I2cReadBytes(int Nbytes, uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  int cnt = 0;
  char message[ 500 ] = "";

  if( Nbytes > BUFFER_MAX )
  { 
    sprintf(message, "%d is more than I2C read buffer size.", Nbytes);
    fprintf(stderr, SD_ERR "%s", message);
    error = -6;
    return;
  }

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return;
  }
      
  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

  if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to lock I2C port. %s", message);
    close( fd );
    error = -2;
    return;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return;
  }

  sprintf(message, "I2C[%02X] read", address);
  fprintf(stderr, SD_DEBUG "%s", message);

  if( read(fd, buffer, Nbytes) != Nbytes )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to read from slave. %s", message);
    close( fd );
    error = -4;
    return;
  }
  else
  {
    sprintf(message, "I2C received [");
    if( Nbytes > 160 ) Nbytes = 160;
    for(int i = 0; i < Nbytes; i++ ) sprintf(message + strlen(message), "%02X ", buffer[ i ]);
    sprintf(message + strlen(message), "]");
    fprintf(stderr, SD_DEBUG "%s", message);
  }

  close( fd );
  error = 0;

  return;
}

/// I2Chip member function to write one byte to given address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. One byte is transfered to the I2C chip. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
void I2Chip::I2cWriteUInt8(uint8_t data, uint16_t address, uint8_t *buffer, int & error){
  int fd, rd;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return;
  }

  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

 if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open i2c port. %s", message);
    close( fd );
    return;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return;
  }

  buffer[ 0 ] = data; 
  sprintf(message, "I2C[%02X] write byte [%02X]", address, buffer[ 0 ] );
  fprintf(stderr, SD_DEBUG "%s", message);

  if( write(fd, buffer, 1) != 1 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Error writing to I2C slave. %s", message);
    close( fd );
    error = -4;
    return;
  }

  close( fd );
  error = 0;

  return;
}

/// I2Chip member function to write one byte to given register address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. One register pointer byte is transfered to the I2C chip
/// followed by one data byte. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
void I2Chip::I2cWriteRegisterUInt8(uint8_t reg, uint8_t data, uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return;
  }

  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

 if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open i2c port. %s", message);
    close( fd );
    return;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return;
  }

  buffer[ 0 ] = reg; 
  buffer[ 1 ] = data;

  sprintf(message, "I2C[%02X] write byte [%02X] to register [%02X]", address, buffer[ 1 ], buffer[ 0 ] );
  fprintf(stderr, SD_DEBUG "%s", message);

  if( write(fd, buffer, 3) != 3 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Error writing to I2C slave. %s", message);
    close( fd );
    error = -4;
    return;
  }

  close( fd );
  error = 0;

  return;
}

/// I2Chip member function to write two bytes to given register address.

/// After opening the device file the port is locked to avoid any other
/// process using the interface at the same time. If the locking fails it
/// is tried again maximum I2LOCK_MAX times while sleeping one second between
/// the attempts. One register pointer byte is transfered to the I2C chip
/// followed by two data bytes. 
/// https://www.kernel.org/doc/Documentation/i2c/dev-interface
void I2Chip::I2cWriteRegisterUInt16(uint8_t reg, uint16_t data, uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  int cnt = 0;
  char message[ 500 ] = "";

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return;
  }

  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

 if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open i2c port. %s", message);
    close( fd );
    return;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return;
  }

  buffer[ 0 ] = reg; 
  buffer[ 1 ] = (uint8_t)(( data & 0xFF00 ) >> 8); 
  buffer[ 2 ] = (uint8_t)( data & 0x00FF ); 

  sprintf(message, "I2C[%02X] write bytes [%02X %02X] to register [%02X]", address, buffer[ 1 ], buffer[ 2 ], buffer[ 0 ] );
  fprintf(stderr, SD_DEBUG "%s", message);

  if( write(fd, buffer, 3) != 3 ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Error writing to I2C slave. %s", message);
    close( fd );
    error = -4;
    return;
  }

  close( fd );
  error = 0;

  return;
}

// Write N bytes from buffer to I2C address.

// The chip I2C address and pointer to buffer are given as parameters.
// Error codes: -1 failed to open I2C port, -2 failed to lock I2C port,
// -3 unable to get bus access to talk to slave, -4 unable to read
// from slave and -5 more or less data received than expected.
void I2Chip::I2cWriteBytes(int Nbytes, uint16_t address, uint8_t *buffer, int & error)
{
  int fd, rd;
  int cnt = 0;
  char message[ 500 ] = "";

  if( Nbytes > BUFFER_MAX )
  { 
    sprintf(message, "%d is more than I2C write buffer size.", Nbytes);
    fprintf(stderr, SD_ERR "%s", message);
    error = -6;
    return;
  }

  if( ( fd = open(i2cdev.c_str(), O_RDWR) ) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to open I2C port. %s", message);
    error = -1;
    return;
  }
      
  rd = flock(fd, LOCK_EX|LOCK_NB);

  // try again if port locking failed
  cnt = I2LOCK_MAX;
  while( rd == 1 && cnt > 0 )
  {
    sleep( 1 );
    rd = flock(fd, LOCK_EX|LOCK_NB);
    cnt--;
  }

  if( rd )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Failed to lock I2C port. %s", message);
    close( fd );
    error = -2;
    return;
  }

  if( ioctl(fd, I2C_SLAVE, address) < 0 )
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Unable to get bus access to talk to slave. %s", message);
    close( fd );
    error = -3;
    return;
  }

  sprintf(message, "I2C[%02X] write", address);
  fprintf(stderr, SD_DEBUG "%s", message);

  if( write(fd, buffer, Nbytes) != Nbytes ) 
  {
    strncpy(message, strerror( errno ), 400);
    fprintf(stderr, SD_ERR "Error writing to I2C slave. %s", message);
    close( fd );
    error = -4;
    return;
  }

  close( fd );
  error = 0;

  return;
}

