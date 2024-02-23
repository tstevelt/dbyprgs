/*----------------------------------------------------------------------------
	Program : dbymkscr.c
	Author  : Tom Stevelt
	Date    : 04/02/2019
	Synopsis: Make screens for dbymaint.cgi
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
#include	"dbymkscr.h"

static	int		Row, Column;

static int EachField ( DBY_QUERY *Query )
{
	fprintf ( fpScreen, "label,%s,%d,%d\n", Query->EachRow[0], Row, Column );
	Column++;
	if ( nsStrcmp ( Query->EachRow[3], "PRI" ) == 0 )
	{
		fprintf ( fpScreen, "key,%s,%d,%d\n", Query->EachRow[0], Row, Column );
	}
	else
	{
		fprintf ( fpScreen, "field,%s,%d,%d\n", Query->EachRow[0], Row, Column );
	}
	Row++;
	Column = 1;
	fprintf ( fpScreen, "\n" );

	return ( 0 );
}

static int EachTable ( DBY_QUERY *Query )
{
	if ( OneTable == 1 && nsStrcmp ( Query->EachRow[0], TableName ) != 0 )
	{
		return ( 0 );
	}

	sprintf ( ScreenName, "%s/%s/%s.scr", SCREEN_DIR, DatabaseName, Query->EachRow[0] );

	if  ( access ( ScreenName, F_OK ) == 0 )
	{
		printf ( "%s exists, skipping\n", ScreenName );
		return ( 0 );
	}

	if (( fpScreen = fopen ( ScreenName, "w" )) == (FILE *)0 )
	{
		printf ( "Cannot create %s for output\n", ScreenName );
		return ( 0 );
	}

	printf ( "Making %s\n", ScreenName );

	Row = Column = 1;

	fprintf ( fpScreen, "\n" );
	fprintf ( fpScreen, "table,%s\n", Query->EachRow[0] );
	fprintf ( fpScreen, "title,%s on file\n", Query->EachRow[0] );
	fprintf ( fpScreen, "mode,AMD\n" );
	fprintf ( fpScreen, "\n" );

	sprintf ( Statement, "desc %s", Query->EachRow[0] );
	dbySelectCB ( "dbymkscr", &MySql, Statement, (int(*)()) EachField, LogFileName );

	nsFclose ( fpScreen );

	return ( 0 );
}

int main ( int argc, char *argv[] )
{
	getargs ( argc, argv );

	dbyConnect ( &MySql, DatabaseName, "tms", 0, stdout );

	sprintf ( Statement, "show tables" );
	dbySelectCB ( "dbymkscr", &MySql, Statement, (int(*)()) EachTable, LogFileName );

	return ( 0 );
}
