COMMAND = g++
FLAGS = -W -Wall -ansi -pedantic -g -std=c++1y 
INPUTS = main.cpp

main: main.cpp utils.hpp
	$(COMMAND) $(FLAGS) -o main main.cpp 

mainMap: mainMap.cpp
	$(COMMAND) $(FLAGS) -o mainMap  mainMap.cpp
	
mainHashTable: mainHashTable.o  HashTable.hpp
	$(COMMAND) $(FLAGS) -o mainHashTable  mainHashTable.o 

mainHashTable.o : mainHashTable.cpp HashTable.hpp
	$(COMMAND) $(FLAGS) -c mainHashTable.cpp -o mainHashTable.o 

clean:
	rm main mainHashTable *.o