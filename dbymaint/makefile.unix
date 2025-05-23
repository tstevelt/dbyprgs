
PROG=dbymaint

ifeq ($(LDFLAGS),"-m32")
LIB = dbymaint.a
XLIB = /usr/local/lib/shslib.a /usr/local/lib/weblib.a /usr/local/lib/dbylib.a /usr/local/lib/shslib.a /usr/lib/mysql/libmysqlclient.so.18.0.0  -lz -lssl -luuid -lcrypto -lm
else
LIB = dbymaint_64.a
XLIB = /usr/local/lib64/shslib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a /usr/lib64/libmysqlclient.so  -lz -lssl -luuid -lcrypto -lm
endif

DEF = /usr/local/include

WEBDIR1 = /var/www/vhosts/shs_stocks/html
WEBDIR2 = /var/www/vhosts/madisonfreethinkers/html
WEBDIR3 = /var/www/vhosts/acct/html
WEBDIR4 = /var/www/vhosts/shs_acct/html
WEBDIR5 = /var/www/vhosts/nutrition/html

PRG1 = $(WEBDIR1)/$(PROG).cgi
PRG2 = $(WEBDIR2)/$(PROG).cgi
PRG3 = $(WEBDIR3)/$(PROG).cgi
PRG4 = $(WEBDIR4)/$(PROG).cgi
PRG5 = $(WEBDIR5)/$(PROG).cgi

CSS1 = $(WEBDIR1)/$(PROG).css
CSS2 = $(WEBDIR2)/$(PROG).css
CSS3 = $(WEBDIR3)/$(PROG).css
CSS4 = $(WEBDIR4)/$(PROG).css
CSS5 = $(WEBDIR5)/$(PROG).css

FILES = \
	$(LIB)(dbymaint.o)	\
	$(LIB)(Connect.o)		\
	$(LIB)(PaintTop.o)	\
	$(LIB)(PaintScreen.o)	\
	$(LIB)(GetInput.o)	\
	$(LIB)(GetScreen.o)	\
	$(LIB)(TakeAction.o)

.SILENT:

STAMP: $(PRG1) $(PRG2) $(PRG3) $(PRG4) $(PRG5) $(CSS1) $(CSS2) $(CSS3) $(CSS4) $(CSS5)

$(PRG1): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG1)"
	gcc $(LDFLAGS) -o $(PRG1) $(LIB) $(XLIB)

$(CSS1): dbymaint_invest.css
	cp -pv dbymaint_invest.css $(CSS1)

$(PRG2): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG2)"
	gcc $(LDFLAGS) -o $(PRG2) $(LIB) $(XLIB)

$(CSS2): dbymaint_madisonfreethinkers.css
	cp -pv dbymaint_madisonfreethinkers.css $(CSS2)

$(PRG3): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG3)"
	gcc $(LDFLAGS) -o $(PRG3) $(LIB) $(XLIB)

$(CSS3): dbymaint_madisonfreethinkers.css
	cp -pv dbymaint_madisonfreethinkers.css $(CSS3)

$(PRG4): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG4)"
	gcc $(LDFLAGS) -o $(PRG4) $(LIB) $(XLIB)

$(CSS4): dbymaint_madisonfreethinkers.css
	cp -pv dbymaint_madisonfreethinkers.css $(CSS4)

$(PRG5): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG5)"
	gcc $(LDFLAGS) -o $(PRG5) $(LIB) $(XLIB)

$(CSS5): dbymaint_madisonfreethinkers.css
	cp -pv dbymaint_madisonfreethinkers.css $(CSS5)

$(LIB): $(FILES)

$(FILES): dbymaint.h $(DEF)/shslib.h $(DEF)/shslib.h $(DEF)/weblib.h $(DEF)/dbylib.h

clean:
	rm -f $(LIB)
	rm -f TODO

all:
	make clean
	make

.PRECIOUS: $(LIB)

