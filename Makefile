SOURCES=$(wildcard *.cc)
EXE=$(basename $(SOURCES))
CXXFLAGS= -std=c++11 -Wall -D DEVELOPMENT

default: $(EXE)

clean:
	rm -f $(EXE)
