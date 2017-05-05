#!/usr/bin/python

# Echo server program
import socket
import sys

def start_up():
    print "start up!"

def send_message():
    print "sent message!"

def receive_message():
    print "receive message!"

if __name__ == "__main__":
    #Get port number 
    if len(sys.argv) != 2:
        print "USAGE: ./chatserve <port #>"
        exit(1)

    port_num = int(sys.argv[1])

    #hard code server's handle
    handle_a = "blorps"

    HOST = ''               # Symbolic name meaning all available interfaces
    PORT = port_num            
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    s.listen(1)

    conn, addr = s.accept()       
    
    #get client's handle first 
    client_message = conn.recv(12)
    handle_b =     client_message
    
    #send handle to client 
    sent = conn.send(handle_a)
    if sent == 0:
        raise RuntimeError("socket connection broken")

    while 1:            
        #receive message from client 
        client_message = conn.recv(500)
        client_message = client_message.strip('\x00')
        
        #check for quit command 
        if client_message == "\quit":
            print "quitting"
            break
        
        print handle_b + '> ' + client_message
        
        #send message to client
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
    
