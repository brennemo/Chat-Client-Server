#!/usr/bin/python

# Morgan Brenner
# brennemo@oregonstate.edu 
# CS 372 Program 1 

import signal 
import socket
import sys
    
def start_up(argv):
	"""
	Returns: socket file descriptor 
	"""
    HOST = ''               
    PORT = int(argv[1])         
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    s.bind((HOST, PORT))
    s.listen(1)                #allow 1 connection at a time 
    return s 

     
def send_message(conn):
	"""
	Returns: 1 to quit chat, 0 to continue
	"""
    server_message = ''
    sys.stdout.write(handle_a + '> ')
    server_message = raw_input()
    sent = conn.send(server_message)
    if sent == 0:
        raise RuntimeError("socket connection broken")
        
    #check for quit command 
    if server_message == "\quit":
        return 1
        
    return 0 


def receive_message(conn):
	"""
	Returns: 1 to quit chat, 0 to continue
	"""
    client_message = ''
    client_message = conn.recv(501)
    
    #check for quit command 
    test_quit = client_message.strip('\x00')
    if test_quit == "\quit":
        return 1 
    
    print handle_b + '> ' + client_message
    return 0 


def sigint_handler(signal, frame):
	"""
	sigint handler for closing server program cleanly 
	"""
    print "\nexiting chatserve"
    sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler)
    
if __name__ == "__main__":
    #Get port number 
    if len(sys.argv) != 2:
        print "USAGE: ./chatserve <port #>"
        exit(1)

    #hard code server's handle
    handle_a = "blorps"

    #set up socket, bind, and start listening 
    s = start_up(sys.argv)

    #accept consecutive connections
    while 1: 
        print "waiting for client connection on " + sys.argv[1]
        conn, addr = s.accept()       
        
        #get client's handle first 
        client_message = conn.recv(11)
        handle_b =     client_message
        
        #send handle to client 
        sent = conn.send(handle_a)
        if sent == 0:
            raise RuntimeError("socket connection broken")

        #chat with client  
        while 1:    
            #receive message from client 
            if receive_message(conn) == 1: 
                print "ending chat"
                break
            
            #send message to client
            if send_message(conn) == 1:
                print "ending chat"
                break 

        conn.close()
    
