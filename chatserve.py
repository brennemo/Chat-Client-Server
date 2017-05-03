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
    print port_num 

    #hard code server's handle
    handle_a = "blorps"

    HOST = ''               # Symbolic name meaning all available interfaces
    PORT = port_num            
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    s.listen(1)

    conn, addr = s.accept()       

    print 'Connected by', addr
    
    #get client's handle first 
    client_message = conn.recv(10)
    handle_b =     client_message
	
	#send handle to client 
	sent = self.sock.send(handle_a)
            if sent == 0:
                raise RuntimeError("socket connection broken")

    while 1:            
        client_message = conn.recv(500)
        print handle_b + '> ' + client_message
        
        server_message = raw_input()
		sent = self.sock.send(server_message)
            if sent == 0:
                raise RuntimeError("socket connection broken")

    conn.close()
    
