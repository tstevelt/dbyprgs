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

#include	"dbyfix.h"

static void Usage ()
{
	printf ( "USAGE: dbyfix db table keyfield fixfield\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	if ( argc < 4 )
	{
		Usage ();
	}

	OneTable = 1;

	sprintf ( DatabaseName, "%s", argv[1] );

	sprintf ( TableName, "%s", argv[2] );

	sprintf ( KeyFieldName, "%s", argv[3] );

	sprintf ( FixFieldName, "%s", argv[4] );

}
