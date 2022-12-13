#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"

namespace pr {

class ServerSocket {
	int socketfd;
	const int MAX_CONN = 10;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port);

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}
	int getMaxConn() const {return MAX_CONN;}
	Socket accept();

	void close();
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */
