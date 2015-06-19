# Define source files and executable objectives
SOURCES=$(wildcard *.cc)
EXE=$(basename $(SOURCES))
CXXFLAGS= -std=c++11 -Wall

# Default objective. Make will do the rest
default: $(EXE)

results: results1.csv results2.csv results3.csv

%.csv: $(EXE)
	sudo ./$(EXE) > $@

clean:
	rm -f $(EXE)
