TARGETS = simple

CXXFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)
	./run-tests.sh $(TARGETS)

$(TARGETS): ../ranget.h

clean:
	rm -f *.o $(TARGETS)
