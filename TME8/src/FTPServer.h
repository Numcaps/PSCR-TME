#ifndef SRC_FTPSERVER_H_
#define SRC_FTPSERVER_H_
#include "TCPServer.h"
#include "ConnectionHandler.h"
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace pr
{
    class FTPConnectionHandler : public ConnectionHandler
    {
        DIR *dir;

    public:
        FTPConnectionHandler(DIR *d) : dir(d){};
        virtual void handleConnection(Socket s)
        {
            /* Recupere le file descriptor */
            int fd = s.getFD();

            /* Buffer message du client */
            char client_buff[256];
            memset(client_buff, 0, sizeof(client_buff));

            /* Taille du buffer client */
            ssize_t msz = sizeof(client_buff);

            /* Boucle de controle */
            while (1)
            {
                auto nblu = read(fd, client_buff, msz);
                if (nblu == 0)
                {
                    std::cout << "Fin connexion par client" << std::endl;
                    break;
                }
                else if (nblu < msz)
                {
                    perror("FTPServer handler read error !");
                    break;
                }
                else
                {
                    /* Recuperation de la requete */
                    enum FTPServer::FTP_REQUEST fq;
                    char rq[9];
                    char filename[256];
                    memset(rq, 0, sizeof(rq));
                    memset(filename, 0, sizeof(filename));
                    int i;
                    for (i = 0; i < 256; i++)
                    {
                        if (client_buff[i] == ' ')
                        {
                            break;
                        }
                        rq[i] = client_buff[i];
                    }
                    i++;

                    /**
                     *  Recuperation speculative du fichier a download
                     *  ou qui va etre upload 
                    */
                    strcpy(filename, rq + i);

                    /* Determination de la requete */
                    fq = FTPServer::toFTP_REQUEST(rq);
                    switch (fq)
                    {
                    case FTPServer::LIST:
                        std::cout << "Sending list file ..." << std::endl;
                        /* Parcours des entrees du directory */
                        struct dirent *dir_list;
                        while ((dir_list = readdir(dir)) != nullptr)
                        {
                            /* Standard recommande d'utiliser strlen pour d_name*/
                            write(fd, dir_list->d_name, strlen(dir_list->d_name));
                        }
                        break;
                    case FTPServer::DOWNLOAD:
                        std::cout << "Sending file " << filename
                                  << " ..." << std::endl;
                        /* Recherche du fichier dans directory */
                        struct dirent *dir_list;
                        while ((dir_list = readdir(dir)) != nullptr)
                        {
                            /* Le fichier existe ! */
                            if (!strcmp(filename, dir_list->d_name))
                            {
                                /* Ouverture du fichier */
                                int fdf = open(dir_list->d_name, O_RDONLY);
                                char c[512];
                                memset(c, 0, sizeof(c));
                                /* On envoie des paquets de 512 octets*/
                                while (read(fdf, c, sizeof(c)) != 0)
                                {
                                    write(fd, c, sizeof(c));
                                }
                                /* Fermeture du fichier */
                                close(fdf);
                                break;
                            }
                        }
                        perror("Error file do not exist !");
                    case FTPServer::UPLOAD:
                        std::cout << "Sending file " << filename
                                  << " ..." << std::endl;
                    default:
                        break;
                    }
                }
            }
        }
        // une copie identique
        virtual ConnectionHandler *clone() const
        {
            return new FTPConnectionHandler(dir);
        }
    };

    /*==================================================================*/
    class FTPServer
    {
        TCPServer *serv;
        /* repertoire ou mettre les fichiers du client */
        DIR *dir;

    public:
        enum FTP_REQUEST
        {
            LIST = 0,
            UPLOAD,
            DOWNLOAD
        };
        FTPServer(ConnectionHandler *ch, const char *d);
        void startServer(int port);
        void stopServer();
        DIR *getDir();
        static char *FTPServer::toString(FTP_REQUEST fr);
        static FTP_REQUEST FTPServer::toFTP_REQUEST(const char *s);

        ~FTPServer();
    };
    /*==================================================================*/
    FTPServer::FTPServer(ConnectionHandler *ch, const char *d)
    {

        if ((dir = opendir(d)) == nullptr)
        {
            perror("opendir error !");
        }

        serv = new TCPServer(ch);
    }

    void FTPServer::startServer(int port)
    {
        serv->startServer(port);
    }

    void FTPServer::stopServer()
    {
        serv->stopServer();
    }

    char *FTPServer::toString(FTP_REQUEST fr)
    {
        switch (fr)
        {
        case LIST:
            return "LIST";
        case UPLOAD:
            return "UPLOAD";
        case DOWNLOAD:
            return "DOWNLOAD";
        default:
            perror("FTP_Server toString error !");
            break;
        }
    }

    FTPServer::FTP_REQUEST FTPServer::toFTP_REQUEST(const char *s)
    {
        std::string st(s);
        if (s == "LIST")
        {
            return LIST;
        }
        if (s == "UPLOAD")
        {
            return UPLOAD;
        }
        if (s == "DOWNLOAD")
        {
            return DOWNLOAD;
        }
        else
        {
            perror("toFTP_REQUEST error !");
        }
    }

    DIR *FTPServer::getDir()
    {
        return dir;
    }

} // namespace pr

#endif