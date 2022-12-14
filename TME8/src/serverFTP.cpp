#include "FTPServer.h"

using namespace pr;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        perror("Too few arguments !");
        return 1;
    }

    FTPServer server(new FTPConnectionHandler(), argv[2]);

    int port = atoi(argv[1]);
    std::thread t(&pr::FTPServer::startServer, std::ref(server), port);

    // server.startServer(1664);

    // attend entree sur la console
    std::string s;
    std::cin >> s;

    // on quit
    server.stopServer();
    t.join();

    return 0;
}
