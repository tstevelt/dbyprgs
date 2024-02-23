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

TYPE	char	DatabaseName[256];
TYPE	char	TableName[128];
TYPE	char	KeyFieldName[128];
TYPE	char	FixFieldName[128];
TYPE	int		OneTable;

TYPE	int		RunMode;


/*----------------------------------------------------------
	standard database variables.	
----------------------------------------------------------*/
TYPE  MYSQL   MySql;
TYPE	DBY_OPTIONS	DbyOptions;
TYPE	char		SelectStatement[10240];
TYPE	char		UpdateStatement[10240];
TYPE	char		Fragment[128];
TYPE	char		WhereClause[1024];
TYPE	char		OrderByClause[1024];
#ifdef MAIN
char	*LogFileName = "/var/local/dbyfix.log";
#else
TYPE	char	*LogFileName;
#endif

/*------------------------------
:.,$d
:r ! mkproto -p *.c
------------------------------*/


/* dbyfix.c */
int main ( int argc , char *argv []);

/* getargs.c */
void getargs ( int argc , char *argv []);
