Socket.o: Socket.cpp
	g++ -Wall -g -std=c++1y -c -o Socket.o Socket.cpp
ServerSocket.o: ServerSocket.cpp
	g++ -Wall -g -std=c++1y -c -o ServerSocket.o ServerSocket.cpp

client: client.o ServerSocket.o Socket.o
	g++ -Wall -g -std=c++1y -o client client.o ServerSocket.o Socket.o

client.o : client.cpp 
	g++ -Wall -g -std=c++1y -c -o client.o client.cpp

server: server.o ServerSocket.o Socket.o
	g++ -Wall -g -std=c++1y -o server server.o ServerSocket.o Socket.o

server.o : server.cpp 
	g++ -Wall -g -std=c++1y -c -o server.o server.cpp

server2: server2.o ServerSocket.o Socket.o TCPServer.o
	g++ -Wall -g -std=c++1y -o server2 server2.o ServerSocket.o Socket.o TCPServer.o

server2.o : server2.cpp 
	g++ -Wall -g -std=c++1y -c -o server2.o server2.cpp

TCPServer.o: TCPServer.cpp
	g++ -Wall -g -std=c++1y -c -o TCPServer.o TCPServer.cpp

serverFTP: serverFTP.o FTPServer.h ServerSocket.o Socket.o TCPServer.o
	g++ -Wall -g -std=c++1y -o serverFTP serverFTP.o ServerSocket.o Socket.o TCPServer.o
serverFTP.o : serverFTP.cpp FTPServer.h
	g++ -Wall -g -std=c++1y -c -o serverFTP.o serverFTP.cpp

clientFTP: clientFTP.o FTPServer.h ServerSocket.o Socket.o TCPServer.o
	g++ -Wall -g -std=c++1y -o clientFTP clientFTP.o ServerSocket.o Socket.o TCPServer.o 
clientFTP.o : clientFTP.cpp FTPServer.h
	g++ -Wall -g -std=c++1y -c -o clientFTP.o clientFTP.cpp
FTP : clientFTP serverFTP

clean:
	rm *.o client server server2 serverFTP clientFTP