
#include <iostream>
#include <string>

#include "zm/server/client_proxy.h"
#include "zm/server/server.h"

namespace zm {


ClientProxy::ClientProxy(Server& s, std::shared_ptr<Socket> sock_)
                        : s_(s), clientSock_(sock_) {
}

void ClientProxy::updateState(proto::Game gs) {
  eventQueue_.push(gs);
}

void ClientProxy::startGame() {
  sender_ = new Sender(eventQueue_, clientSock_);
  sender_->start();
  receiver_ = new Receiver(*this, clientSock_);
  receiver_->start();
}

void ClientProxy::dispatchEvent(proto::ClientEvent ce) {
  switch (ce.state) {
    case proto::moveLeft: s_.left(0); break;
    case proto::moveRight: s_.right(0); break;
    case proto::jump: s_.jump(0); break;
    case proto::moveUp: s_.up(0); break;
    case proto::moveDown: break;
    case proto::stopMoving: s_.stopHorizontalMove(0); break;
    case proto::shoot: s_.shoot(0); break;
    case proto::shutdown: break;

    case proto::selectLevel1: s_.selectLevel(0); break;
    case proto::selectLevel2: s_.selectLevel(1); break;
    case proto::selectLevel3: s_.selectLevel(2); break;
    case proto::selectLevel4: s_.selectLevel(3); break;
    case proto::selectLevel5: s_.selectLevel(4); break;
  }
}

std::shared_ptr<Socket> ClientProxy::getSocket() {
  return clientSock_;
}

ClientProxy::~ClientProxy() {
  proto::Game game(proto::GameState::lost);
  eventQueue_.push(game);

  clientSock_->close();
  receiver_->stop = true;

  sender_->join();
  receiver_->join();

  delete sender_;
  delete receiver_;
}

proto::Game ClientProxy::getState() {
  return s_.getState();
}


Sender::Sender(Queue<proto::Game>& eventQueue,
               std::shared_ptr<Socket> clientSock) :
               eventQueue_(eventQueue), clientSock_(clientSock) {
}

void Sender::run() {
  proto::Game game;
  do {
    game = eventQueue_.pop();
    if (game.state != proto::GameState::playing) continue;
    std::string s = game.serialize();
    clientSock_->write(s);
  } while (game.state == proto::GameState::playing);
}


Receiver::Receiver(ClientProxy& cp, std::shared_ptr<Socket> clientSock)
                  : cp_(cp), clientSock_(clientSock), stop(false) {
}

void Receiver::run() {
  proto::ClientEvent event;
  do {
    try {
      event = proto::ClientEvent::deserialize(clientSock_->read());
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      continue;
    }
    cp_.dispatchEvent(event);
  } while (!stop);
}


} // zm
