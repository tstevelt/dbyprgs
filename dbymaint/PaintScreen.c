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

void PaintScreen ()
{
	int		xe, xr, lineno;
	int		ScreenLevel;
	char	xbuffer[10240];
	char 	*tokens[MAXELEM];
	int		/* tokcnt, xt, */ xf;
	int		MinSize, MaxSize;

	switch ( RunMode )
	{
		case MODE_UNKNOWN:
		case MODE_CLEAR:
			ScreenLevel = 1;
			break;
		case MODE_FIND:
		case MODE_INSERT:
		case MODE_UPDATE:
		case MODE_DELETE:
		case MODE_FIRST:
		case MODE_PREVIOUS:
		case MODE_NEXT:
		case MODE_LAST:
			ScreenLevel = 2;
			sprintf ( DataFileName, "%s/%s/%s.data", SCREEN_DIR, DatabaseName, SessionID );

			if (( fpData = fopen ( DataFileName, "r" )) == (FILE *)0 )
			{
				printf ( "Cannot open %s for input<br>\n", DataFileName );
				break;
			}
			lineno = 0;
			while ( fgets ( xbuffer, sizeof(xbuffer), fpData ) != (char *)0 )
			{
				lineno++;
				if ( lineno == CurrentRow )
				{
					/* tokcnt = */ GetTokensA ( xbuffer, "|\n", tokens, MAXELEM );
					break;
				}
			}

			nsFclose ( fpData );
			break;
	}

// printf ( "RunMode %d, ScreenLevel %d<br>\n", RunMode, ScreenLevel  );

	/*----------------------------------------------------------
		Got these from the URL
	----------------------------------------------------------*/
	printf ( "<input type='hidden' name='DB'      value='%s'>\n", DatabaseName );
	printf ( "<input type='hidden' name='screen'  value='%s'>\n", ScreenName );
	printf ( "<input type='hidden' name='session' value='%20.20s'>\n", SessionID );
	printf ( "<input type='hidden' name='FieldCount' value='%d'>\n", FieldCount );
	printf ( "<input type='hidden' name='RowCount' value='%d'>\n", RowCount );
	printf ( "<input type='hidden' name='CurrentRow' value='%d'>\n", CurrentRow );

	/*----------------------------------------------------------
		Title
	----------------------------------------------------------*/
	printf ( "<table class='maint'>\n" );
	printf ( "<tr>\n" );
	printf ( "<th class='maint' colspan='%d' align='center'>\n", ScreenColumns );
	printf ( "%s</th>\n", ScreenTitle );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		Fields
	----------------------------------------------------------*/
	for ( xe = 0, xr = -1, xf = 0; xe < ElementCount; xe++ )
	{
		if ( xr < ElementArray[xe].Row )
		{
			if ( xr > -1 )
			{
				printf ( "</tr>\n" );
			}
			printf ( "<tr>\n" );
			xr = ElementArray[xe].Row;
		}
		printf ( "<td>\n" );
		switch ( ElementArray[xe].Mode )
		{
			case FIELD_MODE_LABEL:
				printf ( "%s", ElementArray[xe].Text );
				break;

			case FIELD_MODE_KEY:
			case FIELD_MODE_FIELD:
				if ( ElementArray[xe].Length == 0 )
				{
					switch ( ElementArray[xe].Type )
					{
						case DATATYPE_CHAR:
							MinSize = MaxSize = 2;
							break;
						case DATATYPE_STRING:
						case DATATYPE_DOUBLE:
							if ( ScreenLevel == 2 )
							{
								MinSize = MaxSize = nsStrlen ( tokens[xf] );
							}
							else
							{
								// arbitrary
								MinSize = MaxSize = 20;
							}
							break;
						case DATATYPE_SHORT:
							MinSize = MaxSize = 6;
							break;
						case DATATYPE_LONG:
							MinSize = MaxSize = 11;
							break;
						case DATATYPE_MYSQL_DATE:
						case DATATYPE_SYSTEM_DATE:
							MinSize = MaxSize = 11;
							break;
					}
				}
				else
				{
					MinSize = MaxSize = ElementArray[xe].Length;
				}

				if ( MinSize > 40 )
				{
					MinSize = 40;
				} 
				else if ( MinSize < 20 )
				{
					MinSize += 3;
				}

				printf ( "<input type='search' name='field_%s' size='%d' maxlength='%d'",  
					ElementArray[xe].Text, MinSize, MaxSize );

				if ( ScreenLevel == 2 )
				{
					printf ( " value='%s'", tokens[xf] );
				}

#ifdef DISABLE_FIELDS_IF_NO_UPDATE_BUTTON
				if ( ScreenLevel == 1 || (ScreenMode & SCREEN_MODE_UPDATE) == SCREEN_MODE_UPDATE )
				{
					printf ( ">\n" );
				}
				else
				{
					printf ( " disabled>\n" );
				}
#else
					printf ( ">\n" );
#endif
				xf++;
				break;

			case FIELD_MODE_DISPLAY:
				printf ( "%s\n", tokens[xf] );
				xf++;
				break;

		}
		printf ( "</td>\n" );
	}
	if ( xr > -1 )
	{
		printf ( "</tr>\n" );
	}


	/*----------------------------------------------------------
		Add Find
		or Save Delete and Navigation
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td class='buttons' colspan='%d'>\n", ScreenColumns );
	if ( ScreenLevel == 1 )
	{
		printf ( "&emsp;<input type='button' value='Quit' onclick='javascript:window.close();'>" );
		printf ( "&emsp;<input type='submit' name='submitFind' value='Find'>" );
		if  ( ScreenMode & SCREEN_MODE_INSERT )
		{
			printf ( "&emsp;<input type='submit' name='submitInsert' value='Insert'>" );
		}
	}
	else
	{
		printf ( "&emsp;<input type='button' value='Quit' onclick='javascript:window.close();'>\n" );
		printf ( "&emsp;<input type='submit' name='submitClear' value='Clear'>\n" );
		if  ( ScreenMode & SCREEN_MODE_UPDATE )
		{
			printf ( "&emsp;<input type='submit' name='submitUpdate' value='Update'>\n" );
		}
		if  ( ScreenMode & SCREEN_MODE_DELETE )
		{
			printf ( "&emsp;<input type='submit' name='submitDelete' value='Delete'>\n" );
		}
		if ( RowCount > 1 )
		{
			printf ( "&emsp;<input type='submit' name='submitFirst' value='First'%s>\n",       CurrentRow == 1 ? " disabled" : "" );
			printf ( "&emsp;<input type='submit' name='submitPrevious' value='Previous'%s>\n", CurrentRow == 1 ? " disabled" : "" );
			printf ( "&emsp;<input type='submit' name='submitNext' value='Next'%s>\n", CurrentRow == RowCount ? " disabled" : "" );
			printf ( "&emsp;<input type='submit' name='submitLast' value='Last'%s>\n", CurrentRow == RowCount ? " disabled" : "" );
			printf ( "&emsp;%d of %d", CurrentRow, RowCount );
		}
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		end 
	----------------------------------------------------------*/
	printf ( "</table>\n" );
}
