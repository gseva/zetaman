
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
  // s_.newPlayer();
}

void ServerProxy::getMap_() {
  std::string mapString = serverSock_.readLine();
  map_.fromReducedString(mapString);
}

void ServerProxy::startLevel() {
  receiver_ = new Receiver(*this, serverSock_);
  receiver_->start();
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

void ServerProxy::updateState(proto::Game gs) {
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


Receiver::Receiver(ServerProxy& sp, Socket& serverSock)
                  : sp_(sp), serverSock_(serverSock), stop(false) {
}

void Receiver::run() {
  proto::Game game;
  do {
    try {
      game = proto::Game::deserialize(serverSock_.readLine());
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      continue;
    }
    sp_.updateState(game);
  } while (!stop);
}

} // zm
