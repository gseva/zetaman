
#include "server_proxy.h"

#include <iostream>

#include "client/client.h"

ServerProxy::ServerProxy(Client* c) : c_(c) {
};

void ServerProxy::jump() {
  // s_.jump()
  std::cout << "hola\n";
}

void ServerProxy::setState(GameState state) {
  c_->draw(state);
}
