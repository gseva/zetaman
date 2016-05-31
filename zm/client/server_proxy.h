#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

#include <vector>
#include <string>

#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/connection.h"
#include "zm/json/jsonserializer.h"
#include "zm/client/signal_handler.h"

namespace zm {


class Sender : public Thread {
Queue<proto::ClientEvent>& eventQueue_;
Socket& serverSock_;
public:
  Sender(Queue<proto::ClientEvent>& eventQueue, Socket& serverSock);
  virtual void run();
};


class ServerProxy {
// Server s_;
Queue<proto::ClientEvent> eventQueue_;
Socket serverSock_;
Sender* sender_;
JsonMap map_;

public:
  GameUpdateHandler updateHandler;

  ServerProxy();
  ~ServerProxy();

  void connect();
  void startLevel();
  void shutdown();

  void jump();
  void moveRight();
  void moveLeft();
  void stopHorizontalMove();
  void up();
  void shoot();

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

  proto::Game getState();

  void updateState(zm::proto::Game g);
};

} // zm

#endif
