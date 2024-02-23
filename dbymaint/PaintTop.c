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

#define		CSS_VERSION		"20190403_03"

void PaintTop ()
{
	WEBPARMS	*ptrWebParms = webInitParms();

	// cgiPagePart ( PP_CGI_START, 0, NULL );
	webContentHtml ();
	
	ptrWebParms->WP_Title = "Query by Form";
	// cgiPagePart ( PP_HEAD_START_HTML5, 1, ptrWebParms );
	webStartHead ( ptrWebParms );

	printf ( "<meta name='viewport' content='width=device-width, initial-scale=1'>" );
	printf ( "<link rel='stylesheet' href='dbymaint.css?version=%s'>\n", CSS_VERSION );

	//cgiPagePart ( PP_HEAD_TO_BODY_CSS, 0, NULL );
	webHeadToBody ( ptrWebParms );

	ptrWebParms->WP_FormName   = "dbymaint";
	ptrWebParms->WP_FormAction = "dbymaint.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	// cgiPagePart (  PP_FORM_START, 4, ptrWebParms );
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

}
