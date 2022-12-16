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
        FTPServer(ConnectionHandler *ch, char *);
        void startServer(int port);
        void stopServer();
        DIR *getDir();
        static const char *toString(FTP_REQUEST fr);
        static FTP_REQUEST toFTP_REQUEST(const char *s);
    };
    /*==================================================================*/
    class FTPConnectionHandler : public ConnectionHandler
    {
        DIR *dir;
        char *path;

    public:
        void setDir(DIR *d) { dir = d; }
        void setPath(char *p)
        {
            path = p;
        }
        virtual void handleConnection(Socket s)
        {
            int sync;
            char end = '\0';
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
                std::cout << "Waiting for client request ..." << std::endl;
                auto nblu = read(fd, client_buff, msz);
                if (nblu == 0)
                {
                    std::cout << "Fin connexion par client" << std::endl;
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
                     *  ou qui va etre upload(besoin pour creer fichier)
                     */
                    strcpy(filename, client_buff + i);

                    /* Determination de la requete */
                    fq = FTPServer::toFTP_REQUEST(rq);
                    switch (fq)
                    {
                    case FTPServer::LIST:
                    {
                        std::cout << "LIST request received !" << std::endl;
                        std::cout << "Sending list file ..." << std::endl;
                        /* Parcours des entrees du directory */
                        rewinddir(dir);
                        struct dirent *dir_l;
                        while ((dir_l = readdir(dir)) != nullptr)
                        {
                            // std::cout << "[DEBUG] LIST SERVER : dir_l=" << dir_l->d_name << std::endl;
                            /* Standard recommande d'utiliser strlen pour d_name*/
                            if (write(fd, dir_l->d_name, strlen(dir_l->d_name)) == -1)
                            {
                                perror("Error write LIST");
                            }
                            /* Boucle de synchro */
                            int r;
                            while ((r = read(fd, &sync, sizeof(sync))) != 0)
                            {
                                if (r == -1)
                                    perror("Error read sync !");
                                else if (sync == 1)
                                    break;
                            }
                        }

                        if (write(fd, &end, sizeof(end)) == -1)
                        {
                            perror("Error write end LIST");
                        }
                    }
                    break;
                    case FTPServer::DOWNLOAD:
                    {
                        std::cout << "DOWNLOAD request received !" << std::endl;
                        std::cout << "Sending file " << filename
                                  << " ..." << std::endl;
                        /* Recherche du fichier dans directory */
                        rewinddir(dir);
                        struct dirent *dir_d;
                        while (true)
                        {
                            if ((dir_d = readdir(dir)) != nullptr)
                            {
                                /* Le fichier existe ! */
                                if (!strcmp(filename, dir_d->d_name))
                                {
                                    /* Ouverture du fichier */
                                    int fdf;
                                    char fp[256];
                                    memset(fp, 0, sizeof(fp));
                                    strcat(fp, path);
                                    strcat(fp, "/");
                                    strcat(fp, dir_d->d_name);

                                    if ((fdf = open(fp, O_RDONLY)) == -1)
                                    {
                                        perror("Unable to open file DOWNLOAD");
                                    }
                                    char data[128];
                                    memset(data, 0, sizeof(data));
                                    /* On envoie des paquets de 128 octets*/
                                    int rd;
                                    while ((rd = read(fdf, data, sizeof(data))) != 0)
                                    {
                                        if (rd == -1)
                                            perror("Error read DOWNLOAD !");
                                        if (write(fd, data, sizeof(data)) != -1)
                                        {
                                            perror("Error write DOWNLOAD");
                                        }
                                    }
                                    /* Fermeture du fichier */
                                    close(fdf);
                                    break;
                                }
                            }
                            else
                            {
                                /* Fichier n'existe pas */
                                std::cout << "File not found in directory, sending nothing ..." << std::endl;
                                write(fd, "", 0);
                                break;
                            }
                        }
                        if (write(fd, &end, sizeof(end)) == -1)
                        {
                            perror("Error write end LIST");
                        }
                    }
                    break;
                    case FTPServer::UPLOAD:
                    {
                        std::cout << "UPLOAD request received !" << std::endl;

                        FILE *fdf;
                        char p[128];
                        memset(p,0,sizeof(p));
                        strcat(p,path);
                        strcat(p,"/");
                        strcat(p,filename);
                        if ((fdf = fopen(p, "w")) == nullptr)
                        {
                            perror("Cannot create file UPLOAD !");
                        }
                        char data[128];
                        int rd;
                        std::cout << "Receiving file " << filename
                                  << " ..." << std::endl;
                        while ((rd = read(fd, data, sizeof(data))) != 0)
                        {
                            if (rd == -1)
                                perror("Error read UPLOAD");
                            if (!strcmp(data, ""))
                                break;
                            fwrite(data, sizeof(char), strlen(data), fdf);
                        }
                        fclose(fdf);
                    }
                    break;
                    default:
                        break;
                    }
                }
            }
        }
        // une copie identique
        virtual ConnectionHandler *clone() const
        {
            auto f = new FTPConnectionHandler();
            f->setDir(dir);
            f->setPath(path);
            return f;
        }
    };

    /*==================================================================*/
    FTPServer::FTPServer(ConnectionHandler *ch, char *d)
    {

        if ((dir = opendir(d)) == nullptr)
        {
            perror("opendir error !");
        }
        FTPConnectionHandler *h;
        if ((h = dynamic_cast<FTPConnectionHandler *>(ch)) != nullptr)
        {
            h->setDir(dir);
            h->setPath(d);
        }
        else
            perror("Handler error ! ");

        serv = new TCPServer(ch);
    }

    void FTPServer::startServer(int port)
    {
        serv->startServer(port);
    }

    void FTPServer::stopServer()
    {
        std::cout << "Closing server ..." << std::endl;
        serv->stopServer();
    }

    const char *FTPServer::toString(FTP_REQUEST fr)
    {
        switch (fr)
        {
        case LIST:
            return std::string("LIST").c_str();
        case UPLOAD:
            return std::string("UPLOAD").c_str();
        case DOWNLOAD:
            return std::string("DOWNLOAD").c_str();
        default:
            perror("FTP_Server toString error !");
            return nullptr;
        }
    }

    FTPServer::FTP_REQUEST FTPServer::toFTP_REQUEST(const char *s)
    {
        std::string st(s);
        if (s == std::string("LIST"))
        {
            return LIST;
        }
        if (s == std::string("UPLOAD"))
        {
            return UPLOAD;
        }
        if (s == std::string("DOWNLOAD"))
        {
            return DOWNLOAD;
        }
        else
        {
            perror("toFTP_REQUEST error,  return LIST !");
            return LIST;
        }
    }

    DIR *FTPServer::getDir()
    {
        return dir;
    }

} // namespace pr

#endif