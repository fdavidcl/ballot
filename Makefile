# Define source files and executable objectives
SOURCES=$(wildcard *.cc)
EXE=$(basename $(SOURCES))
CXXFLAGS= -std=c++11 -Wall

# Default objective. Make will do the rest
default: $(EXE)

clean:
	rm -f $(EXE)
