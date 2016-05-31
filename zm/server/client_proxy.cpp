
#include <iostream>

#include "zm/server/client_proxy.h"
#include "zm/server/server.h"

namespace zm {


ClientProxy::ClientProxy(Server& s, Socket sock_)
                        : s_(s), clientSock_(sock_) {
}

void ClientProxy::updateState(proto::Game gs) {
  eventQueue_.push(gs);
}

void ClientProxy::startGame() {
  sender_ = new Sender(eventQueue_, clientSock_);
  sender_->start();
}

ClientProxy::~ClientProxy() {
  sender_->join();
}

proto::Game ClientProxy::getState() {
  return s_.getState();
}


Sender::Sender(Queue<proto::Game>& eventQueue, Socket& clientSock) :
               eventQueue_(eventQueue), clientSock_(clientSock) {
}

void Sender::run() {
  proto::Game game;
  do {
    game = eventQueue_.pop();
    if (game.state != proto::GameState::playing) continue;
    std::string s = game.serialize() + "\n";
    clientSock_.write(s);
  } while (game.state == proto::GameState::playing);
}


Receiver::Receiver(ClientProxy& cp, Socket& clientSock)
                  : cp_(cp), clientSock_(clientSock), stop(true) {
}

void Receiver::run() {
  proto::Game game;
  do {
    try {
      game = proto::Game::deserialize(clientSock_.readLine());
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
      continue;
    }
    cp_.updateState(game);
  } while (!stop);
}


} // zm
