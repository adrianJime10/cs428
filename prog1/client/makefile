all: main

main: client.o
	g++ -g -pthread -lrt client.o -o client


server.o: server.cpp
	g++ -c -g -pthread -lrt client.cpp

clean:
	rm -f *.o client
