/*----------------------------------------------------------------------------
	Program : dbyfix.c
	Author  : Tom Stevelt
	Date    : 09/10/2022
	Synopsis: Remove leading and trailing spaces, as well as single quotes,
				from given string field.
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
    Utility programs for working with mysql/mariadb

    Copyright (C)  2019 - 2024 Tom Stevelt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
---------------------------------------------------------------------------*/

#define		MAIN
#include	"dbyfix.h"

#define		MAXBUFFER		10240

static		long		Recno = 0;
static		long		Fixed = 0;
static		long		Errors = 0;

static int EachTable ( DBY_QUERY *Query )
{
	char	Buffer[MAXBUFFER];
	int		Length;

	Recno++;

	// printf ( "<%s>\n", Query->EachRow[1] );

	Length = nsStrlen ( Query->EachRow[1] );
	if ( Length == 0 )
	{
		printf ( "RECORD %ld, KEY %s, ZERO LENGTH\n", Recno, Query->EachRow[0] );
		return ( 0 );
	}
	if ( Length >= MAXBUFFER )
	{
		printf ( "RECORD %ld, EXCEEDS MAXBUFFER\n", Recno );
		return ( 0 );
	}

	snprintf ( Buffer, sizeof(Buffer), "%s", Query->EachRow[1] );

	TrimRightAndLeft ( Buffer );
	Stripper ( Buffer, "'" );

	if ( nsStrcmp ( Query->EachRow[1], Buffer ) != 0 )
	{
		printf ( "RECORD %ld, TRIMMED\n", Recno );
		printf ( "%s\n", Query->EachRow[1] );
		printf ( "%s\n", Buffer );

		snprintf ( UpdateStatement, sizeof(UpdateStatement), 
			"update %s set %s = '%s' where %s = '%s'",
				TableName, 
				FixFieldName, Buffer,
				KeyFieldName, Query->EachRow[0] );

		printf ( "%s\n", UpdateStatement );

// long dbyUpdate ( char *Module, MYSQL *MySql, char *Statement, int ErrorHandling, char *LogFileName )

		if ( dbyUpdate ( "dbyfix", &MySql, UpdateStatement, 0, LogFileName ) != 1 )
		{
			Errors++;
		}
		else
		{
			Fixed++;
		}
	}

	return ( 0 );
}

int main ( int argc, char *argv[] )
{
	getargs ( argc, argv );

	dbyConnect ( &MySql, DatabaseName, "tms", 0, stdout );

	sprintf ( SelectStatement, "select %s, %s from %s", KeyFieldName, FixFieldName, TableName );
	dbySelectCB ( "dbyfix", &MySql, SelectStatement, (int(*)()) EachTable, LogFileName );

	printf ( "Examined %ld, Fixed %ld, Errors %ld\n", Recno, Fixed, Errors );
	return ( 0 );
}
