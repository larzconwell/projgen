PREFIX?=/usr/local

CC=gcc
CFLAGS=-std=c99 -g -O2 -Wall -Wextra -Isrc -rdynamic

all: projgen

projgen: deps/commander/commander.o

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin
	install projgen $(DESTDIR)/$(PREFIX)/bin

clean:
	rm -f *.o deps/*/*.o projgen
