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

class Client;
class ServerProxy;

class Sender : public Thread {
Queue<proto::ClientEvent>& eventQueue_;
ProtectedSocket& serverSock_;
public:
  Sender(Queue<proto::ClientEvent>& eventQueue, ProtectedSocket& serverSock);
  virtual void run();
};


class Receiver : public Thread {
ServerProxy& sp_;
ProtectedSocket& serverSock_;
public:
  bool stop;
  Receiver(ServerProxy& sp, ProtectedSocket& serverSock);
  virtual void run();
};



class ServerProxy {
Queue<proto::ClientEvent> eventQueue_;
ProtectedSocket serverSock_;
Sender* sender_;
Receiver* receiver_;
JsonMap map_;

void getConnection_();

public:
  GameUpdateHandler updateHandler;
  bool isHost;

  ServerProxy();
  ~ServerProxy();

  void connect();
  void getMap();
  void startLevel();
  void shutdown();

  void jump();
  void moveRight();
  void moveLeft();
  void stopHorizontalMove();
  void up();
  void shoot();

  void selectLevel(int level);

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

  proto::Game getState();

  void updateState(zm::proto::Game g);
};

} // zm

#endif