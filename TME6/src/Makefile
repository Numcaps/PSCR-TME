fork_exo: fork_exo.o
	g++ -std=c++1y -Wall -g -o fork_exo fork_exo.o
fork_exo.o: fork_exo.cpp
	g++ -std=c++1y -Wall -g -c -o fork_exo.o fork_exo.cpp
rsleep.o : rsleep.cpp
	g++ -std=c++1y -Wall -g -c -o rsleep.o rsleep.cpp
signal_wars.o: signal_wars.cpp
	g++ -std=c++1y -Wall -g -c -o signal_wars.o signal_wars.cpp
signal_wars: signal_wars.o rsleep.o
	g++ -std=c++1y -Wall -g -o signal_wars signal_wars.o rsleep.o

wait_pid: wait_pid.o
	g++ -std=c++1y -Wall -g -o wait_pid wait_pid.o
wait_pid.o: wait_pid.cpp
	g++ -std=c++1y -Wall -g -c -o wait_pid.o wait_pid.cpp
clean :
	rm *.o