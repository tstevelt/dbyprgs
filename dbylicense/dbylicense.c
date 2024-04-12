/*----------------------------------------------------------------------------
	Program : dbylicense.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Dec 2020
	Synopsis: Create license keys for mysql databases.
				Also check a key for validity.
	Return  : Print results to stdout.

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

#define	MAIN
#include	"dbylicense.h"

static char *umsg [] = 
{
	"USAGE: dbylicense -create database expires [appstring]",
	"       expires - mm/dd/yyyy or never",
	"USAGE: dbylicense -check  licensekey",
	""
};

int main ( int argc, char *argv[] )
{
	char	*Database;
	DATEVAL	dvToday;
	DATEVAL	dvExpires;
	char	*CheckLicense;
	char	Salt;
	#define		MAXDB		48
	#define		KEYLEN		64
	char	PlainText[KEYLEN+2];
	char	LicenseKey[KEYLEN+2];
	unsigned char	CheckSum;
	unsigned char	KeySum;
	int		xi, xl, xc;
	char	*tokens[5];
	int		tokcnt;
	int		Verbose = 1;
	char	*cp, *AppString;

	CurrentDateval ( &dvToday );

	if ( argc < 3 )
	{
		shsUsage ( stdout, NULL, umsg, 3, 1 );
	}

	AppString = (char *)0;

	if ( argc >= 4 && nsStrcmp ( argv[1], "-create" ) == 0 )
	{
		Database = argv[2];
		if ( nsStrcmp ( argv[3], "never" ) == 0 )
		{
			dvExpires.month = 12;
			dvExpires.day = 31;
			dvExpires.year4 = 9999;
		}
		else
		{
			StrToDateval ( argv[3], &dvExpires );

			if ( ValidDateval ( &dvExpires, 0 ) != 0 )
			{
				shsUsage ( stdout, "Invallid date", umsg, 3, 1 );
			}

			if ( CompareDateval ( &dvToday, &dvExpires ) >= 0 )
			{
				shsUsage ( stdout, "Expiration date is in the past", umsg, 3, 1 );
			}
		}

		if ( argc == 5 )
		{
			AppString = argv[4];
		}

		shs_seed_random ();


		if ( AppString )
		{
			xl = sprintf ( PlainText, "%.*s:%04d%02d%02d:%s", 
				MAXDB, Database,
				dvExpires.year4, dvExpires.month, dvExpires.day, 
				AppString );
		}
		else
		{
			xl = sprintf ( PlainText, "%.*s:%04d%02d%02d", 
				MAXDB, Database,
				dvExpires.year4, dvExpires.month, dvExpires.day );
		}

		if ( Verbose )
		{
			printf ( "%s\n", PlainText );
		}

		for ( xi = 0, CheckSum = 0; xi < xl; xi++ )
		{
			CheckSum += ((xi%2) ? 3 : 2) * PlainText[xi];
		}

		if ( AppString )
		{
			xl = sprintf ( PlainText, "%.*s:%04d%02d%02d:%s:%d", 
				MAXDB, Database,
				dvExpires.year4, dvExpires.month, dvExpires.day, 
				AppString,
				(short) CheckSum );
		}
		else
		{
			xl = sprintf ( PlainText, "%.*s:%04d%02d%02d:%d", 
				MAXDB, Database,
				dvExpires.year4, dvExpires.month, dvExpires.day, 
				(short) CheckSum );
		}

		if ( Verbose )
		{
			printf ( "%s\n", PlainText );
		}

		Salt = (char) random_range ( (long) 'a', (long) 'z' );

		sprintf ( LicenseKey, "%s", tmsEncode(Salt,PlainText) );

		if ( Verbose )
		{
			printf ( "%s\n", LicenseKey );
		}
		else
		{
			printf ( "%s -->  %s\n", Database, LicenseKey );
		}
	}
	else if ( argc >= 3 && nsStrcmp ( argv[1], "-check" ) == 0 )
	{
		int		CharCount;

		CheckLicense = argv[2];
		xl = sprintf ( PlainText, "%s", tmsDecode(CheckLicense) );
		printf ( "%s", PlainText );

		cp = nsStrrchr ( PlainText, ':' );
		cp++;
		KeySum = nsAtoi ( cp );

		CharCount = nsCharCount ( PlainText, ':' );

		for ( xi = 0, xc = 0, CheckSum = 0; xi < xl; xi++ )
		{
			if ( PlainText[xi] == ':' )
			{
				xc++;
				if ( xc == CharCount )
				{
					break;
				}
			}

			CheckSum += ((xi%2) ? 3 : 2) * PlainText[xi];
		}

		while ( 1 )
		{
			if ( KeySum != (short) CheckSum )
			{
				printf ( "  HACKED\n" );
				printf ( "  CharCount %d, CheckSum %d, KeySum %d\n", CharCount, CheckSum, KeySum );
				break;
			}

			if (( tokcnt = GetTokensStd ( PlainText, ":", tokens, 5, 0 )) < 3 )
			{
				printf ( "  SYNTAX ERROR\n" );
				break;
			}


			StrToDatevalFmt ( tokens[1], DATEFMT_YYYYMMDD, &dvExpires );

			if ( CompareDateval ( &dvToday, &dvExpires ) >= 0 )
			{
				printf ( "  EXPIRED\n" );
				break;
			}

			printf ( " is okay\n" );

			printf ( "Database %s\n", tokens[0] );
			printf ( "Expires  %d/%d/%d\n", dvExpires.month, dvExpires.day, dvExpires.year4 );
			if ( tokcnt == 4 )
			{
				printf ( "AppStr   %s\n", tokens[2] );
			}
			break;
		}
	}
	else
	{
		shsUsage ( stdout, NULL, umsg, 3, 1 );
	}


	return ( 0 );
}
