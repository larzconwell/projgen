PREFIX?=/usr/local

CC=gcc
CFLAGS=-std=c99 -g -O2 -Wall -Wextra -Isrc -rdynamic

all: projgen

projgen: path.o deps/commander/commander.o deps/bstring/bstrlib.o

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin
	install -d $(DESTDIR)/$(PREFIX)/share/projgen
	install projgen $(DESTDIR)/$(PREFIX)/bin
	cp -r langs licenses $(DESTDIR)/$(PREFIX)/share/projgen

uninstall:
	rm -rf $(DESTDIR)/$(PREFIX)/share/projgen
	rm -f $(DESTDIR)/$(PREFIX)/bin/projgen

clean:
	rm -f *.o deps/*/*.o projgen
