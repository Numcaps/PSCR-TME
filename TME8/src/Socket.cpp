#include "Socket.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
namespace pr
{
    std::ostream &operator<<(std::ostream &os, struct sockaddr_in *addr)
    {
        char host[1024];
        getnameinfo((struct sockaddr *)addr, sizeof(addr), host, sizeof(host), NULL, 0, NI_NUMERICHOST);
        return os << host;
    }

    void Socket::connect(const std::string &host, int port)
    {
        /* Resolution nom */
        const char *name = host.c_str();
        struct addrinfo hints;
        struct addrinfo *res,* pp;

        /**
         * Man impose de set a zero tt les
         * autres champs de addrinfo, cf man getnameinfo
         */
        memset(&hints, 0, sizeof hints);

        /* IPV4 */
        hints.ai_family = AF_INET;
        /* socket TCP */
        hints.ai_socktype = SOCK_STREAM;

        /**
         * On recupere l'adresse associe au nom
         * res est une liste
         * hint contient une cellule de cette liste
         * qui correspond a la config qu'on cherche
         */
        if (getaddrinfo(name, nullptr, &hints, &res) == -1)
        {
            perror("getaddrinfo() failed !");
        }
        /**
         * On parocurs la liste
         */
        for (pp = res; pp != NULL; pp = pp->ai_next)
        {
            /**
             * Creation d'une socket TCP
             * bind() non necessaire sauf si client
             * a besoin d'un port de communication
             * precis
             */
            if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            {
                continue;
            }
            ((struct sockaddr_in *)(pp->ai_addr))->sin_port = htons(port);
            /**
             * Initialisation de la connexion
             * Appel bloquant
             */
            if (::connect(fd, pp->ai_addr, pp->ai_addrlen) != -1)
            {
                break;
            }
            close();
        }
        /**
         * Liberation de la memoire allouee pour res
         */
        freeaddrinfo(res);
        if(fd==-1) perror("Failed to connect !");
        
    }

    void Socket::connect(in_addr ipv4, int port)
    {
        struct sockaddr_in adr;
        memset(&adr, 0, sizeof(struct sockaddr_in));
        adr.sin_family = AF_INET;
        adr.sin_port = htons(port);
        adr.sin_addr = ipv4;
        /**
         * Creation d'une socket TCP
         */
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("Failed to create socket !");
        }
        /**
         * Initialisation de la connexion
         * Appel bloquant
         */
        if (::connect(fd, (sockaddr *)&adr, sizeof(adr)) == -1)
        {
            perror("Connexion failed!");
        }
    }

    void Socket::close()
    {
        /**
         * Fermeture de la connexion full-duplex
         */
        shutdown(fd, SHUT_RDWR);

        /**
         * Destruction du file descriptor
         */
        ::close(fd);
        fd=-1;
    }

} // namespace pr
