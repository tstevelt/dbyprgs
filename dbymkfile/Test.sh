#!/bin/sh
#     Utility programs for working with mysql/mariadb
# 
#     Copyright (C)  2018 - 2024 Tom Stevelt
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Affero General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Affero General Public License for more details.
# 
#     You should have received a copy of the GNU Affero General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.

# USAGE: dbymkfile dumpfile [-dates {S|D}] [-fields]

if [ ! -file schema.sql ]
then
	echo "schema.sql not found"
	exit 1
fi

echo "=== no options ==="
dbymkfile schema.sql
mv filemysql.h file_0.h

echo "=== STRING options ==="
dbymkfile schema.sql -dates S
mv filemysql.h file_S.h
diff file_0.h file_S.h

echo "=== DATEVAL options ==="
dbymkfile schema.sql -dates D
mv filemysql.h file_D.h
diff file_0.h file_D.h

echo "=== FIELDS options ==="
dbymkfile schema.sql -fields
mv filemysql.h file_F.h
diff file_0.h file_F.h

rm -f file_*.h


