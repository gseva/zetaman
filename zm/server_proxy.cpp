
#include "zm/server_proxy.h"

#include <iostream>

ServerProxy::ServerProxy() : s_(*this){
}

void ServerProxy::jump() {
  s_.jump();
  std::cout << "hola\n";
}

GameState ServerProxy::getState() {
  return s_.getState();
}

void ServerProxy::updateState(GameState gs) {
  updateHandler.signal_game_update().emit(gs);
}
