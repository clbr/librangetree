.PHONY: all clean install

PREFIX ?= /usr

prechdr = ranget.h.gch


all: $(prechdr)
	$(MAKE) -C test

$(prechdr): ranget.h
	$(CXX) $(CXXFLAGS) ranget.h

clean:
	$(MAKE) -C test clean
	$(MAKE) -C bench clean
	$(MAKE) -C doc clean
	rm -f $(prechdr)

install:
	mkdir -p $(DESTDIR)/$(PREFIX)/include
	install -m644 *.h $(DESTDIR)/$(PREFIX)/include
