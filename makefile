# Declaration of variables
FLAGS = -Iinc -Lobj -pthread -std=c++11
 
# File names
EXEC = output
SOURCES=$(wildcard src/*.cc)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cc=.o)))
 
#all:
#	@echo $(OBJECTS)
#	@echo $(SOURCES)

# Main target
output : main.o obj/physical_state.o obj/simulation.o obj/analyser.o
	g++ $(FLAGS) $(OBJECTS) main.o -o output
 
main.o : main.cc
	g++ -c $(FLAGS) -o main.o main.cc

obj/physical_state.o : src/physical_state.cc
	g++ -c $(FLAGS) -o $@ $^

obj/simulation.o : src/simulation.cc
	g++ -c $(FLAGS) -o $@ $^

obj/analyser.o : src/analyser.cc inc/analyser.h
	g++ -c $(FLAGS) -o $@ src/analyser.cc

# To obtain object files
#obj/%.o: src/%.cpp
#	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
#clean:
#	rm -f $(EXEC) $(OBJECTS)	
