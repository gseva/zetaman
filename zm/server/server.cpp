#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "assets/maps/mapita.json"

#define PPM 64

Server::Server(ServerProxy& sp) : sp(sp){
  JsonSerializer js;
  jm = js.importMap(DEFAULT_PATH);
}

Server::~Server(){
  std::vector<Player*>::iterator playersIterator;
  for ( playersIterator = players.begin(); playersIterator != players.end(); 
    ++playersIterator ) {
    delete (*playersIterator);
  }
  if ( level != NULL )
    stopLevel();
}

void Server::newPlayer(){
  Player* player = new Player();
  players.push_back(player);
}


void Server::startLevel(){
  std::string path(DEFAULT_PATH);
  level = new Level(players, path, sp);
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

std::vector<std::string> Server::getImageNames(){
  return jm.imageNames;
}

std::vector<int> Server::getImages() {
  return jm.imageNumbers;
}
