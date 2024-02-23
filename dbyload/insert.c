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

void insert ()
{
	printf ( "USE %s;\n\n", Database );

	printf ( "LOAD DATA LOCAL INFILE '%s'\n", Filename );

	printf ( " INTO TABLE `%s`\n", Table );

	printf ( " FIELDS TERMINATED BY '%c'\n", Separator );

	printf ( " LINES TERMINATED BY '\\n'\n" );

	for ( int xf = 0; xf < FieldCount; xf++ )
	{
		if ( xf == 0 )
		{
			printf ( " (" );
		}
		else
		{
			printf ( " ," );
		}

		printf ( "`%s`", FieldArray[xf] );
	}
	printf ( ");\n\n" );

	printf ( "SELECT 'Inserted', ROW_COUNT();\n\n" );
}
