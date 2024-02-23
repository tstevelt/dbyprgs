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

static int DebugTakeAction = 0;

static int FindCB ( DBY_QUERY *Query )
{
	int		xf;
	static int firstpass = 1;

	if ( firstpass )
	{
		firstpass = 0;
		FieldCount = mysql_num_fields ( Query->Result );
		RowCount = 0;
		CurrentRow = 0;
		
		sprintf ( DataFileName, "%s/%s/%s.data", SCREEN_DIR, DatabaseName, SessionID );

		if (( fpData = fopen ( DataFileName, "w" )) == (FILE *)0 )
		{
			printf ( "Cannot create %s for output<br>\n", DataFileName );
			return ( -1 );
		}
	}

	if ( DebugTakeAction )
	{
		for ( xf = 0; xf < FieldCount; xf++ )
		{
			printf ( "%s|", Query->EachRow[xf] );
		}
		printf ( "<br>\n" );
	}

	for ( xf = 0; xf < FieldCount; xf++ )
	{
		fprintf ( fpData, "%s|", Query->EachRow[xf] );
	}
	fprintf ( fpData, "\n" );

	RowCount++;

	return ( 0 );
}

static void LoadOne ()
{
	int		xe, xo;

	sprintf ( Statement, "select" );
	for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
	{
		switch ( ElementArray[xe].Mode )
		{
			case FIELD_MODE_LABEL:
				break;

			case FIELD_MODE_KEY:
			case FIELD_MODE_FIELD:
			case FIELD_MODE_DISPLAY:
				if ( ! ElementArray[xe].Text )
				{
					break;
				}
				if ( xo == 0 )
				{
					strcat ( Statement, " " );
				}
				else
				{
					strcat ( Statement, ", " );
				}
				strcat ( Statement, ElementArray[xe].Text );
				xo++;
				break;

		}
	}
	strcat ( Statement, " from " );
	strcat ( Statement, ScreenTable );
	sprintf ( Fragment, " where %s = '%s'",
		ElementArray[KeyField].Text, ElementArray[KeyField].Value );
	strcat ( Statement, Fragment );
	dbySelectCB ( "dbymaint", &MySql, Statement, (int(*)()) FindCB, LogFileName );
	if ( RowCount > 0 )
	{
		nsFclose ( fpData );
		CurrentRow = 1;
	}
}

