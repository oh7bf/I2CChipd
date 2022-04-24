/**************************************************************************
 * 
 * SQLite database class definitions and constructor. 
 *       
 * Copyright (C) 2020 - 2022 Jaakko Koivuniemi.
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
 * Tue Jul 14 10:58:25 CDT 2020
 * Edit: Sun Apr 24 15:15:44 CDT 2022
 *
 * Jaakko Koivuniemi
 **/

#ifndef _SQLITE_HPP
#define _SQLITE_HPP

#include <systemd/sd-daemon.h>
#include <string>
#include <sqlite3.h>
#include <unistd.h>

/// Class for SQLite database functions. 

/// The constructor _SQLite_ sets SQLite database file name, table and insert 
/// query for data storage.
class SQLite 
{
    std::string file;         ///< SQLite database file name
    std::string table;        ///< SQLite database table
    std::string insert_stmt;  ///< SQLite insert statement 

   public:
    /// Construct Database object. 
    SQLite();

    /// Construct Database object with parameters.
    SQLite(std::string file, std::string table, std::string ins_stmt);

    virtual ~SQLite();

    /// Get database file name.
    std::string GetFile() { return file; }

    /// Get database table name.
    std::string GetTable() { return table; }

    /// Get insert query.
    std::string GetInsert() { return insert_stmt; }

    /// Get return string from datetime() query.
    std::string GetDateTime(int & error);

    /// Set database file name.
    void SetFile(std::string file) { this->file = file; }

    /// Set database table name.
    void SetTable(std::string table) { this->table = table; }

    /// Set database insert query.
    void SetInsert(std::string insert_stmt) { this->insert_stmt = insert_stmt; }

    /// Insert name and N doubles to database table and return true in success.
    bool Insert(std::string name, int N, double *data, int & error);

    /// Insert name, Nd doubles and Ni integers to database table and return true in success.
    bool Insert(std::string name, int Nd, double *dbl_array, int Ni, int *int_array, int & error);

};

#endif
