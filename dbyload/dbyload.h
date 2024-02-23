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
#include	"shslib.h"

#ifdef MAIN
#define		TYPE	/* */
#else
#define		TYPE	extern
#endif

#define		MAXBUFF		1024
#define		MAXFIELDS	24
#define		MODE_INSERT		1
#define		MODE_UPDATE		2

TYPE	int		RunMode;
TYPE	char	*Filename;
TYPE	char	Separator;
TYPE	char	*Database;
TYPE	char	*Table;
TYPE	char	*Specfile;
TYPE	char	Buffer[MAXBUFF];
TYPE	char	*FieldArray[MAXFIELDS];
TYPE	int		FieldCount;

/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/

/* dbyload.c */
int main ( int argc , char *argv []);

/* getargs.c */
void getargs ( int argc , char *argv []);

/* getspec.c */
void getspec ( void );

/* insert.c */
void insert ( void );

/* update.c */
void update ( void );
