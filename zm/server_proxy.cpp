
#include "zm/server_proxy.h"

#include <iostream>
#include <string>
#include <vector>

ServerProxy::ServerProxy() : s_(*this){
}

void ServerProxy::connect(){
  s_.newPlayer();
}


void ServerProxy::startTimer() {
  s_.startTimer();
}

void ServerProxy::jump() {
  s_.jump(0);
  std::cout << "hola\n";
}

zm::proto::Game ServerProxy::getState() {
  return s_.getState();
}

void ServerProxy::updateState(zm::proto::Game gs) {
  updateHandler.signal_game_update().emit(gs);
}

void ServerProxy::moveRight(){
  s_.right(0);
}
void ServerProxy::moveLeft(){
  s_.left(0);
}

void ServerProxy::stopHorizontalMove(){
  s_.stopHorizontalMove(0);
}


std::vector<std::string> ServerProxy::getImageNames() {
  return s_.getImageNames();
}

std::vector<int> ServerProxy::getImages() {
  return s_.getImages();
}
