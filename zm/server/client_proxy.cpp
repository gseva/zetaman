
#include <iostream>
#include <string>

#include "zm/server/client_proxy.h"
#include "zm/server/server.h"

namespace zm {


ClientProxy::ClientProxy(Player* p,
   std::shared_ptr<zm::ProtectedSocket> sock_) :
    player_(p), clientSock_(sock_) {
}

void ClientProxy::updateState(proto::Game gs) {
  eventQueue_.push(gs);
}

void ClientProxy::startListening() {
  receiver_ = new Receiver(*this, clientSock_);
  receiver_->start();
}

void ClientProxy::sendMap(const std::string& map) {
  zm::proto::ServerEvent event(zm::proto::mapSelected);
  event.payload = map;
  clientSock_->write(event.serialize());
}

void ClientProxy::startGame() {
  zm::proto::ServerEvent event(zm::proto::gameStart);
  clientSock_->write(event.serialize());

  sender_ = new Sender(eventQueue_, clientSock_);
  sender_->start();
}

void ClientProxy::dispatchEvent(proto::ClientEvent ce) {
  switch (ce.state) {
    case proto::moveLeft: player_->left(); break;
    case proto::moveRight: player_->right(); break;
    case proto::jump: player_->jump(); break;
    case proto::moveUp: player_->up(); break;
    case proto::moveDown: break;
    case proto::stopMoving: player_->stopHorizontalMove(); break;
    case proto::shoot: player_->shoot(); break;
    case proto::shutdown: player_->game.shutdown(player_->name); break;

    case proto::selectLevel1: player_->game.selectLevel(0); break;
    case proto::selectLevel2: player_->game.selectLevel(1); break;
    case proto::selectLevel3: player_->game.selectLevel(2); break;
    case proto::selectLevel4: player_->game.selectLevel(3); break;
    case proto::selectLevel5: player_->game.selectLevel(4); break;
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
  clientSock_->close();
  receiver_->join();

  delete sender_;
  delete receiver_;
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
