//     Utility programs for working with mysql/mariadb
// 
//     Copyright (C)  2018 - 2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include	"dbymkfile.h"

static void Usage ()
{
	printf ( "USAGE: dbymkfile dumpfile [-dates {S|D}] [-fields]\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	if ( argc < 2 )
	{
		Usage ();
	}

	DatesAsStrings = 0;
	FieldDefines = 0;

	ifn = argv[1];

	for ( xa = 2; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && nsStrcmp ( argv[xa], "-dates" ) == 0 )
		{
			xa++;
			switch ( argv[xa][0] )
			{
				case 's':
				case 'S':
					DatesAsStrings = 1;
					break;
				case 'd':
				case 'D':
					DatesAsStrings = 0;
					break;
				default:
					Usage ();
					break;
			}
		}
		else if ( nsStrcmp ( argv[xa], "-fields" ) == 0 )
		{
			FieldDefines = 1;
		}
		else
		{
			Usage ();
		}
	}
}
