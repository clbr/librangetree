.PHONY: all clean install

PREFIX ?= /usr

all:
	$(MAKE) -C test

clean:
	$(MAKE) -C test clean

install:
	mkdir -p $(DESTDIR)/$(PREFIX)/include
	install -m644 *.h $(DESTDIR)/$(PREFIX)/include
