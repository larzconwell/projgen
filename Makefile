PREFIX?=/usr/local

CFLAGS=-std=c99 -g -O2 -Wall -Wextra -Isrc -rdynamic

all: projgen

projgen:

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin
	install projgen $(DESTDIR)/$(PREFIX)/bin

clean:
	rm -f *.o projgen
