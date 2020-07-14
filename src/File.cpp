/**************************************************************************
 * 
 * File class member functions. 
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
 * Mon Jul 13 15:27:20 CDT 2020
 * Edit: 
 *
 * Jaakko Koivuniemi
 **/

#include "File.hpp"

using namespace std;

/// File constructor to initialize all parameters.
File::File(std::string dir, std::string fname)
{
    this->Directory = dir;
    this->FileName = fname;
}

File::~File() { };


/// File member function to open file and write string to it.
bool File::Write(std::string str)
{
  std::string ofile;
  ofile = Directory + FileName;

  std::ofstream outfile( ofile );

  if( outfile.good() )
  {
    outfile << str << "\n";
    outfile.close();
    return true;
  }
  else
  {
    return false;
  }
}

/// File member function to open file and write float to it.
bool File::Write(double val)
{
  std::string ofile;
  ofile = Directory + FileName;

  std::ofstream outfile( ofile );

  if( outfile.good() )
  {
    outfile << val << "\n";
    outfile.close();
    return true;
  }
  else
  {
    return false;
  }
}

/// File member function to open file and write integer to it.
bool File::Write(int val)
{
  std::string ofile;
  ofile = Directory + FileName;

  std::ofstream outfile( ofile );

  if( outfile.good() )
  {
    outfile << val << "\n";
    outfile.close();
    return true;
  }
  else
  {
    return false;
  }
}

