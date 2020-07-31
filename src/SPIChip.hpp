/**************************************************************************
 * 
 * SPIChip class definitions and constructor. 
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
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _SPICHIP_HPP
#define _SPICHIP_HPP 

#include <linux/spi/spidev.h>
#include <systemd/sd-daemon.h>
#include <string>

#define SPILOCK_MAX 10        ///< Maximum number of times SPI device file locking is attempted. 
#define WRITEBUFFER_MAX 1024  ///< Maximum size for SPI write buffer.

/// Class for chips with Serial Peripheral Interface (SPI).

/// The constructor _SPIChip()_ takes name tag, device file for reading
/// and writing serial data, SPI mode defined in `spidev.h` header file,
/// number of bits for word (usually 8), clock frequency used in serial
/// transfer and optional delay before deselecting (Chip Select)
/// before next transfer. 

class SPIChip
{
    std::string name;   ///< name tag for chip
    std::string device; ///< device file to read and write serial data
    uint8_t mode;       ///< SPI mode
    uint8_t bits;       ///< bits per word
    uint32_t clock;     ///< clock rate [Hz]
    uint16_t delay;     ///< delay before deselecting (optional) before next transfer

    /// buffer to transfer serial data to chip
    uint8_t writebuffer[ WRITEBUFFER_MAX ] = { };

  public:
    /// Construct SPIChip object.
    SPIChip();

    /// Construct SPIChip object with parameters needed to talk to chip.
    SPIChip(std::string name, std::string device, uint8_t mode, uint8_t bits, 
            uint32_t clock, uint16_t delay);

    virtual ~SPIChip();

    /// Get SPI chip name tag.
    std::string GetName() { return name; }

    /// Get SPI chip device file name.
    std::string GetDevice() { return device; }

    /// Get serial mode byte.
    uint8_t GetMode() { return mode; }

    /// Get number of bits for word used in serial data transfer.
    uint8_t GetBits() { return bits; }

    /// Get clock frequency used in serial transfer. 
    uint32_t GetClock() { return clock; }

    /// Get optional delay before deselecting before next transfer.
    uint16_t GetDelay() { return delay; }

    /// Set SPI chip name tag.
    void SetName(std::string name) { this->name = name; }

    /// Set SPI chip device file name.
    void SetDevice(std::string device) { this->device = device; }

    /// Set serial mode byte.
    void SetMode(uint8_t mode) { this->mode = mode; }

    /// Set number of bits for word used in serial data transfer.
    void SetBits(uint8_t bits) { this->bits = bits; }

    /// Set clock frequency used in serial transfer. 
    void SetClock(uint32_t clock) { this->clock = clock; }

    /// Get optional delay before deselecting before next transfer.
    void SetDelay(uint16_t delay) { this->delay = delay; }

    /// Write one byte of data to register and read data back from chip.

    /// The serial transfer starts with byte _reg_ followed by _byte_.
    /// The _nbyte_ needs to be at least 2 for valid transfer. At the
    /// same time the _readbuffer_ is filled with data clocked out
    /// from chip. The first byte does not usually have any useful information
    /// since it was received when the _reg_ was clocked into the chip.
    /// In chip read mode the second byte and bytes after that have the
    /// data addressed by _reg_ byte.
    int SPIWriteByteRead(uint8_t reg, uint8_t byte, uint32_t nbytes, uint8_t *readbuffer);

    /// Write 16-bit word of data to register.

    /// The serial transfer starts with byte _reg_ followed by 16-bit _word_.
    int SPIWriteWord(uint8_t reg, uint16_t word);

};

#endif
