.PHONY: all clean

name = librangetree.html
src = librangetree.txt

all: $(name)

$(name): $(src)
	asciidoc -a toc2 -b html5 -a theme=flask -a data-uri $(src)

clean:
	rm -f $(name)
