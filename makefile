# Declaration of variables
FLAGS = -Iinc -Lobj -pthread -std=c++11 -O3
 
# File names
EXEC = exec
SOURCES=$(wildcard src/*.cc)
OBJECTS=$(addprefix obj/,$(notdir $(SOURCES:.cc=.o)))
 
#all:
#	@echo $(OBJECTS)
#	@echo $(SOURCES)

# Main target
exec : $(OBJECTS) obj/main.o
	g++ $(FLAGS) $(OBJECTS) obj/main.o -o exec
 
obj/main.o : main.cc
	g++ -c $(FLAGS) -o obj/main.o main.cc

obj/physical_state.o : src/physical_state.cc inc/physical_state.h
	g++ -c $(FLAGS) -o $@ src/physical_state.cc

obj/simulation.o : src/simulation.cc inc/simulation.h
	g++ -c $(FLAGS) -o $@ src/simulation.cc

obj/analyser.o : src/analyser.cc inc/analyser.h
	g++ -c $(FLAGS) -o $@ src/analyser.cc

obj/manager.o : src/manager.cc inc/manager.h
	g++ -c $(FLAGS) -o $@ src/manager.cc

obj/singlemanager.o : src/singlemanager.cc inc/singlemanager.h
	g++ -c $(FLAGS) -o $@ src/singlemanager.cc

# To obtain object files
#obj/%.o: src/%.cpp
#	$(CC) -c $(CC_FLAGS) $< -o $@
 

