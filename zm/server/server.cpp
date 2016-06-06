#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "assets/maps/mapita.json"

#define PPM 64

Server::Server() : port_("9090"), mapPath_(DEFAULT_PATH),
    accepting_(false), playing_(false) {
  JsonSerializer js;
  jm = js.importMap(DEFAULT_PATH);
}

Server::~Server() {
  std::vector<Player*>::iterator playersIterator;
  for ( playersIterator = players.begin(); playersIterator != players.end();
    ++playersIterator ) {
    delete (*playersIterator);
  }

  for(auto item : proxies) {
    delete item;
  }

  if ( level != NULL )
    stopLevel();
}

void Server::run() {
  accepter_ = new zm::Socket();
  accepter_->bindAndListen(port_);
  // acceptPlayers(&accepter);

  // Acepto el jugador host
  auto hostSock = accepter_->accept();
  newPlayer_();
  newClientProxy_(hostSock);

  accepting_ = true;
  while (accepting_) {
    auto playerSock = accepter_->accept();
    if (playerSock == NULL) continue;
    newPlayer_();
    newClientProxy_(playerSock);
  }

  delete accepter_;
  startLevel();
}

void acceptHost_(zm::Socket& accepter) {
}

void Server::newPlayer_(){
  Player* player = new Player();
  players.push_back(player);
}

void Server::newClientProxy_(std::shared_ptr<zm::Socket> sock) {
  zm::ClientProxy* cp = new zm::ClientProxy(*this, sock);
  proxies.push_back(cp);
}

void Server::selectLevel(int level) {
  mapPath_ = DEFAULT_PATH;
  accepting_ = false;
  accepter_->close();
}

void Server::startLevel(){
  // envio el mapa
  std::string map = jm.getReducedString();
  for(auto&& clientProxy : proxies) {
    clientProxy->getSocket()->write(map);
  }
  for(auto&& clientProxy : proxies) {
    clientProxy->startGame();
  }

  std::string path(DEFAULT_PATH);
  level = new Level(players, path, *this);

  playing_ = true;
  while (!playing_) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

}

void Server::stopLevel(){
  delete level;
}

void Server::jump(int playerNumber){
  level->jump(playerNumber);
}


void Server::right(int playerNumber){
  level->right(playerNumber);
}

void Server::left(int playerNumber){
  level->left(playerNumber);
}

void Server::stopHorizontalMove(int playerNumber){
  level->stopHorizontalMove(playerNumber);
}

void Server::up(int playerNumber){
  level->up(playerNumber);
}

void Server::shoot(int playerNumber){
  level->shoot(playerNumber);
}

zm::proto::Game Server::getState(){
  return level->getState();
}

void Server::updateState() {
  zm::proto::Game game = level->getState();
  for(auto&& clientProxy : proxies) {
    clientProxy->updateState(game);
  }
}
