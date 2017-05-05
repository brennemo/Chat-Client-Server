Morgan Brenner 
CS 372
Chat-Client-Server

-----------------------------------------------------------------
Compilation instructions: 

Give the server script executable permission with the following:
chmod +x chatserve

Compile the client with the following:
gcc -o chatclient chatclient.c

------------------------------------------------------------------
Running the programs: 

Run the server on one host with the following: 
./chatserve <port #>

Run the client on a second host, while chatserve is running on the 
other host, with the following: 
./chatclient <server-hostname> <port #>

------------------------------------------------------------------
Notes: 

The server is written in Python; the .py extension was removed to 
comply with the rubric.

Test environments: 
flip1.engr.oregonstate.edu
flip2.engr.oregonstate.edu 

References for chatserve:
https://docs.python.org/2/library/socket.html
https://docs.python.org/2/howto/sockets.html

References for chatclient: 
http://beej.us/guide/bgnet/