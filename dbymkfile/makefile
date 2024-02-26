PROG = dbymkfile
LIB = dbymkfile.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)
XLIB = /usr/local/lib64/shslib.a

FILES = \
	$(LIB)(dbymkfile.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): dbymkfile.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

