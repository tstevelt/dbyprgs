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

void GetInput ( int Pass )
{
	int		xa, xe, xs;

	if ( Pass == 1 )
	{
		RunMode = MODE_UNKNOWN;

		if ( webGetInput () != 0 )
		{
			printf ( "cgiIntergrate: Input error!<br>\n" );
		}		
		DatabaseName = "unknonwn";
		ScreenName = "unknonwn";
		memset ( SessionID, '\0', sizeof(SessionID) );
	}

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		
		if ( Pass == 1 )
		{
			if ( nsStrcmp ( webNames[xa], "DB" ) == 0 )
			{
				DatabaseName = webValues[xa];
			}
			else if ( nsStrcmp ( webNames[xa], "screen" ) == 0 )
			{
				ScreenName = webValues[xa];
			}
			else if ( nsStrcmp ( webNames[xa], "session" ) == 0 )
			{
				sprintf ( SessionID, "%20.20s", webValues[xa] );
			}
			else if ( nsStrcmp ( webNames[xa], "where" ) == 0 )
			{
				PreloadWhereClause = webValues[xa];
				RunMode = MODE_PRELOAD;
			}
		}
		else if ( Pass == 2 )
		{
// printf ( "pass %d, name=value %s = %s<br>\n", Pass, webNames[xa] , webValues[xa] );
			if ( nsStrcmp ( webNames[xa], "FieldCount" ) == 0 )
			{
				FieldCount = nsAtoi ( webValues[xa] );
			}
			else if ( nsStrcmp ( webNames[xa], "RowCount" ) == 0 )
			{
				RowCount = nsAtoi ( webValues[xa] );
			}
			else if ( nsStrcmp ( webNames[xa], "CurrentRow" ) == 0 )
			{
				CurrentRow = nsAtoi ( webValues[xa] );
			}
			else if ( nsStrncmp ( webNames[xa], "field_", 6 ) == 0 )
			{
				for ( xe = 0; xe < ElementCount; xe++ )
				{
					if ( nsStrcmp ( &webNames[xa][6], ElementArray[xe].Text ) == 0 )
					{
						if (( ElementArray[xe].Mode == FIELD_MODE_FIELD ) ||
							( ElementArray[xe].Mode == FIELD_MODE_KEY   ))
						{
							ElementArray[xe].Value = webValues[xa];
/*----------------------------------------------------------
	moved below loop, don't make substitution unless in find mode
							for ( xs = 0; xs < nsStrlen ( ElementArray[xe].Value ); xs++ )
							{
								if ( ElementArray[xe].Value[xs] == '*' )
								{
									ElementArray[xe].Value[xs] = '%';
								}
							}
----------------------------------------------------------*/
							break;
						}
					}
				}
				if ( xe >= ElementCount )
				{
					printf ( "Could not find field %s in array<br>\n", webNames[xa] );
				}
			}

			else if ( nsStrcmp ( webNames[xa], "submitClear" ) == 0 )
			{
				RunMode = MODE_CLEAR;
			}
			else if ( nsStrcmp ( webNames[xa], "submitFind" ) == 0 )
			{
				RunMode = MODE_FIND;
			}
			else if ( nsStrcmp ( webNames[xa], "submitInsert" ) == 0 )
			{
				RunMode = MODE_INSERT;
			}
			else if ( nsStrcmp ( webNames[xa], "submitUpdate" ) == 0 )
			{
				RunMode = MODE_UPDATE;
			}
			else if ( nsStrcmp ( webNames[xa], "submitDelete" ) == 0 )
			{
				RunMode = MODE_DELETE;
			}
			else if ( nsStrcmp ( webNames[xa], "submitFirst" ) == 0 )
			{
				RunMode = MODE_FIRST;
			}
			else if ( nsStrcmp ( webNames[xa], "submitPrevious" ) == 0 )
			{
				RunMode = MODE_PREVIOUS;
			}
			else if ( nsStrcmp ( webNames[xa], "submitNext" ) == 0 )
			{
				RunMode = MODE_NEXT;
			}
			else if ( nsStrcmp ( webNames[xa], "submitLast" ) == 0 )
			{
				RunMode = MODE_LAST;
			}
		}
	}
	if ( RunMode == MODE_FIND )
	{
		for ( xe = 0; xe < ElementCount; xe++ )
		{
			if (( ElementArray[xe].Mode == FIELD_MODE_FIELD ) ||
				( ElementArray[xe].Mode == FIELD_MODE_KEY   ))
			{
				for ( xs = 0; xs < nsStrlen ( ElementArray[xe].Value ); xs++ )
				{
					if ( ElementArray[xe].Value[xs] == '*' )
					{
						ElementArray[xe].Value[xs] = '%';
					}
				}
			}
		}
	}
}
