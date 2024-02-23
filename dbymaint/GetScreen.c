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

#include	"dbymaint.h"

static int DebugGetScreen = 0;
static int SawMultipleKeys;
static char *MySQL_Type;

static int EachField ( DBY_QUERY *Query )
{
#ifdef DEBUG
	int		xf;

	for ( xf = 0; xf < mysql_num_fields ( Query->Result ); xf++ )
	{
		printf ( "%s|", Query->EachRow[xf] );
	}
	printf ( "<br>\n" );
#endif
	MySQL_Type = Query->EachRow[1];
	return ( -1 );
}

static void GetFieldInfo ( char *FieldName, int *FieldType, int *FieldQuoted, int *FieldLength )
{
	char	*cp;

	*FieldType = 0;
	*FieldQuoted = 0;
	*FieldLength = 0;

	sprintf ( Statement, "show fields from %s.%s where Field = '%s'",
		DatabaseName, ScreenTable, FieldName );

	dbySelectCB ( "dbymaint", &MySql, Statement, (int(*)()) EachField, LogFileName );

	if ( DebugGetScreen )
	{
		printf ( "%s<br>\n", MySQL_Type );
	}

	if (( cp = nsStrchr ( MySQL_Type, '(' )) != (char *)0 )
	{
		cp++;
		*FieldLength = nsAtoi ( cp );
	}

	if ( nsStrncmp ( MySQL_Type, "char" , 4 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "varchar" , 7 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "binary" , 6 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "varbinary" , 9 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "blob" , 4 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "text" , 4 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "enum" , 4 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}
	else if ( nsStrncmp ( MySQL_Type, "set" , 3 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
	}

	else if ( nsStrncmp ( MySQL_Type, "integer" , 7 ) == 0 )
	{
		*FieldType = DATATYPE_LONG;
	}
	else if ( nsStrncmp ( MySQL_Type, "int" , 3 ) == 0 )
	{
		*FieldType = DATATYPE_LONG;
	}
	else if ( nsStrncmp ( MySQL_Type, "smallint" , 8 ) == 0 )
	{
		*FieldType = DATATYPE_SHORT;
	}
	else if ( nsStrncmp ( MySQL_Type, "tinyint" , 7 ) == 0 )
	{
		*FieldType = DATATYPE_SHORT;
	}
	else if ( nsStrncmp ( MySQL_Type, "mediumint" , 9 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
	}
	else if ( nsStrncmp ( MySQL_Type, "bigint" , 6 ) == 0 )
	{
		*FieldType = DATATYPE_LONG;
	}
	else if ( nsStrncmp ( MySQL_Type, "decimal" , 7 ) == 0 )
	{
		*FieldType = DATATYPE_DOUBLE;
	}
	else if ( nsStrncmp ( MySQL_Type, "numeric" , 7 ) == 0 )
	{
		*FieldType = DATATYPE_LONG;
	}
	else if ( nsStrncmp ( MySQL_Type, "float" , 5 ) == 0 )
	{
		*FieldType = DATATYPE_DOUBLE;
	}
	else if ( nsStrncmp ( MySQL_Type, "double" , 6 ) == 0 )
	{
		*FieldType = DATATYPE_DOUBLE;
	}
	else if ( nsStrncmp ( MySQL_Type, "bit" , 3 ) == 0 )
	{
		*FieldType = DATATYPE_SHORT;
	}

	else if ( nsStrncmp ( MySQL_Type, "date" , 4 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
		*FieldLength = 10;
	}
	else if ( nsStrncmp ( MySQL_Type, "datetime" , 8 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
		*FieldLength = 19;
	}
	else if ( nsStrncmp ( MySQL_Type, "timestamp" , 9 ) == 0 )
	{
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
		*FieldLength = 19;
	}
	else if ( nsStrncmp ( MySQL_Type, "time" , 4 ) == 0 )
	{
		/*---------------------------------------------------------------------------
			what changed?
		xxx	*FieldType = DATATYPE_SHORT;
		xxx	*FieldLength = 4;
		---------------------------------------------------------------------------*/
		*FieldType = DATATYPE_STRING;
		*FieldQuoted = 1;
		*FieldLength = 8;
	}
	else if ( nsStrncmp ( MySQL_Type, "year " , 4 ) == 0 )
	{
		*FieldType = DATATYPE_SHORT;
		*FieldLength = 4;
	}

	else
	{
		printf ( "Unknown datatype %s<br>\n", MySQL_Type );
	}
}

static int cmprec ( ELEMENT *a, ELEMENT *b )
{
	if ( a->Row < b->Row )
	{
		return ( -1 );
	}
	if ( a->Row > b->Row )
	{
		return ( 1 );
	}
	if ( a->Column < b->Column )
	{
		return ( -1 );
	}
	if ( a->Column > b->Column )
	{
		return ( 1 );
	}
	return ( 0 );
}

void GetScreen ( char *ScreenName )
{
	char	FileName[256];
	FILE	*fp;
	char	xbuffer[1024];
#define		MAXTOKS		10
	char	*tokens[MAXTOKS];
	int		tokcnt, lineno;

	SawMultipleKeys = ScreenRows = ScreenColumns = 0;

	sprintf ( FileName, "%s/%s/%s.scr", SCREEN_DIR, DatabaseName, ScreenName );

	if (( fp = fopen ( FileName, "r" )) == (FILE *)0 )
	{
		printf ( "Cannot open screen %s<br>\n", FileName );
		return;
	}
	
	if ( DebugGetScreen )
	{
		printf ( "opened %s okay<br>\n", FileName );
	}

	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		if (( tokcnt = GetTokensA ( xbuffer, ",\n\r\t", tokens, MAXTOKS )) < 2 )
		{
			continue;
		}

		if ( tokens[0][0] == '#' )
		{
			continue;
		}

		if ( nsStrcmp ( tokens[0], "table" ) == 0 )
		{
			snprintf ( ScreenTable, sizeof(ScreenTable), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "title" ) == 0 )
		{
			snprintf ( ScreenTitle, sizeof(ScreenTitle), "%s", tokens[1] );
		}
		else if ( nsStrcmp ( tokens[0], "mode" ) == 0 )
		{
			ScreenMode = 0;
			if ( nsStrchr ( tokens[1], 'A' ) != (char *)0 )
			{
				ScreenMode = (ScreenMode | SCREEN_MODE_INSERT );
			}
			if ( nsStrchr ( tokens[1], 'M' ) != (char *)0 )
			{
				ScreenMode = (ScreenMode | SCREEN_MODE_UPDATE );
			}
			if ( nsStrchr ( tokens[1], 'D' ) != (char *)0 )
			{
				ScreenMode = (ScreenMode | SCREEN_MODE_DELETE );
			}
		}

		else if ( nsStrcmp ( tokens[0], "label" ) == 0 )
		{
			if ( ElementCount >= MAXELEM )
			{
				printf ( "Exceeds MAXELEM in line %d.<br>\n", lineno );
				return;
			}
			ElementArray[ElementCount].Mode = FIELD_MODE_LABEL;
			snprintf ( ElementArray[ElementCount].Text, sizeof ( ElementArray[ElementCount].Text ), "%s", tokens[1] );
			ElementArray[ElementCount].Row = nsAtoi ( tokens[2] );
			ElementArray[ElementCount].Column = nsAtoi ( tokens[3] );
			if ( ScreenRows < ElementArray[ElementCount].Row )
			{
				ScreenRows = ElementArray[ElementCount].Row;
			}
			if ( ScreenColumns < ElementArray[ElementCount].Column )
			{
				ScreenColumns = ElementArray[ElementCount].Column;
			}
			ElementCount++;
		}

		else if (( nsStrcmp ( tokens[0], "field" ) == 0 ) ||
				 ( nsStrcmp ( tokens[0], "key"   ) == 0 ))
		{
			if ( ElementCount >= MAXELEM )
			{
				printf ( "Exceeds MAXELEM in line %d.<br>\n", lineno );
				return;
			}
			if ( nsStrcmp ( tokens[0], "key" ) == 0 )
			{
				ElementArray[ElementCount].Mode = FIELD_MODE_KEY;
				if ( KeyField == 0 )
				{
					KeyField = ElementCount;
				}
				else
				{
					SawMultipleKeys = 1;
				}
				if ( DebugGetScreen )
				{
					printf ( "KeyField %d ElementCount %d SawMultipleKeys %d<br>\n",
								KeyField, ElementCount, SawMultipleKeys );
				}
			}
			else
			{
				ElementArray[ElementCount].Mode = FIELD_MODE_FIELD;
			}
			snprintf ( ElementArray[ElementCount].Text, sizeof ( ElementArray[ElementCount].Text ), "%s", tokens[1] );
			ElementArray[ElementCount].Row = nsAtoi ( tokens[2] );
			ElementArray[ElementCount].Column = nsAtoi ( tokens[3] );
			if ( ScreenRows < ElementArray[ElementCount].Row )
			{
				ScreenRows = ElementArray[ElementCount].Row;
			}
			if ( ScreenColumns < ElementArray[ElementCount].Column )
			{
				ScreenColumns = ElementArray[ElementCount].Column;
			}

			GetFieldInfo ( ElementArray[ElementCount].Text, 
					&ElementArray[ElementCount].Type, &ElementArray[ElementCount].Quoted, &ElementArray[ElementCount].Length );

			if ( DebugGetScreen )
			{
				printf ( "Type %d  Quoted %d  Length %d<br>\n",
					ElementArray[ElementCount].Type, ElementArray[ElementCount].Quoted, ElementArray[ElementCount].Length );
			}
			ElementCount++;
		}

		else if ( nsStrcmp ( tokens[0], "display" ) == 0 )
		{
			if ( ElementCount >= MAXELEM )
			{
				printf ( "Exceeds MAXELEM in line %d.<br>\n", lineno );
				return;
			}
			ElementArray[ElementCount].Mode = FIELD_MODE_DISPLAY;
			snprintf ( ElementArray[ElementCount].Text, sizeof ( ElementArray[ElementCount].Text ), "%s", tokens[1] );
			ElementArray[ElementCount].Row = nsAtoi ( tokens[2] );
			ElementArray[ElementCount].Column = nsAtoi ( tokens[3] );
			if ( ScreenRows < ElementArray[ElementCount].Row )
			{
				ScreenRows = ElementArray[ElementCount].Row;
			}
			if ( ScreenColumns < ElementArray[ElementCount].Column )
			{
				ScreenColumns = ElementArray[ElementCount].Column;
			}
			ElementCount++;
		}
		else
		{
			printf ( "Unknown token [%s] in line %d<br>\n", tokens[0], lineno );
		}

	}

	nsFclose ( fp );

	if ( KeyField == 0 || SawMultipleKeys == 1 )
	{
		if (((ScreenMode & SCREEN_MODE_UPDATE ) == SCREEN_MODE_UPDATE ) ||
			((ScreenMode & SCREEN_MODE_DELETE ) == SCREEN_MODE_DELETE ))
		{
			if ( KeyField == 0 )
			{
				printf ( "Cannot update or delete without key field<br>\n" );
			}
			if ( SawMultipleKeys == 1 )
			{
				printf ( "Screen can only have one key field<br>\n" );
			}

			ScreenMode = (ScreenMode & SCREEN_MODE_INSERT);
		}
	}


	qsort ( ElementArray, ElementCount, sizeof(ELEMENT), (int(*)()) cmprec );
}
