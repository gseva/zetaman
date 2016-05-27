
#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"
#include "level.h"
#include "zm/game_protocol.h"

#define PPM 64

Level::Level(std::vector<Player*>& connectedPlayers, const std::string& path, 
  ServerProxy& sp) : timer(physics, sp, enemies), players(connectedPlayers){
  JsonSerializer js;
  
  jm = js.importMap(path);
  physics.setMap(jm);


  for ( unsigned int i = 0; i < players.size(); ++i ) {
    players[i]->createBody(&physics);
  }

  Enemy* enemy = new Enemy(physics, 5.5f, 3.5f);
  enemies.push_back(enemy);
  enemy = new Enemy(physics, 2.5f, 8.5f);
  enemies.push_back(enemy);
  
  timer.start();
}
Level::~Level(){
  std::vector<Enemy*>::iterator iEnemy;
  for ( iEnemy = enemies.begin(); iEnemy != enemies.end(); ++iEnemy ) {
    delete (*iEnemy);
  }

  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    delete (*iPlayer);
  }
  timer.join();
}

zm::proto::Game Level::getState(){
//TODO: iterar sobre player y cargar correctamente el game
  b2Vec2 position = players[0]->getPosition();
  zm::proto::Game gs;
  for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
    enemy != enemies.end(); ++enemy ) {
    zm::proto::Enemy protoEnemy;
    protoEnemy.pos.x = (*enemy)->getPosition().x* PPM;
    protoEnemy.pos.y = (*enemy)->getPosition().y * -PPM + 768;
    gs.enemies.push_back(protoEnemy);
  }
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
