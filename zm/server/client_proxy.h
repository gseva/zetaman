
#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include <memory>
#include <string>

#include "zm/connection.h"
#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/server/player.h"

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
Player* player_;

Queue<proto::Game> eventQueue_;
std::shared_ptr<zm::ProtectedSocket> clientSock_;
Sender* sender_;
Receiver* receiver_;
public:
  explicit ClientProxy(Player* p,
                       std::shared_ptr<zm::ProtectedSocket> sock_);
  void shutdown();

  void startListening();
  void sendMap(const std::string& map);
  void startGame();
  void sendLevelWon();

  void updateState(zm::proto::Game gs);
  void dispatchEvent(proto::ClientEvent ce);

  std::shared_ptr<zm::ProtectedSocket> getSocket();
};


} // zm
#endif
