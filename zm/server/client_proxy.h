
#ifndef __CLIENT_PROXY_H__
#define __CLIENT_PROXY_H__

#include "zm/game_protocol.h"

class Server;

class ClientProxy {
Server &s_;
public:
  explicit ClientProxy(Server& s);

  void updateState(zm::proto::Game gs);

  zm::proto::Game getState();

};

#endif
