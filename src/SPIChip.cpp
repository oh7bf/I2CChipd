/**************************************************************************
 * 
 * SPIChip class member functions for configuration and reading chips. 
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
 * Edit: Fri 31 Jul 2020 04:46:21 PM CDT
 *
 * Jaakko Koivuniemi
 **/


#include "SPIChip.hpp"
#include <string.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <unistd.h>

using namespace std;

/// SPIChip constructor to initialize all the parameters.

SPIChip::SPIChip(string name, string device, uint8_t mode, uint8_t bits, 
                 uint32_t clock, uint16_t delay)
{
  this->name = name;
  this->device = device;
  this->mode = mode;
  this->bits = bits;
  this->clock = clock;
  this->delay = delay;
};

SPIChip::~SPIChip() { };

/// SPIChip member function to write two bytes and read back at least two bytes.
 
/// Transfer one byte of data with serial peripheral interface to connected
/// chip register address and at same time receive 'nbytes' of data from it. 
/// First the 'reg' is transmitted followed by 'byte' and zero bytes 0x00 are
/// added to get the wanted data length.

int SPIChip::SPIWriteByteRead(uint8_t reg, uint8_t byte, uint32_t nbytes, uint8_t *readbuffer)
{
  int fd, rd;
  int cnt = 0;
  int i = 0;
  char message[ 500 ] = "";

  if( ( fd = open(device.c_str(), O_RDWR) ) < 0 )
  {
    fprintf(stderr, SD_ERR "Failed to open SPI port\n");
    return -1;
  }
  else
  {
    rd = flock(fd, LOCK_EX | LOCK_NB);

    cnt = SPILOCK_MAX;
    while( ( rd == 1 ) && ( cnt > 0 ) ) // try again if port locking failed
    {
      sleep(1);
      rd = flock(fd, LOCK_EX | LOCK_NB);
      cnt--;
    }

    if( rd )
    {
      fprintf(stderr, SD_ERR "Failed to lock SPI port\n");
      close( fd );
      return -2;
    }
    else
    {
      if( ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write mode\n");
        close( fd );
        return -3;
      }

      if( ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI read mode\n");
        close( fd );
        return -4;
      }

      if( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write bits per word\n");
        close( fd );
        return -5;
      }

      if( ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI read bits per word\n");
        close( fd );
        return -6;
      }

      if( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &clock) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write clock frequency\n");
        close( fd );
        return -7;
      }

      if( ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &clock) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI read clock frequency\n");
        close( fd );
        return -8;
      }

      writebuffer[ 0 ] = reg;
      writebuffer[ 1 ] = byte;

      struct spi_ioc_transfer tr = 
      {
         tx_buf : (uint64_t)writebuffer,
         rx_buf : (uint64_t)readbuffer,

         len : nbytes,
         speed_hz : clock,

         delay_usecs : delay,
         bits_per_word : bits,
         cs_change : 1,
         tx_nbits : 0,
         rx_nbits : 0,
         pad : 0
      };


      sprintf( message, "SPI chip register [%02X] write byte [%02X]\n", reg, byte);
      fprintf(stderr, SD_DEBUG "%s", message );
      if( ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1 )
      {
        fprintf(stderr, SD_ERR "Failed to transfer spi message\n");
        close( fd );
        return -8;
      }
      else
      {
        sprintf(message, "SPI received [");
        if( nbytes > 160 ) nbytes = 160;
	for( i = 0; i < (int)nbytes; i++ ) sprintf( message + strlen(message), "%02X ", readbuffer[ i ] );
        strcat( message, "]\n");
        fprintf(stderr, SD_DEBUG "%s\n", message);
      }

      close(fd);

    }
  }

  return 0;

};

/// SPIChip member function to write three bytes to chip.

/// Transfer one byte of data with serial peripheral interface to connected
/// chip register address and this followed by 16-bit word. 
int SPIChip::SPIWriteWord(uint8_t reg, uint16_t word)
{
  int fd, rd;
  int cnt = 0;
  int i = 0;
  char message[ 500 ] = "";

  if( ( fd = open(device.c_str(), O_RDWR) ) < 0 )
  {
    fprintf(stderr, SD_ERR "Failed to open SPI port\n");
    return -1;
  }
  else
  {
    rd = flock(fd, LOCK_EX | LOCK_NB);

    cnt = SPILOCK_MAX;
    while( ( rd == 1 ) && ( cnt > 0 ) ) // try again if port locking failed
    {
      sleep(1);
      rd = flock(fd, LOCK_EX | LOCK_NB);
      cnt--;
    }

    cnt = SPILOCK_MAX;
    while( ( rd == 1 ) && ( cnt > 0 ) ) // try again if port locking failed
    {
      sleep(1);
      rd = flock(fd, LOCK_EX | LOCK_NB);
      cnt--;
    }

    if( rd )
    {
      fprintf(stderr, SD_ERR "Failed to lock SPI port\n");
      close( fd );
      return -2;
    }
    else
    {
      if( ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write mode\n");
        close( fd );
        return -3;
      }

      if( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write bits per word\n");
        close( fd );
        return -5;
      }

      if( ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI read bits per word\n");
        close( fd );
        return -6;
      }

      if( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &clock) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI write clock frequency\n");
        close( fd );
        return -7;
      }

      if( ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &clock) < 0 )
      {
        fprintf(stderr, SD_ERR "Failed to set SPI read clock frequency\n");
        close( fd );
        return -8;
      }

      writebuffer[ 0 ] = reg;
      writebuffer[ 1 ] = (uint8_t)(word>>8);
      writebuffer[ 2 ] = (uint8_t)(0x00FF & word);

      uint8_t readbuffer[ 3 ];
      struct spi_ioc_transfer tr = 
      {
         tx_buf : (uint64_t)writebuffer,
         rx_buf : (uint64_t)readbuffer,

         len : 3,
         speed_hz : clock,

         delay_usecs : delay,
         bits_per_word : bits,
         cs_change : 1,
         tx_nbits : 0,
         rx_nbits : 0,
         pad : 0
      };

      sprintf( message, "SPI chip register [%02X] write word [%04X]\n", reg, word);
      fprintf(stderr, SD_DEBUG "%s", message );
      if( ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1 )
      {
        fprintf(stderr, SD_ERR "Failed to transfer spi message\n");
        close( fd );
        return -8;
      }
      else
      {
        sprintf(message, "SPI received [");
        for( i = 0; i < 3; i++ ) sprintf( message, "%02X ", readbuffer[ i ] );
        strcat( message, "]");
        fprintf(stderr, SD_DEBUG "%s\n", message);
      }

      close(fd);

    }
  }

  return 0;

};

