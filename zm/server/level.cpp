
#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"
#include "level.h"

#define PPM 64

Level::Level(std::vector<Player*>& connectedPlayers, const std::string& path, 
  ServerProxy& sp) : timer(physics, sp), players(connectedPlayers){
  JsonSerializer js;
  
  jm = js.importMap(path);
  physics.setMap(jm);

  std::vector<Player*>::iterator i;
  for ( i = players.begin(); i != players.end(); ++i ) {
    (*i)->createBody(&physics);
  }

  timer.start();
}
Level::~Level(){
  timer.join();
}

zm::proto::Game Level::getState(){
//TODO: iterar sobre player y cargar correctamente el game
  b2Vec2 position = players[0]->getPosition();

  zm::proto::Game gs;
  gs.x = position.x * PPM;
  gs.y = position.y * -PPM + 768;
  return gs;
}

void Level::jump(int playerNummber){
  players[playerNummber]->jump();
}

void Level::right(int playerNummber){
  players[playerNummber]->right();
}

void Level::left(int playerNummber){
  players[playerNummber]->left();
}

void Level::stopHorizontalMove(int playerNummber){
  players[playerNummber]->stopHorizontalMove();
}

std::vector<std::string> Level::getImageNames(){
  return jm.imageNames;
}
std::vector<int> Level::getImages(){
  return jm.imageNumbers;
}
