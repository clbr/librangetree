.PHONY: all clean install dist

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

dist:
	VER=`git describe`; \
	git archive --prefix librangetree-$$VER/ HEAD | pigz -9 > \
	/tmp/librangetree-$$VER.tar.gz
