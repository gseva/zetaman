
#include "zm/client/server_proxy.h"
#include "zm/client/client.h"

#include <iostream>
#include <string>
#include <vector>

namespace zm {

ServerProxy::ServerProxy() : sender_(NULL), isHost(false) {
}

void ServerProxy::connect(){
  serverSock_.connect("127.0.0.1", "9090");
  getConnection_();

  sender_ = new Sender(eventQueue_, serverSock_);
  sender_->start();
}

void ServerProxy::getConnection_() {
  std::cout << "Leyendo evento" << std::endl;
  std::string eventString = serverSock_.read();
  proto::ServerEvent ev = proto::ServerEvent::deserialize(eventString);
  isHost = ev.state == proto::connectedAsHost;
  std::cout << "recibo evento, isHost: " << isHost <<  std::endl;
}

void ServerProxy::getMap() {
  std::string mapString = serverSock_.read();

  std::cout << "recibo map " << mapString <<  std::endl;
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

void ServerProxy::moveRight() {
  proto::ClientEvent ce(proto::ClientEventType::moveRight);
  eventQueue_.push(ce);
}

void ServerProxy::moveLeft() {
  proto::ClientEvent ce(proto::ClientEventType::moveLeft);
  eventQueue_.push(ce);
}

void ServerProxy::stopHorizontalMove() {
  proto::ClientEvent ce(proto::ClientEventType::stopMoving);
  eventQueue_.push(ce);
}

void ServerProxy::up() {
  proto::ClientEvent ce(proto::ClientEventType::moveUp);
  eventQueue_.push(ce);
}

void ServerProxy::shoot() {
  proto::ClientEvent ce(proto::ClientEventType::shoot);
  eventQueue_.push(ce);
}

void ServerProxy::selectLevel(int level) {
  proto::ClientEventType type;
  switch (level) {
    case 1: type = proto::selectLevel1; break;
    case 2: type = proto::selectLevel2; break;
    case 3: type = proto::selectLevel3; break;
    case 4: type = proto::selectLevel4; break;
    case 5: type = proto::selectLevel5; break;
  }
  proto::ClientEvent ce(type);
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


Sender::Sender(Queue<proto::ClientEvent>& eventQueue,
               ProtectedSocket& serverSock) :
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


Receiver::Receiver(ServerProxy& sp, ProtectedSocket& serverSock)
                  : sp_(sp), serverSock_(serverSock), stop(false) {
}

void Receiver::run() {
  proto::Game game;
  do {
    try {
      std::string res = serverSock_.read();
      game = proto::Game::deserialize(res);
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      throw;
    }
    sp_.updateState(game);
  } while (!stop);
}

} // zm
