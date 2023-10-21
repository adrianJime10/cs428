Author: Adrian Jimenez

Program: Assignment #1 UDP Pinger (Client and Server)

How to run: 
Make sure you are in the directory with the cpp file for the executable you wish to make (server for 
server, client for client) and type 'make'. Run the server first in a separate terminal using './server'
and then run the client using './client'. Use 'make clean' to clean up the executable and object files.

Design choices:
I decided to use pthreads every time I sent a message to the server so that I may simply use the 
pthreads conditional timer as a timeout if a packet was lost. The thread was killed as soon as its function completed (sending a message) or it timed out. I also decided to use chrono as a timer to
time the RTT.

Possible Failure point:
This code may fail if the message sent by the client to the server ever exceeds 1024 bytes.

Possible Improvements:
This code may be improved by allowing the user to type a custom message into the client before sending
the message to the server.

This code may also be improved by making the buffer a resizeable array in the client and the server.
