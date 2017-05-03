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

    #hard code handle
    handle_a = "blorps"

    HOST = ''               # Symbolic name meaning all available interfaces
    PORT = port_num            
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    s.listen(1)

    conn, addr = s.accept()		#hangs here 

    print 'Connected by', addr

    while 1:
	    client_message = conn.recv(500)
	    print client_message
		#data = conn.recv(1024)
        #if not data: break
        #conn.sendall(data)

    conn.close()
	
