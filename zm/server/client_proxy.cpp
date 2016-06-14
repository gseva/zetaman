
#include <iostream>
#include <string>

#include "zm/server/client_proxy.h"
#include "zm/server/server.h"

namespace zm {


ClientProxy::ClientProxy(Server& s, int playerNumber,
   std::shared_ptr<zm::ProtectedSocket> sock_) :
    s_(s), playerNumber_(playerNumber), clientSock_(sock_) {
}

void ClientProxy::updateState(proto::Game gs) {
  eventQueue_.push(gs);
}

void ClientProxy::startListening() {
  receiver_ = new Receiver(*this, clientSock_);
  receiver_->start();
}

void ClientProxy::startGame() {
  sender_ = new Sender(eventQueue_, clientSock_);
  sender_->start();
}

void ClientProxy::dispatchEvent(proto::ClientEvent ce) {
  switch (ce.state) {
    case proto::moveLeft: s_.left(playerNumber_); break;
    case proto::moveRight: s_.right(playerNumber_); break;
    case proto::jump: s_.jump(playerNumber_); break;
    case proto::moveUp: s_.up(playerNumber_); break;
    case proto::moveDown: break;
    case proto::stopMoving: s_.stopHorizontalMove(playerNumber_); break;
    case proto::shoot: s_.shoot(playerNumber_); break;
    case proto::shutdown: s_.shutdown(playerNumber_); break;

    case proto::selectLevel1: s_.selectLevel(0); break;
    case proto::selectLevel2: s_.selectLevel(1); break;
    case proto::selectLevel3: s_.selectLevel(2); break;
    case proto::selectLevel4: s_.selectLevel(3); break;
    case proto::selectLevel5: s_.selectLevel(4); break;
  }
}

std::shared_ptr<zm::ProtectedSocket> ClientProxy::getSocket() {
  return clientSock_;
}

void ClientProxy::shutdown() {
  receiver_->stop = true;

  proto::Game game(proto::GameState::lost);
  eventQueue_.push(game);

  sender_->join();
  receiver_->join();

  clientSock_->close();

  delete sender_;
  delete receiver_;
}

proto::Game ClientProxy::getState() {
  return s_.getState();
}


Sender::Sender(Queue<proto::Game>& eventQueue,
               std::shared_ptr<zm::ProtectedSocket> clientSock) :
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


Receiver::Receiver(ClientProxy& cp,
                   std::shared_ptr<zm::ProtectedSocket> clientSock)
                  : cp_(cp), clientSock_(clientSock), stop(false) {
}

void Receiver::run() {
  proto::ClientEvent event;
  do {
    try {
      std::string ev = clientSock_->read();
      std::cout << "Recibo " << ev << std::endl;
      event = proto::ClientEvent::deserialize(ev);
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      throw;
    }
    cp_.dispatchEvent(event);
  } while (!stop);
}


} // zm
