all: main

main: server.o
	g++ -g -pthread -lrt server.o -o server


server.o: server.cpp
	g++ -c -g -pthread -lrt server.cpp

clean:
	rm -f *.o server
