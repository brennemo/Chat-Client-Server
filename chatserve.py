#!/usr/bin/python

# Morgan Brenner
# brennemo@oregonstate.edu 
# CS 372 Program 1 

import socket
import sys

def start_up(port_num):
    print "start up!"
    HOST = ''               
    PORT = port_num            
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    return s 
    #s.bind((HOST, PORT))
    #s.listen(1)                #allow 1 connection at a time 
    
#Returns 1 to quit chat, 0 to continue 
def send_message(conn):
    server_message = ''
    sys.stdout.write(handle_a + '> ')
    server_message = raw_input()
    sent = conn.send(server_message)
    if sent == 0:
        raise RuntimeError("socket connection broken")
        
    #check for quit command 
    if server_message == "\quit":
        print "quitting"
        return 1
        
    return 0 


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
    #s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s = start_up(port_num)

    s.bind((HOST, PORT))
    s.listen(1)                #allow 1 connection at a time 

    #outer while 1 loop starts here for consecutive connections 
    conn, addr = s.accept()       
    
    #get client's handle first 
    client_message = conn.recv(11)
    handle_b =     client_message
    
    #send handle to client 
    sent = conn.send(handle_a)
    if sent == 0:
        raise RuntimeError("socket connection broken")

    #chat 
    while 1:    
        #receive message from client 
        if receive_message(conn) == 1: 
            break
        
        #send message to client
        if send_message(conn) == 1:
            break 

    conn.close()
    
