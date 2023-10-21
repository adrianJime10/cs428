Author: Adrian Jimenez

Program: Assignment #2 TCP (1 Server, 2 Clients)

How to run:
Make sure you are in the directory with the cpp file for the executable you wish to make (server for 
server, client for client) and type 'make'. Run the server first in a separate terminal using './server'
and then run the client using './client'. Use 'make clean' to clean up the executable and object files.

Design Choices:
I decided to use pthreads for both the server and the client. The server used two threads, one per
connection established, so that it make handle each request seperately and simultaneously. The client
used two threads as well so that it may, again, send two different messages separately and simultaneously
in order to mimic two clients interacting with a single server.

Possible failure points:
If any of the syscalls fail or if the message is too large to fit in the buffer, the code may crash.

Possible improvements:
Implement error handling to make sure the incoming message is small enough and to check for failures
in the syscalls.

Implement a resizeable array at both the server and client.
