
#include "zm/client/server_proxy.h"

#include <iostream>
#include <string>
#include <vector>
#

namespace zm {

ServerProxy::ServerProxy() : sender_(NULL) {
}

void ServerProxy::connect(){
  serverSock_.connect("127.0.0.1", "9090");
  sender_ = new Sender(eventQueue_, serverSock_);

  sender_->start();

  std::string mapString = serverSock_.readLine();
  std::cout << mapString << std::endl;
  map_.fromReducedString(mapString);
  // s_.newPlayer();
}


void ServerProxy::startLevel() {
  // s_.startLevel();
}

void ServerProxy::jump() {
  // s_.jump(0);
}

proto::Game ServerProxy::getState() {
  // return s_.getState();
  proto::Game game;
  return game;
}

void ServerProxy::updateState(zm::proto::Game gs) {
  updateHandler.signal_game_update().emit(gs);
}

void ServerProxy::moveRight(){
  // s_.right(0);
}
void ServerProxy::moveLeft(){
  // s_.left(0);
}

void ServerProxy::stopHorizontalMove(){
  // s_.stopHorizontalMove(0);
}

void ServerProxy::shutdown() {
  proto::ClientEvent client(proto::ClientEventType::shutdown);
  eventQueue_.push(client);

  sender_->join();

  serverSock_.close();

  delete sender_;
}

ServerProxy::~ServerProxy() {
  shutdown();
}


void ServerProxy::up(){
  // s_.up(0);
}

void ServerProxy::shoot(){
  // s_.shoot(0);
}

std::vector<std::string> ServerProxy::getImageNames() {
  return map_.imageNames;
}

std::vector<int> ServerProxy::getImages() {
  return map_.imageNumbers;
}


Sender::Sender(Queue<proto::ClientEvent>& eventQueue, Socket& serverSock) :
               eventQueue_(eventQueue), serverSock_(serverSock) {
}

void Sender::run() {
  proto::ClientEvent client;
  do {
    client = eventQueue_.pop();
    if (client.state == proto::ClientEventType::shutdown) continue;
    std::string s = client.serialize() + "\n";
    serverSock_.write(s);
  } while (client.state != proto::ClientEventType::shutdown);
}

} // zm
