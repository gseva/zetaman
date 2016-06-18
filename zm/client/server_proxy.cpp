
#include "zm/client/server_proxy.h"
#include "zm/client/client.h"

#include <iostream>
#include <string>
#include <vector>

namespace zm {

ServerProxy::ServerProxy(Client& client) : sender_(NULL),
    client_(client) {
}

void ServerProxy::connect(){
  serverSock_.connect("127.0.0.1", "9090");

  sender_ = new Sender(eventQueue_, serverSock_);
  sender_->start();

  receiver_ = new Receiver(*this, serverSock_);
  receiver_->start();
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

void ServerProxy::dispatchEvent(proto::ServerEvent event) {
  switch (event.state) {
    case proto::connected: client_.waitForPlayers(); break;
    case proto::connectedAsHost: client_.selectLevel(); break;
    case proto::playerConnected:
      client_.showConnectedPlayer(event.payload);
      break;
    case proto::mapSelected:
      map_.fromReducedString(event.payload);
      client_.startGame();
      break;
    case proto::gameStart: break;
  }
}


void ServerProxy::shutdown() {
  receiver_->stop = true;

  proto::ClientEvent ce(proto::ClientEventType::shutdown);
  eventQueue_.push(ce);

  sender_->join();

  serverSock_.close();
  receiver_->join();


  delete sender_;
  delete receiver_;
}

ServerProxy::~ServerProxy() {
  shutdown();
}


JsonMap& ServerProxy::getJsonMap() {
  return map_;
}


Sender::Sender(Queue<proto::ClientEvent>& eventQueue,
               ProtectedSocket& serverSock) :
               eventQueue_(eventQueue), serverSock_(serverSock) {
}

void Sender::run() {
  proto::ClientEvent client;
  do {
    client = eventQueue_.pop();
    //if (client.state == proto::ClientEventType::shutdown) continue;
    std::string s = client.serialize();
    serverSock_.write(s);
  } while (client.state != proto::ClientEventType::shutdown);
}


Receiver::Receiver(ServerProxy& sp, ProtectedSocket& serverSock)
    : sp_(sp), serverSock_(serverSock), receiveEvents(true), stop(false) {
}

void Receiver::run() {
  do {
    try {
      std::string res = serverSock_.read();
      if (receiveEvents) {
        proto::ServerEvent ev = proto::ServerEvent::deserialize(res);
        if (ev.state == proto::gameStart) receiveEvents = false;
        sp_.dispatchEvent(ev);
      } else {
        proto::Game game = proto::Game::deserialize(res);
        sp_.updateState(game);
      }
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      throw;
    }
  } while (!stop);
}

} // zm
