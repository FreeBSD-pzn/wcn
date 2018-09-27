#---------------------------------------------
#
#
#
CC	:= cc
CFLAGS	:= -c -Wall
LINK	:= cc
LFLAGS	:=
LIBS	:=

#PREFIX=$(DESTDIR)/usr/local
# - Local PREFIX directory ---------------------
PREFIX=/home
BINDIR=$(PREFIX)/bin
#MANDIR=$(PREFIX)/man/man1

WCN	:= wcn.o table.o

.c.o:
	$(CC) $(CFLAGS) $*.c

all:	wcn

wcn:	$(WCN)
	$(LINK) -o wcn $(WCN) $(LIBS)


install:	wcn
		install -b wcn $(BINDIR)

clean:
		rm -f *.o wcn

.PHONY:	clean	install

#---------------------------------------------
wcn.o: wcn.c config.h
table.o: table.c config.h
#---------------------------------------------
