TARGETS = simple x-search-before-fin period random individual identica \
		mem-too-little signed

CXXFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)
	@./run-tests.sh $(TARGETS)

$(TARGETS): ../ranget.h

clean:
	rm -f *.o $(TARGETS)
