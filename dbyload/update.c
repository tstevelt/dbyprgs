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

void update ()
{
	int		xf;

	/*----------------------------------------------------------
		set database
	----------------------------------------------------------*/
	printf ( "USE %s;\n\n", Database );

	/*----------------------------------------------------------
		drop temp table
	----------------------------------------------------------*/
	printf ( "DROP TABLE IF EXISTS temp_%s;\n\n", Table );

	/*----------------------------------------------------------
		create temp table
	----------------------------------------------------------*/
	printf ( "CREATE TEMPORARY TABLE temp_%s SELECT ", Table );
	for ( xf = 0; xf < FieldCount; xf++ )
	{
		if ( xf > 0 )
		{
			printf ( ", " );
		}
		printf ( "`%s`", FieldArray[xf] );
	}
	printf ( " FROM %s LIMIT 0;\n\n", Table );

	/*----------------------------------------------------------
		load temp table
	----------------------------------------------------------*/
	printf ( "LOAD DATA LOCAL INFILE '%s'\n", Filename );
	printf ( " INTO TABLE temp_%s FIELDS TERMINATED BY '%c' (", Table, Separator );
	for ( xf = 0; xf < FieldCount; xf++ )
	{
		if ( xf > 0 )
		{
			printf ( ", " );
		}
		printf ( "`%s`", FieldArray[xf] );
	}
	printf ( ");\n\n" );

	/*----------------------------------------------------------
		update target table from temp table
	----------------------------------------------------------*/
	printf ( "UPDATE %s\n", Table );

	printf ( " INNER JOIN temp_%s on temp_%s.%s = %s.%s\n", 
		Table, Table, FieldArray[0], Table, FieldArray[0] );

	for ( xf = 1; xf < FieldCount; xf++ )
	{
		if ( xf == 1 )
		{
			printf ( " SET" );
		}
		else
		{
			printf ( ",\n" );
		}
		printf ( " %s.%s = temp_%s.%s",
			Table, FieldArray[xf], Table, FieldArray[xf] );
		
	}
	printf ( ";\n\n" );

	printf ( "SELECT 'Updated', ROW_COUNT();\n\n" );

	/*----------------------------------------------------------
		drop temp table
	----------------------------------------------------------*/
	printf ( "DROP TEMPORARY TABLE temp_%s;\n\n", Table );
}
