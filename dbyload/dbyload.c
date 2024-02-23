/*----------------------------------------------------------------------------
	Program : dbyload.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Apr 2023
	Synopsis: Insert or Update mysql table from delimited file.
			  Modeled after Unify DBLOAD program:
			  usage: DBLOAD [ -n | -u ] [ -l ] [ -s<separator> ] [ -b ]
				  <database> <table> <file> <specfile> [ <comment> ]
	Return  : 0

	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>

	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.

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
#include	"dbyload.h"

int main ( int argc, char *argv[] )
{
	getargs ( argc, argv );

	getspec ();

	switch ( RunMode )
	{
		case MODE_INSERT:
			insert ();
			break;

		case MODE_UPDATE:
			update ();
			break;
	}

	return ( 0 );
}
