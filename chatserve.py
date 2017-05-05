#!/usr/bin/python

# Morgan Brenner
# brennemo@oregonstate.edu 
# CS 372 Program 1 

import socket
import sys

def start_up():
    print "start up!"
    

def send_message():
    print "send message!"


#Returns 1 to quit chat, 0 to continue 
def receive_message(conn):
    client_message = ''
    client_message = conn.recv(501)
    
    #check for quit command 
    test_quit = client_message.strip('\x00')
    if test_quit == "\quit":
        print "quitting"
        return 1 
        #break
    
    print handle_b + '> ' + client_message
    return 0 

if __name__ == "__main__":
    #Get port number 
    if len(sys.argv) != 2:
        print "USAGE: ./chatserve <port #>"
        exit(1)

    port_num = int(sys.argv[1])

    #hard code server's handle
    handle_a = "blorps"

    HOST = ''               
    PORT = port_num            
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    s.listen(1)                #allow 1 connection at a time 

    conn, addr = s.accept()       
    
    #get client's handle first 
    client_message = conn.recv(11)
    handle_b =     client_message
    
    #send handle to client 
    sent = conn.send(handle_a)
    if sent == 0:
        raise RuntimeError("socket connection broken")

    while 1:    
        """    
        #receive message from client 
        client_message = ''
        client_message = conn.recv(501)
        
        #check for quit command 
        test_quit = client_message.strip('\x00')
        if test_quit == "\quit":
            print "quitting"
            break
        
        print handle_b + '> ' + client_message
        """
        if receive_message(conn) == 1: 
            break
        
        #send message to client
        server_message = ''
        sys.stdout.write(handle_a + '> ')
        server_message = raw_input()
        sent = conn.send(server_message)
        if sent == 0:
            raise RuntimeError("socket connection broken")
            
        #check for quit command 
        if server_message == "\quit":
            print "quitting"
            break

    conn.close()
    
