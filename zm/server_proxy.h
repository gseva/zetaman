#ifndef __SERVER_PROXY_H__
#define __SERVER_PROXY_H__

#include "zm/game_protocol.h"
#include "zm/client/signal_handler.h"
#include "zm/server/server.h"


class ServerProxy {

Server s_;

public:
  GameUpdateHandler updateHandler;

  ServerProxy();

  void jump();

  GameState getState();

  void updateState(GameState gs);


};

#endif
