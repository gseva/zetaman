#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

#include "game_protocol.h"

class Client;

class ServerProxy {

// Server s_;
Client* c_;

public:
  ServerProxy(Client* c);

  void jump();

  void setState(GameState state);

};

#endif
