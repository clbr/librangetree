TARGETS = countspeed

CXXFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): ../ranget.h timing.h

clean:
	rm -f *.o $(TARGETS)
