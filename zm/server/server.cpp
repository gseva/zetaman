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
  std::vector<Player*>::iterator playersInterator;
  for ( playersInterator = players.begin(); playersInterator != players.end(); 
    ++playersInterator ) {
    delete (*playersInterator);
  }
}

void Server::newPlayer(){
  Player* player = new Player();
  players.push_back(player);
}


void Server::startLevel(){
  std::string path(DEFAULT_PATH);
  level = new Level(players, path, sp);
}

void Server::jump(int playerNummber){
  level->jump(playerNummber);
}


void Server::right(int playerNummber){
  level->right(playerNummber);
}

void Server::left(int playerNummber){
  level->left(playerNummber);
}

void Server::stopHorizontalMove(int playerNummber){
  level->stopHorizontalMove(playerNummber);
}

void Server::up(int playerNummber){
  level->up(playerNummber);
}

zm::proto::Game Server::getState(){
  //TODO: iterar sobre player y cargar correctamente el game
  return level->getState();
}

std::vector<std::string> Server::getImageNames(){
  return jm.imageNames;
}

std::vector<int> Server::getImages() {
  return jm.imageNumbers;
}
