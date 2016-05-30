
#include <iostream>

#include "zm/server/client_proxy.h"
#include "zm/server/server.h"

ClientProxy::ClientProxy(Server& s) : s_(s) {
}

void ClientProxy::updateState(zm::proto::Game gs) {
  std::cout << "update gamestate!" << std::endl;
}

zm::proto::Game ClientProxy::getState() {
  return s_.getState();
}
