#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

#include "game_protocol.h"
#include "server/server.h"

class Client;

class ServerProxy {

Server s_;

public:
  ServerProxy();

  void jump();

  GameState getState();

};

#endif
