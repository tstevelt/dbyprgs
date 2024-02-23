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

#include    "dbyload.h"

void getspec ()
{
	FILE	*fp;
	char	Delimeters[4];

	if (( fp = fopen ( Specfile, "r" )) == NULL )
	{
		printf ( "Cannot open specfile for reading\n" );
		exit ( 1 );
	}

	fgets ( Buffer, sizeof(Buffer), fp );

	nsFclose ( fp );

	sprintf ( Delimeters, "%c\n\r", Separator );

	FieldCount = GetTokensD ( Buffer, Delimeters, FieldArray, MAXFIELDS );

	if ( FieldCount <= 1 )
	{
		printf ( "Field count is %d\n", FieldCount );
		printf ( "Are your data and spec files '%c' delimited?\n", Separator );
		exit ( 1 );
	}
}