void TakeAction ()
{
	int		xe, xo, Offset;

	switch ( RunMode )
	{
		case MODE_FIND:
		case MODE_PRELOAD:
			sprintf ( Statement, "select" );
			for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
			{
				switch ( ElementArray[xe].Mode )
				{
					case FIELD_MODE_LABEL:
						break;

					case FIELD_MODE_KEY:
					case FIELD_MODE_FIELD:
					case FIELD_MODE_DISPLAY:
						if ( ! ElementArray[xe].Text )
						{
							break;
						}
						if ( xo == 0 )
						{
							strcat ( Statement, " " );
						}
						else
						{
							strcat ( Statement, ", " );
						}
						strcat ( Statement, ElementArray[xe].Text );
						xo++;
						break;

				}
			}
			strcat ( Statement, " from " );
			strcat ( Statement, ScreenTable );
			if ( RunMode == MODE_FIND )
			{
				if ( DebugTakeAction )
				{
					printf ( "MODE FIND<br>\n" );
				}

				for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
				{
					if ( DebugTakeAction )
					{
						printf ( "elem %d mode %d, text %s, value %s<br>\n", 
							xe, ElementArray[xe].Mode, ElementArray[xe].Text, ElementArray[xe].Value );
					}

					switch ( ElementArray[xe].Mode )
					{
						case FIELD_MODE_LABEL:
							break;

						case FIELD_MODE_KEY:
						case FIELD_MODE_FIELD:
						case FIELD_MODE_DISPLAY:
							if ( ! ElementArray[xe].Text )
							{
								if ( DebugTakeAction )
								{
									printf ( " not Text<br>\n" );
								}
								break;
							}
							if ( ! ElementArray[xe].Value )
							{
								if ( DebugTakeAction )
								{
									printf ( " not Value<br>\n" );
								}
								break;
							}
							if ( nsStrlen ( ElementArray[xe].Value ) == 0 )
							{
								if ( DebugTakeAction )
								{
									printf ( " not Length<br>\n" );
								}
								break;
							}

							if ( xo == 0 )
							{
								strcat ( Statement, " where " );
							}
							else
							{
								strcat ( Statement, " and " );
							}

							strcat ( Statement, ElementArray[xe].Text );

							Offset = 0;
							if ( nsStrchr ( ElementArray[xe].Value , '%' ) != (char *)0 )
							{
								strcat ( Statement, " like " );
							}
							else if ( nsStrncmp ( ElementArray[xe].Value, "<=", 2 ) == 0 )
							{
								strcat ( Statement, " <= " );
								Offset = 2;
							}
							else if ( nsStrncmp ( ElementArray[xe].Value, "<", 1 ) == 0 )
							{
								strcat ( Statement, " < " );
								Offset = 1;
							}
							else if ( nsStrncmp ( ElementArray[xe].Value, ">=", 2 ) == 0 )
							{
								strcat ( Statement, " >= " );
								Offset = 2;
							}
							else if ( nsStrncmp ( ElementArray[xe].Value, ">", 1 ) == 0 )
							{
								strcat ( Statement, " > " );
								Offset = 1;
							}
							else
							{
								strcat ( Statement, " = " );
							}

							if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );

							strcat ( Statement, &ElementArray[xe].Value[Offset] );

							if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );

							xo++;
							break;

					}
				}
				if ( xo == 0 )
				{
					printf ( "No criteria entered.<br>\n" );
					RunMode = MODE_CLEAR;
					break;
				}
			}
			else
			{
				strcat ( Statement, " where " );
				strcat ( Statement, PreloadWhereClause );
				RunMode = MODE_FIND;
			}

			if ( DebugTakeAction )
			{
				printf ( "%s<br>\n", Statement );
			}
			
			dbySelectCB ( "dbymaint", &MySql, Statement, (int(*)()) FindCB, LogFileName );
			if ( RowCount > 0 )
			{
				nsFclose ( fpData );
				CurrentRow = 1;
			}
			else
			{
				printf ( "Nothing matches criteria!<br>\n" );
				RunMode = MODE_CLEAR;
			}
			break;

		case MODE_FIRST:
			CurrentRow = 1;
			break;

		case MODE_PREVIOUS:
			if ( CurrentRow > 1 )
			{
				CurrentRow--;
			}
			break;

		case MODE_NEXT:
			if ( CurrentRow < RowCount )
			{
				CurrentRow++;
			}
			break;

		case MODE_LAST:
			CurrentRow = RowCount;
			break;

		case MODE_CLEAR:
			CurrentRow = RowCount = 0;
			break;

		case MODE_INSERT:
			sprintf ( Statement, "insert into %s ", ScreenTable );
			for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
			{
				switch ( ElementArray[xe].Mode )
				{
					case FIELD_MODE_LABEL:
						break;

					case FIELD_MODE_KEY:
					case FIELD_MODE_FIELD:
						if ( ! ElementArray[xe].Text )
						{
							break;
						}
						if ( ! ElementArray[xe].Value || nsStrlen(ElementArray[xe].Value) == 0 )
						{
							break;
						}
						if ( xo == 0 )
						{
							strcat ( Statement, " (" );
						}
						else
						{
							strcat ( Statement, ", " );
						}
						strcat ( Statement, ElementArray[xe].Text );
						xo++;
						break;

				}
			}
			if ( xo > 0 )
			{
				strcat ( Statement, ") values" );
			}
			else
			{
				printf ( "No columns had values<br>\n" );
				RunMode = MODE_CLEAR;
				break;
			}

			for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
			{
				switch ( ElementArray[xe].Mode )
				{
					case FIELD_MODE_LABEL:
						break;

					case FIELD_MODE_KEY:
					case FIELD_MODE_FIELD:
						if ( ! ElementArray[xe].Text )
						{
							break;
						}
						if ( ! ElementArray[xe].Value || nsStrlen(ElementArray[xe].Value) == 0 )
						{
							break;
						}
						if ( xo == 0 )
						{
							strcat ( Statement, " (" );
						}
						else
						{
							strcat ( Statement, ", " );
						}
						if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );
						strcat ( Statement, ElementArray[xe].Value );
						if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );
						xo++;
						break;

				}
			}
			if ( xo > 0 )
			{
				strcat ( Statement, ")" );
			}
			else
			{
				printf ( "No columns had values<br>\n" );
				RunMode = MODE_CLEAR;
				break;
			}

			if ( DebugTakeAction )
			{
				printf ( "%s<br>\n", Statement );
			}
			
			RowCount = dbyInsert ( "dbymaint", &MySql, Statement, 0, LogFileName );
			if ( RowCount == 0 )
			{
				printf ( "Insert failed!<br>\n" );
				RunMode = MODE_CLEAR;
			}
			else if ( KeyField > 0 )
			{
				LoadOne ();
			}
			else
			{
				// don't know how to get it back without key value
				// RowCount = 1;
				// CurrentRow = 1;
				printf ( "Okay<br>\n" );
				RunMode = MODE_CLEAR;
				RowCount = 0;
				CurrentRow = 0;
			}
			break;

		case MODE_UPDATE:
			sprintf ( Statement, "update %s set ", ScreenTable );
			for ( xe = 0, xo = 0; xe < ElementCount; xe++ )
			{
				switch ( ElementArray[xe].Mode )
				{
					case FIELD_MODE_LABEL:
						break;

					case FIELD_MODE_KEY:
					case FIELD_MODE_FIELD:
						if ( ! ElementArray[xe].Text )
						{
							break;
						}
						if ( ! ElementArray[xe].Value )
						{
							break;
						}
						if ( nsStrlen(ElementArray[xe].Value) == 0 )
						{
							break;
						}
						if ( nsStrncmp ( ElementArray[xe].Value, "(null)", 6 ) == 0 )
						{
							break;
						}

						if ( xo > 0 )
						{
							strcat ( Statement, ", " );
						}
						strcat ( Statement, ElementArray[xe].Text );
						strcat ( Statement, "=" );
						if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );
						strcat ( Statement, ElementArray[xe].Value );
						if ( ElementArray[xe].Quoted ) strcat ( Statement, "'" );
						xo++;
						break;

				}
			}
			if ( xo == 0 )
			{
				printf ( "No columns had values<br>\n" );
				RunMode = MODE_CLEAR;
				break;
			}

			if ( ElementArray[KeyField].Quoted )
			{
				sprintf  ( Fragment, " where %s = '%s'", ElementArray[KeyField].Text, ElementArray[KeyField].Value );
			}
			else
			{
				sprintf  ( Fragment, " where %s = %s", ElementArray[KeyField].Text, ElementArray[KeyField].Value );
			}
			strcat ( Statement, Fragment );

			if ( DebugTakeAction )
			{
				printf ( "%s<br>\n", Statement );
			}
			
			RowCount = dbyUpdate ( "dbymaint", &MySql, Statement, 0, LogFileName );
			if ( RowCount == 0 )
			{
				printf ( "Nothing changed<br>\n" );
				RunMode = MODE_CLEAR;
			}
			else if ( KeyField > 0 )
			{
				LoadOne ();
			}
			else
			{
				printf ( "Okay<br>\n" );
			}
			break;

		case MODE_DELETE:
			sprintf ( Statement, "delete from %s where %s = '%s'",
					ScreenTable, ElementArray[KeyField].Text, ElementArray[KeyField].Value );

			RowCount = dbyDelete ( "dbymaint", &MySql, Statement, 0, LogFileName );
			if ( RowCount == 0 )
			{
				printf ( "Delete Failed<br>\n" );
			}
			else
			{
				printf ( "Okay<br>\n" );
				RunMode = MODE_CLEAR;
				RowCount = 0;
				CurrentRow = 0;
			}

			break;

	}
}
