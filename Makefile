Q = @
CC = g++
CXXVERSION = -std=c++1z

PACKAGES = glm
OUTFILE = main

INCS = -Iinclude
LIBS =
LDFLAGS = -Wl,--rpath=./
WARNINGS = -w
CFLAGS = -mtune=native -O2 $(CXXVERSION) -g -pipe $(LDFLAGS) $(WARNINGS)
CFLAGS += $(INCS) $(LIBS)
CFLAGS += $(shell pkg-config $(PACKAGES) --cflags --libs)

HEADERS = $(wildcard *.h src/*.hpp)
CFILES = $(wildcard *.cpp src/*.c)
OBJECTS = $(notdir $(CFILES:.cpp=.o))


%.o: %.cpp $(HEADERS)
	@echo "	CC $<"
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

all: $(OBJECTS)
	@echo "	LD $(OUTFILE)"
	$(Q)$(CC) $^ -o $(OUTFILE) $(CFLAGS)

.PHONY: clean

clean:
	rm build/*.o
