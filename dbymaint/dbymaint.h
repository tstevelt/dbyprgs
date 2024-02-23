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

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>

#include <stdint.h>
#include <assert.h>
#include <time.h>
#include <libgen.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <sys/types.h>

#include	"shslib.h"
#include	"weblib.h"
#include	"dbylib.h"

#ifdef MAIN
#define	TYPE	/* */
#else
#define	TYPE	extern
#endif

TYPE	char	*ScreenName, *DatabaseName;
#define		SCREEN_DIR		"/var/local/webstate"

#define		MODE_UNKNOWN	101
#define		MODE_FIND		102
#define		MODE_INSERT		103
#define		MODE_UPDATE		104
#define		MODE_DELETE		105
#define		MODE_FIRST		106
#define		MODE_PREVIOUS	107
#define		MODE_NEXT		108
#define		MODE_LAST		109
#define		MODE_CLEAR		110
#define		MODE_PRELOAD	111
TYPE	int		RunMode;

#define		SCREEN_MODE_INSERT		1
#define		SCREEN_MODE_UPDATE		2
#define		SCREEN_MODE_DELETE		4
TYPE	int		ScreenMode;
TYPE	char	SessionID[21];
TYPE	char	ScreenTable[40];
TYPE	char	ScreenTitle[128];
TYPE	int		ScreenRows;
TYPE	int		ScreenColumns;
TYPE	int		KeyField;
TYPE	char	*PreloadWhereClause;

#define		FIELD_MODE_LABEL		1
#define		FIELD_MODE_KEY			2
#define		FIELD_MODE_FIELD		3
#define		FIELD_MODE_DISPLAY		4

typedef struct
{
	int		Mode;
	char	Text [ 40 ];
	int		Row;
	int		Column;
	char	*Value;
	int		Type;
	int		Quoted;
	int		Length;
} ELEMENT;

#define		MAXELEM		100
TYPE	ELEMENT		ElementArray[MAXELEM];
TYPE	int			ElementCount;

TYPE	int			FieldCount;
TYPE	int			RowCount;
TYPE	int			CurrentRow;
TYPE	char		DataFileName[1024];
TYPE	FILE		*fpData;

/*----------------------------------------------------------
	standard database variables.	
----------------------------------------------------------*/
TYPE  MYSQL   MySql;
TYPE	DBY_OPTIONS	DbyOptions;
TYPE	char		Statement[10240];
TYPE	char		Fragment[128];
TYPE	char		WhereClause[1024];
TYPE	char		OrderByClause[1024];
#ifdef MAIN
char	*LogFileName = "/var/local/dbymaint.log";
#else
TYPE	char	*LogFileName;
#endif

/*------------------------------
:.,$d
:r ! mkproto -p *.c
------------------------------*/

/* Connect.c */
void Connect ( MYSQL *MySql , char *DatabaseName );

/* dbymaint.c */
int main ( int argc , char *argv []);

/* GetInput.c */
void GetInput ( int Pass );

/* GetScreen.c */
void GetScreen ( char *ScreenName );

/* PaintScreen.c */
void PaintScreen ( void );

/* PaintTop.c */
void PaintTop ( void );

/* TakeAction.c */
void TakeAction ( void );
