#!/usr/bin/env python

# Echo server program
import socket
import sys
import SocketServer

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        print "%s wrote:" % self.client_address[0]
        print self.data
        # just send back the same data, but upper-cased
        self.request.send(self.data.upper())

if __name__ == "__main__":
    #Get port number 
    if len(sys.argv) != 2:
        print "USAGE: ./chatserve <port #>"
        exit(1)

    port_num = sys.argv[1]

    #hard code handle
    handle_a = "blorps"

    HOST = ''               # Symbolic name meaning all available interfaces
    PORT = port_num            
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	
	s.bind((HOST, PORT))
	s.listen(1)
	
	conn, addr = s.accept()

	print 'Connected by', addr
	while 1:
		data = conn.recv(1024)
		if not data: break
		conn.sendall(data)
	conn.close()


    #HOST, PORT = "localhost", port_num

    # Create the server, binding to localhost on port 9999
    #server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    #server.serve_forever()

