
#include "zm/server_proxy.h"

#include <iostream>

ServerProxy::ServerProxy() : s_(*this){
}

void ServerProxy::startTimer() {
  s_.startTimer();
}

void ServerProxy::jump() {
  s_.jump();
  std::cout << "hola\n";
}

zm::Game ServerProxy::getState() {
  return s_.getState();
}

void ServerProxy::updateState(zm::Game gs) {
  updateHandler.signal_game_update().emit(gs);
}

void ServerProxy::moveRight(){
  s_.right();
}
void ServerProxy::moveLeft(){
  s_.left();
}

void ServerProxy::stopHorizontalMove(){
  s_.stopHorizontalMove();
}


std::vector<std::string> ServerProxy::getImageNames() {
  return s_.getImageNames();
}

std::vector<int> ServerProxy::getImages() {
  return s_.getImages();
}
