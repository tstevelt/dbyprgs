/*----------------------------------------------------------------------------
	Program : dbymaint.c
	Author  : Tom Stevelt
	Date    : 03/11/2019
	Synopsis: Query by forms for mysql/mariadb.  
				Use dbymkscr to make default screen forms.
	Return  : 

	Todo
	---------------------------------------------------------------------
	Delete and Update from a set go back to clear and lose set.
	Better styling

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		10/26/2019	Added wildcard support.

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
#include	"dbymaint.h"

static int DebugMain = 0;

int main ( int argc, char *argv[] )
{
	PaintTop ();

	if ( DebugMain )
	{
		printf ( "before getinput<br>\n" );
	}

	GetInput ( 1 );
	if ( DebugMain )
	{
		printf ( "after get input 1<br>\n" );
	}

	if ( nsStrlen ( SessionID ) == 0 )
	{
		int		xi, xl;

		seed_random_with_usec ();
		for ( xi = 0; xi < 20; xi++ )
		{
			xl = random_range ( 65, 90 );
			SessionID[xi] = (char) xl;
		}
	}

	if ( DebugMain )
	{
		printf ( "session %20.20s<br>\n", SessionID );
	}

	Connect ( &MySql, DatabaseName );
	if ( DebugMain )
	{
		printf ( "after connect %s<br>\n", DatabaseName );
	}

	GetScreen ( ScreenName );
	if ( DebugMain )
	{
		printf ( "after load screen<br>\n" );
	}

	GetInput ( 2 );
	if ( DebugMain )
	{
		printf ( "after get input 2, run mode %d<br>\n", RunMode );
	}

	TakeAction ();
	if ( DebugMain )
	{
		printf ( "after take action<br>\n" );
	}

	PaintScreen ();
	if ( DebugMain )
	{
		printf ( "done<br>\n" );
	}

	return ( 0 );
}

