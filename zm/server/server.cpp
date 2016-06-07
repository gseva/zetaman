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
}

Server::~Server() {
  std::vector<Player*>::iterator playersIterator;
  for ( playersIterator = players.begin(); playersIterator != players.end();
    ++playersIterator ) {
    delete (*playersIterator);
  }

  for (auto item : proxies) {
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
  auto hostSock = std::make_shared<zm::ProtectedSocket>(accepter_->accept());
  newPlayer_();
  newClientProxy_(hostSock);
  zm::proto::ServerEvent event(zm::proto::connectedAsHost);
  std::string ev = event.serialize();
  std::cout << "Envio evento: " << ev << std::endl;
  hostSock->write(ev);

  accepting_ = true;
  while (accepting_) {
    auto playerSock = accepter_->accept();
    std::cout << "Acepto un jugador!: " << ev << std::endl;
    if (playerSock == NULL) continue;
    std::cout << "Creo new player!: " << ev << std::endl;
    newPlayer_();
    newClientProxy_(std::make_shared<zm::ProtectedSocket>(playerSock));
    zm::proto::ServerEvent event(zm::proto::connected);
    std::string ev = event.serialize();
    std::cout << "Envio evento: " << ev << std::endl;
    playerSock->write(ev);
  }

  std::cout << "Deleteo accepter: "  << std::endl;
  delete accepter_;
  std::cout << "Empiezo level: "  << std::endl;
  startLevel();
}

void acceptHost_(zm::Socket& accepter) {
}

void Server::newPlayer_(){
  Player* player = new Player();
  players.push_back(player);
}

void Server::newClientProxy_(std::shared_ptr<zm::ProtectedSocket> sock) {
  zm::ClientProxy* cp = new zm::ClientProxy(*this, sock);
  proxies.push_back(cp);
  cp->startListening();
}

void Server::selectLevel(int level) {
  std::cout << "Selecciono nivel " << level << std::endl;
  mapPath_ = DEFAULT_PATH;
  accepting_ = false;
  accepter_->close();
}

void Server::startLevel(){
  // envio el mapa
  JsonSerializer js;
  jm = js.importMap(DEFAULT_PATH);

  std::string map = jm.getReducedString();

  std::cout << "Escribo mapa" << std::endl;
  for (auto clientProxy : proxies) {
    clientProxy->getSocket()->write(map);
  }
  std::cout << "starteo game" << std::endl;
  for (auto clientProxy : proxies) {
    clientProxy->startGame();
  }
  std::cout << "Creo new level! " << mapPath_ << std::endl;
  level = new Level(players, mapPath_, *this);

  std::cout << "Empiezo a jugar! " << std::endl;
  playing_ = true;
  while (playing_) {
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
  for (auto&& clientProxy : proxies) {
    clientProxy->updateState(game);
  }
}
