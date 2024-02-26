/*----------------------------------------------------------------------------
	Program : dbymkfile.c
	Author  : Tom Stevelt
	Date    : 12/06/2018
	Synopsis: Make file<dbname>.h
	Return  : 

	Generate Dump file:
	mysqldump -h db1124.cltg6yf2dfpd.us-east-2.rds.amazonaws.com --no-data madisonfreethinkers > schema.sql
	mysqldump -h localhost --no-data acct                > schema.sql


	MySQL data types:
		https://dev.mysql.com/doc/refman/8.0/en/storage-requirements.html

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		11/13/2020	Added option for field defines output (FIELDS_).
						Added option for dates as string or DATEVAL.
						Broke out getargs().
	tms		09/07/2022	Added bigint
----------------------------------------------------------------------------*/
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

#define		MAIN
#include	"dbymkfile.h"

#undef DEBUG

int main ( int argc, char *argv[] )
{
	char	*ofn = "filemysql.h";
	FILE	*ifp;
	FILE	*ofp;
	char	xbuffer[1024];
#define		MAXTOKS		20
	char	*tokens[MAXTOKS];
	int		tokcnt, xi, xl;
	char	*toktype[MAXTOKS];
#define		MAXFLDS		256
	char	*tokflds[MAXFLDS];
	int		fldcnt, xf;
	char	CurrentTable[100];
	char	UpperTable[100];
	char	LowerTable[100];
	int		TableNumber;
	char	CurrentField[100];
//	char	UpperField[100];
	char	LowerField[100];
	int		FieldNumber;
	int		InTable;
	int		IsUnsigned;
	int		FieldLength, ArrayLength;
	char	FieldType[100];
	char	FieldList[4096];

	getargs ( argc, argv );

	if (( ifp = fopen ( ifn, "r" )) == (FILE *)0 )
	{
		printf ( "Cannot open dump file [%s] for reading.\n", ifn );
		exit ( 1 );
	}

	if (( ofp = fopen ( ofn, "w" )) == (FILE *)0 )
	{
		printf ( "Cannot create [%s] for output.\n", ofn );
		exit ( 1 );
	}

	fprintf ( ofp, "/*--------------------------------------------------------\n" );
	fprintf ( ofp, "\t%s for schema %s\n", ofn, ifn );
	fprintf ( ofp, "--------------------------------------------------------*/\n" );
	
	xl = sprintf ( FieldList, "%s", ofn );
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( FieldList[xi] == '.' )
		{
			FieldList[xi] = '_';
		}
		else
		{
			FieldList[xi] = toupper ( FieldList[xi] );
		}
	}

	fprintf ( ofp, "#ifndef %s\n", FieldList );
	fprintf ( ofp, "#define %s\n", FieldList );
	fprintf ( ofp, "\n" );
	fprintf ( ofp, "#ifndef TYPE\n" );
	fprintf ( ofp, "#ifdef MAIN\n" );
	fprintf ( ofp, "#define  TYPE   /* */\n" );
	fprintf ( ofp, "#else\n" );
	fprintf ( ofp, "#define  TYPE   extern\n" );
	fprintf ( ofp, "#endif\n" );
	fprintf ( ofp, "#endif\n" );
	fprintf ( ofp, "\n" );

	memset ( CurrentTable, '\0', sizeof(CurrentTable) );
	TableNumber = 0;
	InTable = 0;

	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != (char *)0 )
	{
		tokcnt = GetTokensW ( xbuffer, tokens, MAXTOKS );

		if ( tokcnt < 3 )
		{
			continue;
		}

		if (( nsStrcmp ( tokens[0], "CREATE" ) == 0 ) &&
			( nsStrcmp ( tokens[1], "TABLE"  ) == 0 ))
		{
			xl = nsStrlen ( tokens[2] );	
			xl -= 2;
#ifdef DEBUG
			printf ( "%*.*s\n", xl, xl, &tokens[2][1] );
#endif

			sprintf ( CurrentTable, "%*.*s", xl, xl, &tokens[2][1] );
			for ( xi = 0; xi < xl; xi++ )
			{
				UpperTable[xi] = toupper ( CurrentTable[xi] );
				LowerTable[xi] = tolower ( CurrentTable[xi] );
			}
			UpperTable[xl] = '\0';
			LowerTable[xl] = '\0';

			fprintf ( ofp, "#ifdef %s\n", UpperTable );
			fprintf ( ofp, "#define TABLE_%s %d\n", CurrentTable, ++TableNumber );
			fprintf ( ofp, "typedef struct\n" );
			fprintf ( ofp, "{\n" );

			InTable = 1;
			memset ( FieldList, '\0', sizeof(FieldList) );
			FieldNumber = 0;
		}
		else if ( InTable == 1 )
		{
			if ( nsStrncmp ( tokens[1], "ENGINE=", 7 ) == 0 )
			{
				fprintf ( ofp, "} X%s;\n", UpperTable );

				fprintf ( ofp, "TYPE X%s x%s;\n", UpperTable, LowerTable );

				if ( FieldNumber )
				{
					fprintf ( ofp, "#define FIELDS_%s  \"%s\"\n", UpperTable, FieldList );
				}

/*----------------------------------------------------------
// fixit				
#define		MAXFLDS		256
	char	*tokflds[MAXFLDS];
	int		fldcnt, xf;
----------------------------------------------------------*/
				if ( FieldDefines )
				{
					fldcnt = GetTokensA ( FieldList, ",", tokflds, MAXFLDS );
					for ( xf = 0; xf < fldcnt; xf++ )
					{
						fprintf ( ofp, "#define\t%s_%s\t%d\n", UpperTable, tokflds[xf], xf );
					}
				}

				fprintf ( ofp, "#endif\n\n" );
				InTable = 0;
			}
			else if ( tokens[0][0] == '`' )
			{
				/*---------------------------------------------------------------------------
					CREATE TABLE `company` (
					  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
					  `Gmember` int(11) unsigned DEFAULT NULL,
					  `Gname` varchar(60) NOT NULL DEFAULT '',
					  `Gblurb` varchar(60) NOT NULL DEFAULT '',
					  PRIMARY KEY (`id`),
					  KEY `groupcontact` (`Gmember`),
					  CONSTRAINT `groupcontact` FOREIGN KEY (`Gmember`) REFERENCES `member` (`id`)
					) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
				---------------------------------------------------------------------------*/
				xl = nsStrlen ( tokens[0] );	
				xl -= 2;
#ifdef DEBUG
				printf ( "    %*.*s\n", xl, xl, &tokens[0][1] );
#endif

				sprintf ( CurrentField, "%*.*s", xl, xl, &tokens[0][1] );
				for ( xi = 0; xi < xl; xi++ )
				{
					// UpperField[xi] = toupper ( CurrentField[xi] );
					LowerField[xi] = tolower ( CurrentField[xi] );
				}
				// UpperField[xl] = '\0';
				LowerField[xl] = '\0';

				GetTokensD ( tokens[1], "() ", toktype, MAXTOKS );

				if ( nsStrcmp ( tokens[2], "unsigned" ) == 0 )
				{
					IsUnsigned = 1;
				}
				else
				{
					IsUnsigned = 0;
				}

				FieldLength = nsAtoi ( toktype[1] );
				ArrayLength = 0;

				if ( nsStrcmp ( toktype[0], "int" ) == 0 )
				{
					if ( FieldLength < 5 )
					{
						sprintf ( FieldType, "%sshort", IsUnsigned ? "unsigned " : "" );
					}
					else
					{
						sprintf ( FieldType, "%slong", IsUnsigned ? "unsigned " : "" );
					}
				}
				else if ( nsStrcmp ( toktype[0], "smallint" ) == 0 )
				{
					sprintf ( FieldType, "%sshort", IsUnsigned ? "unsigned " : "" );
				}
				else if ( nsStrcmp ( toktype[0], "bigint" ) == 0 )
				{
					sprintf ( FieldType, "%slong", IsUnsigned ? "unsigned " : "" );
				}
				else if ( nsStrcmp ( toktype[0], "double" ) == 0 )
				{
					// sprintf ( FieldType, "%sdouble", IsUnsigned ? "unsigned " : "" );
					sprintf ( FieldType, "double" );
				}
				else if ( nsStrcmp ( toktype[0], "date" ) == 0 )
				{
					if ( DatesAsStrings == 1 )
					{
						sprintf ( FieldType, "char " );
						ArrayLength = 11 + 1;
					}
					else
					{
						sprintf ( FieldType, "DATEVAL " );
					}
				}
				else if ( nsStrcmp ( toktype[0], "datetime" ) == 0 )
				{
					sprintf ( FieldType, "char " );
					ArrayLength = 20 + 1;
				}
				else if ( nsStrcmp ( toktype[0], "time" ) == 0 )
				{
					sprintf ( FieldType, "char " );
					ArrayLength = 9 + 1;
				}
				else if ( nsStrcmp ( toktype[0], "tinyint" ) == 0 )
				{
					sprintf ( FieldType, "char " );
				}
				else if ( nsStrcmp ( toktype[0], "varchar" ) == 0 )
				{
					// sprintf ( FieldType, "%schar *", IsUnsigned ? "unsigned " : "" );
					sprintf ( FieldType, "%schar ", IsUnsigned ? "unsigned " : "" );
					ArrayLength = FieldLength + 1;
				}
				else if ( nsStrcmp ( toktype[0], "char" ) == 0 )
				{
					/*----------------------------------------------------------
					  `Mstatus` char(1) DEFAULT 'P',
						`Mrole` char(1) DEFAULT 'M',
						  `Mipaddr` char(16) DEFAULT NULL,
					----------------------------------------------------------*/
					sprintf ( FieldType, "%schar ", IsUnsigned ? "unsigned " : "" );
					ArrayLength = FieldLength + 1;
				}
				else
				{
					sprintf ( FieldType, "/*unknown*/ %s%s ", IsUnsigned ? "unsigned " : "", toktype[0] );
				}

				if ( ArrayLength == 0 )
				{
					fprintf ( ofp, "\t%s	x%s;\n", FieldType, LowerField );
				}
				else
				{
					fprintf ( ofp, "\t%s	x%s[%d];\n", FieldType, LowerField, ArrayLength );
				}

				if ( FieldNumber )
				{
					strcat ( FieldList, "," );
				}
				strcat ( FieldList, CurrentField );
				FieldNumber++;

			}
		}
	}

	fprintf ( ofp, "#endif\n" );

	nsFclose ( ifp );
	nsFclose ( ofp );

	if ( TableNumber == 0 )
	{
		printf ( "No tables found\n" );
	}
	else
	{
		printf ( "Wrote %d tables\n", TableNumber );
	}

	return ( 0 );
}
