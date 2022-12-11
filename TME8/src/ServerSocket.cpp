#include "ServerSocket.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "Socket.h"

namespace pr
{
    ServerSocket::ServerSocket(int port)
    {
        struct sockaddr_in addr;
        in_addr saddr;
        saddr.s_addr = INADDR_ANY;

        memset(&addr, 0, sizeof(addr));
        addr.sin_addr = saddr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        /**
         * Creation d'une socket TCP
         *
         */
        std::cout << "Connection initialization ..." << std::endl;
        if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Failed to create socket !");
        }

        std::cout << "Binding ..." << std::endl;
        /**
         * Association du processus serveur
         * au port
         */
        if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        {
            perror("Failed to bind the socket !");
        }

        std::cout << "Listening on port " << port << "..." << std::endl;
        /**
         * Ecoute sur le port
         */
        if (listen(socketfd, 1) == -1)
        {
            perror("Failed to listen for connection !");
        }
        std::cout << "Establishing connection ..." << std::endl;
    }

    Socket ServerSocket::accept()
    {
        int fd;
        /**
         * sockaddr of client
         */
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        /**
         * Acceptation de la connexion
         */
        
        if ((fd = ::accept(socketfd, (struct sockaddr *)(&client), &len)) == -1)
        {
            perror("Failed to accept the connexion !");
        }
        std::cout << "Connected to "<<&client<<std::endl;
        return Socket(fd);
    }

    void ServerSocket::close()
    {
        /**
         * Fermeture de la connexion full-duplex
         */
        shutdown(socketfd, SHUT_RDWR);

        /**
         * Destruction du file descriptor
         */
        ::close(socketfd);
    }
} // namespace pr
