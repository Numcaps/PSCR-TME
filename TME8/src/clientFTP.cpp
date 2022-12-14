#include "FTPServer.h"
#include "Socket.h"
int main(int argc, char const *argv[])
{
    const int ack = 1;
    int port;
    char address[15];
    char rq[11];
    char filename[256];
    pr::Socket sck;

    std::cout << "Enter IP server address : ";
    std::cin >> address;
    std::cout << "Enter server port  : ";
    std::cin >> port;

    sck.connect(std::string(address), port);
    if (sck.isOpen())
    {
        int fd = sck.getFD();

        /* Recuperation de la requete */
        std::cout << "Commands : \
                    \n - LIST \
                    \n - UPLOAD path/filename\
                    \n - DOWNLOAD ./filename\
                    \n - QUIT\
                    \n ";
        while (true)
        {
            std::cout << "Enter a request : ";
            std::cin >> rq;
            if (std::string(rq) == "QUIT")
            {
                std::cout << "Closing connection ..." << std::endl;
                write(fd, "", 0);
                break;
            }

            pr::FTPServer::FTP_REQUEST frt = pr::FTPServer::toFTP_REQUEST(rq);
            switch (frt)
            {
            case pr::FTPServer::LIST:
            {
                std::cout << "Sending LIST request to server ..." << std::endl;
                char data[512];
                explicit_bzero(data, sizeof(data));
                /* Separateur pour parsing du serveur */
                strcat(rq, " ");
                if (write(fd, rq, sizeof(rq)) == -1)
                {
                    perror("Error write LIST");
                }
                std::cout << "Receiving LIST response from server ..." << std::endl;
                int rd;

                while ((rd = read(fd, data, sizeof(data))) != 0)
                {
                    if (rd == -1)
                        perror("Error read LIST");
                    if (rd == 1 and !strcmp(data, ""))
                        break;
                    std::cout << "-" << data << std::endl;

                    explicit_bzero(data, sizeof(data));
                    /* Envoie ack pour avoir les donnees dans l'ordre */
                    if (write(fd, &ack, sizeof(ack)) == -1)
                        perror("Error write ack !");
                }
                std::cout << "LIST request completed !" << std::endl
                          << std::endl;
            }
            break;
            case pr::FTPServer::DOWNLOAD:
            {
                std::cout << "Enter filename : ";
                std::cin >> filename;
                char data[512];
                /* File descriptor du fichier a telecharger */
                int ffd;

                /* Creation de la requete */
                strcat(rq, " ");
                strcat(rq, filename);

                /* Envoie de la requete au serveur */
                std::cout << "Sending DOWNLOAD request to server ..." << std::endl;
                if (write(fd, rq, sizeof(rq)) == -1)
                {
                    perror("Error write DOWNLOAD");
                }
                if ((ffd = open(filename, O_CREAT | O_WRONLY)) == -1)
                {
                    perror("Cannot create file DOWNLOAD !");
                }

                /* Reception de la reponse du serveur */
                std::cout << "Receiving DOWNLOAD response from server ..." << std::endl;
                int rd;
                while ((rd = read(fd, data, sizeof(data))) != 0)
                {
                    if (rd == -1)
                        perror("Error read DOWNLOAD");
                    if (write(ffd, data, sizeof(data)) == -1)
                    {
                        perror("Error write DOWNLOAD");
                    }
                }
                close(ffd);
                std::cout << "DOWNLOAD request completed !" << std::endl
                          << std::endl;
            }
            break;
            case pr::FTPServer::UPLOAD:
            {
                std::cout << "Enter filename : ";
                std::cin >> filename;
                char data[512];
                /* File descriptor du fichier a televerser */
                int ffd;

                /* Creation de la requete */
                strcat(rq, " ");
                strcat(rq, filename);

                /* Envoie de la requete au serveur */
                std::cout << "Sending UPLOAD request to server ..." << std::endl;
                if (write(fd, rq, sizeof(rq)) == -1)
                {
                    perror("Error write UPLOAD");
                }
                if ((ffd = open(filename, O_RDONLY)) == -1)
                {
                    perror("Cannot read file UPLOAD !");
                }

                int rd;
                while ((rd = read(ffd, data, sizeof(data))) != 0)
                {
                    if (rd == -1)
                        perror("Error read UPLOAD");
                    if (write(fd, data, sizeof(data)) == -1)
                    {
                        perror("Error write UPLOAD");
                    }
                }
                close(ffd);
                std::cout << "UPLOAD request completed !" << std::endl
                          << std::endl;
            }
            break;
            default:
                break;
            }
        }
    }

    return 0;
}
