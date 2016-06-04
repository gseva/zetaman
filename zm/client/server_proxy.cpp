
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


  getMap_();

  sender_ = new Sender(eventQueue_, serverSock_);
  sender_->start();
}

void ServerProxy::getMap_() {
  std::string mapString = serverSock_.read();
  map_.fromReducedString(mapString);
}

void ServerProxy::startLevel() {
  receiver_ = new Receiver(*this, serverSock_);
  receiver_->start();
}

proto::Game ServerProxy::getState() {
  proto::Game game;
  return game;
}

void ServerProxy::updateState(proto::Game gs) {
  updateHandler.signal_game_update().emit(gs);
}

void ServerProxy::jump() {
  proto::ClientEvent ce(proto::ClientEventType::jump);
  eventQueue_.push(ce);
}

void ServerProxy::moveRight(){
  proto::ClientEvent ce(proto::ClientEventType::moveRight);
  eventQueue_.push(ce);
}

void ServerProxy::moveLeft(){
  proto::ClientEvent ce(proto::ClientEventType::moveLeft);
  eventQueue_.push(ce);
}

void ServerProxy::stopHorizontalMove(){
  proto::ClientEvent ce(proto::ClientEventType::stopMoving);
  eventQueue_.push(ce);
}

void ServerProxy::up(){
  proto::ClientEvent ce(proto::ClientEventType::moveUp);
  eventQueue_.push(ce);
}

void ServerProxy::shoot(){
  proto::ClientEvent ce(proto::ClientEventType::shoot);
  eventQueue_.push(ce);
}


void ServerProxy::shutdown() {
  proto::ClientEvent ce(proto::ClientEventType::shutdown);
  eventQueue_.push(ce);

  receiver_->stop = true;
  sender_->join();

  serverSock_.close();
  receiver_->join();

  delete sender_;
  delete receiver_;
}

ServerProxy::~ServerProxy() {
  shutdown();
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
    std::string s = client.serialize();
    serverSock_.write(s);
  } while (client.state != proto::ClientEventType::shutdown);
}


Receiver::Receiver(ServerProxy& sp, Socket& serverSock)
                  : sp_(sp), serverSock_(serverSock), stop(false) {
}

void Receiver::run() {
  proto::Game game;
  do {
    try {
      game = proto::Game::deserialize(serverSock_.read());
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      continue;
    }
    sp_.updateState(game);
  } while (!stop);
}

} // zm
