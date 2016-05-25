#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "assets/maps/mapita.json"

#define PPM 64

Server::Server(ServerProxy& sp) : timer(physics, sp){
  JsonSerializer js;
  jm = js.importMap(DEFAULT_PATH);
  physics.setMap(jm);
}

Server::~Server(){
  timer.join();
  std::vector<Player*>::iterator playersInterator;
  for ( playersInterator = players.begin(); playersInterator != players.end(); 
    ++playersInterator ) {
    delete (*playersInterator);
  }
}

void Server::newPlayer(){
  Player* player = new Player(physics);
  players.push_back(player);
}

void Server::startTimer() {
  timer.start();
}

void Server::jump(int playerNummber){
	players[playerNummber]->jump();
}


void Server::right(int playerNummber){
  players[playerNummber]->right();
}
void Server::left(int playerNummber){
  players[playerNummber]->left();
}

void Server::stopHorizontalMove(int playerNummber){
  players[playerNummber]->stopHorizontalMove();
}

zm::proto::Game Server::getState(){
  //TODO: iterar sobre player y cargar correctamente el game
  b2Vec2 position = players[0]->getPosition();

  zm::proto::Game gs;
  gs.x = position.x * PPM;
  gs.y = position.y * -PPM + 768;
  return gs;
}

std::vector<std::string> Server::getImageNames(){
  return jm.imageNames;
}

std::vector<int> Server::getImages() {
  return jm.imageNumbers;
}
