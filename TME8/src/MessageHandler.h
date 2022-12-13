#ifndef SRC_MESSAGEHANDLER_H_
#define SRC_MESSAGEHANDLER_H_

#include "ConnectionHandler.h"
#include <iostream>
#include <signal.h>
#include <poll.h>

namespace pr
{

    // une interface pour gerer la communication
    class MessageHandler : public ConnectionHandler
    {
        /* Compteur de client */
        static int cpt;
        /* Identifiant de client */
        int id;
        /* Condition d'arret */
        bool stop = true;

    public:
        MessageHandler() : id(cpt) { cpt++; }
        // gerer une conversation sur une socket
        virtual void handleConnection(Socket s)
        {
            int buff;
            struct pollfd fd_monitored;
            fd_monitored.fd = s.getFD();
            /* There is data to read */
            fd_monitored.events = POLLIN;

            std::cout << "Entering handler for client " << cpt
                      << std::endl;
            while (stop)
            {
                /* Attend 1 min  */
                int ret = poll(&fd_monitored, 1, 60000);
                if (ret == 0)
                {
                    std::cout << "Client " << id
                              << " not sending data for 1 min. Closing connection ..."
                              << std::endl;
                    s.close();
                }
                else if (ret == -1)
                {
                    s.close();
                    perror("Handler error !");
                }
                else
                {
                    if (fd_monitored.revents & POLLHUP)
                    {
                        std::cout << "Client " << id
                                  << " closed connection. Lefting handler ..."
                                  << std::endl;
                        return;
                    }

                    read(s.getFD(), &buff, sizeof(buff));
                    std::cout << "Client " << id << "> "
                              << buff << std::endl;
                }
            }
            std::cout << "Stopping handler of client " << id
                      << " ..."
                      << std::endl;
        }
        void stopLoop() { stop = false; }
        // une copie identique
        virtual ConnectionHandler *clone() const
        {
            return new MessageHandler();
        }
        // pour virtual
        virtual ~MessageHandler() {}
    };
    int MessageHandler::cpt = 0;
}
#endif /* SRC_CONNECTIONHANDLER_H_ */
