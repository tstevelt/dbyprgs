
PROG=dbymaint
DEF = /usr/local/include

LIB = dbymaint_64.a
# XLIB = /usr/local/lib64/shslib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a /usr/lib64/libmysqlclient.so  -lz -lssl -luuid -lcrypto -lm

XLIB = /usr/local/lib64/weblib.a  \
		/usr/local/lib64/dbylib.a \
		/usr/local/lib64/shslib.a  \
		/usr/local/lib/libmysqlclient.a \
		/usr/local/lib/libcrypto.a \
		/usr/local/lib/libxlsxwriter.a \
		/usr/local/lib/libssl.a \
		/usr/local/Cellar/zlib/1.3.1/lib/libz.a

# libta_lib.a



WEBDIR1 = /Users/tms/Sites/shs_stocks
WEBDIR2 = /Users/tms/Sites/madisonfreethinkers
WEBDIR3 = /Users/tms/Sites/acct
WEBDIR4 = /Users/tms/Sites/shs
WEBDIR5 = /Users/tms/Sites/nutrition

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
	echo "NOT! gcc to load $(PRG2)"
	# gcc $(LDFLAGS) -o $(PRG2) $(LIB) $(XLIB)

$(CSS2): dbymaint_madisonfreethinkers.css
	# cp -pv dbymaint_madisonfreethinkers.css $(CSS2)

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
	echo "NOT using gcc to load $(PRG5)"
	# gcc $(LDFLAGS) -o $(PRG5) $(LIB) $(XLIB)

$(CSS5): dbymaint_madisonfreethinkers.css
	# cp -pv dbymaint_madisonfreethinkers.css $(CSS5)

$(LIB): $(FILES)

$(FILES): dbymaint.h $(DEF)/shslib.h $(DEF)/shslib.h $(DEF)/weblib.h $(DEF)/dbylib.h

clean:
	rm -f $(LIB)
	rm -f TODO

all:
	make clean
	make

.PRECIOUS: $(LIB)

