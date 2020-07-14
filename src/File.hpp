/**************************************************************************
 * 
 * File class definitions and constructor. 
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
 * Mon Jul 13 15:04:39 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#ifndef _FILE_HPP
#define _FILE_HPP

#include <iostream>
#include <fstream>
#include <string>

/// Class for file writing and reading functions. 

/// The constructor _File_ sets directory and file name for writing data.
class File
{
    std::string Directory;       ///< directory 
    std::string FileName;     ///< file name

  public:
    /// Construct File object. 
    File();

    /// Construct File object with parameters.
    File(std::string dir, std::string fname);

    virtual ~File();

    /// Get directory.
    std::string GetDir() { return Directory; }

    /// Get file name.
    std::string GetFile() { return FileName; }

    /// Set directory.
    void SetDir(std::string dir) { this->Directory = dir; }

    /// Set file name.
    void SetFile(std::string fname) { this->FileName = fname; }

    /// Write string to file and return true if success.
    bool Write(std::string str);

    /// Write float to file and return true if success.
    bool Write(double val);

    /// Write integer to file and return true if success.
    bool Write(int val);

};

#endif
