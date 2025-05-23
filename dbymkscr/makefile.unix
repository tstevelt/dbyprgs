
PROG=dbymkscr

ifeq ($(LDFLAGS),"-m32")
LIB = dbymkscr.a
XLIB = /usr/local/lib/dbylib.a /usr/local/lib/shslib.a /usr/lib/mysql/libmysqlclient.so.18.0.0  -lz -lssl -luuid -lcrypto -lm
else
LIB = dbymkscr_64.a
XLIB = /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a /usr/lib64/libmysqlclient.so  -lz -lssl -luuid -lcrypto -lm
endif

DEF = /usr/local/include

PRG = /usr/local/bin/$(PROG)

FILES = \
	$(LIB)(dbymkscr.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc $(LDFLAGS) -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): dbymkscr.h $(DEF)/shslib.h $(DEF)/dbylib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

