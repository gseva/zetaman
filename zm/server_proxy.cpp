
#include "server_proxy.h"

#include <iostream>

#include "server/server.h"

ServerProxy::ServerProxy() : s_(*this){
};

void ServerProxy::jump() {
  s_.jump();
  std::cout << "hola\n";
}

GameState ServerProxy::getState() {
  s_.getState();
}
