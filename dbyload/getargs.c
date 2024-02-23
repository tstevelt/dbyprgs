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

#include	"dbyload.h"

static void Usage ()
{
	printf ( "USAGE: dbyload {-n|-u} database table file specfile [options]\n" );
	printf ( "options:\n" );
	printf ( "  -s separator\n" );
	printf ( "  -l nolog (not used)\n" );
	printf ( "  -b binary (not used)\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	RunMode = 0;
	Separator = ',';

	if ( nsStrcmp ( argv[1], "-n" ) == 0 )
	{
		RunMode = MODE_INSERT;
	}
	else if ( nsStrcmp ( argv[1], "-u" ) == 0 )
	{
		RunMode = MODE_UPDATE;
	}
	else
	{
		Usage ();
	}

	if ( argc < 5 )
	{
		Usage ();
	}

	Database = argv[2];
	Table    = argv[3];
	Filename = argv[4];
	Specfile = argv[5];

	for ( xa = 6; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-s" ) == 0 )
		{
			xa++;
			Separator = argv[xa][0];
		}
		else
		{
			Usage ();
		}
	}

	if ( RunMode == 0 )
	{
		Usage ();
	}

	if ( access ( Filename, R_OK ) != 0 )
	{
		printf ( "FILENAME %s NOT FOUND\n", Filename );
		Usage ();
	}

	if ( access ( Specfile, R_OK ) != 0 )
	{
		printf ( "SPECFILE %s NOT FOUND\n", Specfile );
		Usage ();
	}
}
