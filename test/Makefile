TARGETS = simple

CXXFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): ../ranget.h

clean:
	rm -f *.o $(TARGETS)
