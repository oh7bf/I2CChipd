/**************************************************************************
 * 
 * SQLite class member functions. 
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
 * Tue Jul 14 13:30:25 CDT 2020
 * Edit: Wed Jul 15 10:34:02 CDT 2020
 *
 * Jaakko Koivuniemi
 **/

#include "SQLite.hpp"

using namespace std;

/// SQLite constructor to initialize all parameters.
SQLite::SQLite(std::string file, std::string table, std::string insert_stmt)
{
  this->file = file;
  this->table = table;
  this->insert_stmt = insert_stmt;
}

SQLite::~SQLite() { };

/// SQLite member function to open database connection and execute _select datetime()_.
std::string SQLite::GetDateTime(int & error)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;
  const char query[ 200 ] = "select datetime()";

  char message[ 500 ] = "";
  char dtime[ 500 ] = "";

  int rc = sqlite3_open_v2(file.c_str(), &db, SQLITE_OPEN_READONLY, NULL);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Can not open database: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return "";
  }

  rc = sqlite3_prepare_v2(db, query, 200, &stmt, 0);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Statement prepare failed: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return "";
  }
  else
  {
    if( stmt != NULL )
    {
      rc = sqlite3_step( stmt );
      if( rc != SQLITE_ROW )
      {
        sprintf(message, "Statement failed: %s\n", sqlite3_errmsg( db ) );
        fprintf(stderr, SD_ERR "%s", message);
        error = rc;

        rc = sqlite3_finalize( stmt );
        sqlite3_close( db );

	return "";
      }
      else
      {
        sprintf(dtime, "%s\n", sqlite3_column_text(stmt, 0) );
        fprintf(stderr, SD_DEBUG "SQLite: %s", dtime);
        error = SQLITE_OK;
      }
    }
  }

  rc = sqlite3_finalize( stmt );
  sqlite3_close( db );

  return dtime;
}

/// SQLite member function to open connection and execute insert statement on database table.
bool SQLite::Insert(std::string name, int N, double *data, int & error)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;

  char message[ 500 ] = "";

  int rc = sqlite3_open_v2(file.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Can not open database: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return false;
  }

  int i, j;

  rc = sqlite3_prepare_v2(db, insert_stmt.c_str(), 200, &stmt, 0);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Statement prepare failed: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return false;
  }
  else
  {
    rc = sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), SQLITE_STATIC);

    if( rc != SQLITE_OK )
    {
      sprintf(message, "Binding failed: %s\n", sqlite3_errmsg( db ) );
      fprintf(stderr, SD_ERR "%s", message);
      error = rc;

      rc = sqlite3_finalize( stmt );
      sqlite3_close( db );
      
      return false;
    }
    else
    {
      for( i = 1; i <= N; i++)
      {
        rc = sqlite3_bind_double(stmt, i + 1, data[ i - 1 ]);

    	if( rc != SQLITE_OK )
        {
          sprintf(message, "Binding failed: %s\n", sqlite3_errmsg( db ) );
          fprintf(stderr, SD_ERR "%s", message);
          error = rc;

          rc = sqlite3_finalize( stmt );
          sqlite3_close( db );
        } 
      }
    }
  }     

  rc = sqlite3_step( stmt );

  if( rc == SQLITE_BUSY )
  {
    j = 0;
    while( rc == SQLITE_BUSY && j < 10 )
    {
      fprintf(stderr, SD_WARNING "SQLite database busy, wait 1 s.\n");
      sleep( 1 ); // sleep 1 s
      rc = sqlite3_step( stmt );
    }
  }

  if( rc != SQLITE_DONE )
  {
    sprintf(message, "Statement failed: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;

    rc = sqlite3_finalize( stmt );
    sqlite3_close( db );

    return false;
  } 

  rc = sqlite3_finalize( stmt );
  sqlite3_close( db );

  return true;
}

/// SQLite member function to open connection and execute insert statement on database table.
bool SQLite::Insert(std::string name, int Nd, double *dbl_array, int Ni, int *int_array, int & error)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;

  char message[ 500 ] = "";

  int rc = sqlite3_open_v2(file.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Can not open database: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return false;
  }

  int i, j;

  rc = sqlite3_prepare_v2(db, insert_stmt.c_str(), 200, &stmt, 0);

  if( rc != SQLITE_OK )
  {
    sprintf(message, "Statement prepare failed: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;
    sqlite3_close( db );

    return false;
  }
  else
  {
    rc = sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), SQLITE_STATIC);

    if( rc != SQLITE_OK )
    {
      sprintf(message, "Binding failed: %s\n", sqlite3_errmsg( db ) );
      fprintf(stderr, SD_ERR "%s", message);
      error = rc;

      rc = sqlite3_finalize( stmt );
      sqlite3_close( db );
      
      return false;
    }
    else
    {
      for( i = 1; i <= Nd; i++)
      {
        rc = sqlite3_bind_double(stmt, i + 1, dbl_array[ i - 1 ]);

    	if( rc != SQLITE_OK )
        {
          sprintf(message, "Binding failed: %s\n", sqlite3_errmsg( db ) );
          fprintf(stderr, SD_ERR "%s", message);
          error = rc;

          rc = sqlite3_finalize( stmt );
	  sqlite3_close( db );
        } 
      }

      for( i = 0; i < Ni; i++)
      {
        rc = sqlite3_bind_int(stmt, i + Nd + 2, int_array[ i ]);

    	if( rc != SQLITE_OK )
        {
          sprintf(message, "Binding failed: %s\n", sqlite3_errmsg( db ) );
          fprintf(stderr, SD_ERR "%s", message);
          error = rc;

          rc = sqlite3_finalize( stmt );
	  sqlite3_close( db );
        } 
      }
    }
  }     
  
  rc = sqlite3_step( stmt );

  if( rc == SQLITE_BUSY )
  {
    j = 0;
    while( rc == SQLITE_BUSY && j < 10 )
    {
      fprintf(stderr, SD_WARNING "SQLite database busy, wait 1 s.\n");
      sleep( 1 ); // sleep 1 s
      rc = sqlite3_step( stmt );
    }
  }

  if( rc != SQLITE_DONE )
  {
    sprintf(message, "Statement failed: %s\n", sqlite3_errmsg( db ) );
    fprintf(stderr, SD_ERR "%s", message);
    error = rc;

    rc = sqlite3_finalize( stmt );
    sqlite3_close( db );

    return false;
  } 

  rc = sqlite3_finalize( stmt );
  sqlite3_close( db );

  return true;
}

