#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

#include <vector>
#include <string>

#include "zm/game_protocol.h"
#include "zm/client/signal_handler.h"
#include "zm/server/server.h"


class ServerProxy {
Server s_;

public:
  GameUpdateHandler updateHandler;

  ServerProxy();
  void connect();
  void startLevel();

  void jump();
  void moveRight();
  void moveLeft();
  void stopHorizontalMove();
  void up();
  void shoot();

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

  zm::proto::Game getState();

  void updateState(zm::proto::Game g);
};

#endif
