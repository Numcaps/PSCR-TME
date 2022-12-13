#include "TCPServer.h"
#include <iostream>
#include "ServerSocket.h"
// #include "MessageHandler.h"
#include <signal.h>
#include <string.h>
namespace pr
{

   bool TCPServer::startServer(int port)
   {
      /**
       * Creation du serveur
       * reste en attente de connexion
       */

      ss = new ServerSocket(port);
      if (ss->getFD()==-1)
      {
         return false;
      }
      
      while (true)
      {
         /* Bloquant */
         Socket sck = ss->accept();
         if (sck.getFD() == -1)
            break;
         ConnectionHandler *ch = handler->clone();
         // chv.push_back(ch);
         threads.emplace_back(&ConnectionHandler::handleConnection, std::ref(ch), sck);
      }
      return true;
   }

   void TCPServer::stopServer()
   {
      // for (ConnectionHandler *c : chv)
      // {
      //    dynamic_cast<MessageHandler *>(c)->stopLoop();
      // }

      for (auto &t : threads)
      {
         t.join();
      }
      ss->close();
   }
}