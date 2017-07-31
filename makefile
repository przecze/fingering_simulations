# Declaration of variables
FLAGS = -Iinc -Lobj -pthread -std=c++11
 
# File names
EXEC = exec
SOURCES=$(wildcard src/*.cc)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cc=.o)))
 
#all:
#	@echo $(OBJECTS)
#	@echo $(SOURCES)

# Main target
exec : main.o obj/physical_state.o obj/simulation.o obj/analyser.o obj/manager.o
	g++ $(FLAGS) $(OBJECTS) main.o -o exec
 
main.o : main.cc
	g++ -c $(FLAGS) -o main.o main.cc

obj/physical_state.o : src/physical_state.cc
	g++ -c $(FLAGS) -o $@ $^

obj/simulation.o : src/simulation.cc inc/simulation.h
	g++ -c $(FLAGS) -o $@ src/simulation.cc

obj/analyser.o : src/analyser.cc inc/analyser.h
	g++ -c $(FLAGS) -o $@ src/analyser.cc

obj/manager.o : src/manager.cc inc/manager.h
	g++ -c $(FLAGS) -o $@ src/manager.cc

# To obtain object files
#obj/%.o: src/%.cpp
#	$(CC) -c $(CC_FLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f main.o	
