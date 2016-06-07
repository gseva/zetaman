
#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <memory>

#include "zm/connection.h"
#include "zm/game_protocol.h"
#include "zm/thread.h"

class Server;

namespace zm {


class ClientProxy;

class Sender : public Thread {
Queue<proto::Game>& eventQueue_;
std::shared_ptr<zm::ProtectedSocket> clientSock_;
public:
  Sender(Queue<proto::Game>& eventQueue,
         std::shared_ptr<zm::ProtectedSocket> clientSock);
  virtual void run();
};


class Receiver : public Thread {
ClientProxy& cp_;
std::shared_ptr<zm::ProtectedSocket> clientSock_;
public:
  bool stop;
  Receiver(ClientProxy& cp, std::shared_ptr<zm::ProtectedSocket> clientSock);
  virtual void run();
};


class ClientProxy {
Server &s_;
Queue<proto::Game> eventQueue_;
std::shared_ptr<zm::ProtectedSocket> clientSock_;
Sender* sender_;
Receiver* receiver_;
public:
  explicit ClientProxy(Server& s, std::shared_ptr<zm::ProtectedSocket> sock_);
  ~ClientProxy();

  void startListening();
  void startGame();

  void updateState(zm::proto::Game gs);
  void dispatchEvent(proto::ClientEvent ce);

  std::shared_ptr<zm::ProtectedSocket> getSocket();

  zm::proto::Game getState();
};


} // zm
#endif
