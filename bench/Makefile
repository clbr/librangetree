TARGETS = countspeed scaling mil

CXXFLAGS += -Wall -Wextra
CPPFLAGS += -I ..

.PHONY: all clean

all: $(TARGETS)

scaling: CXXFLAGS += -fopenmp
scaling: LDFLAGS += -fopenmp

$(TARGETS): ../ranget.h timing.h

clean:
	rm -f *.o $(TARGETS)
