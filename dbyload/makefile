PROG = dbyload
LIB = dbyload.a
MYSQL_DIR = /usr/lib64
MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
MLIB = $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)

XLIB = /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

FILES = \
	$(LIB)(dbyload.o)	\
	$(LIB)(getargs.o)	\
	$(LIB)(getspec.o)	\
	$(LIB)(insert.o)	\
	$(LIB)(update.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB) $(MLIB)

$(LIB): $(FILES)

$(FILES): dbyload.h $(DEF)/shslib.h $(DEF)/dbylib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

