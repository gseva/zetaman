
#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/level.h"
#include "zm/game_protocol.h"

#define PPM 64
#define PLAYER "player"
#define ENEMY "enemy"

#define XMAX 47
#define XMIN 0

Level::Level(std::vector<Player*>& connectedPlayers, const std::string& path, 
  ServerProxy& sp) : timer(physics, sp, enemies, bullets),
  players(connectedPlayers){
  JsonSerializer js;
  jm = js.importMap(path);
  physics.setMap(jm);
  unsigned int amountPlayers = 0;
  for ( std::vector<SpawnData>::iterator i = jm.spawnsData.begin();
    i != jm.spawnsData.end(); ++i ) {
    if ( jm.spawnTypes[(*i).type] == ENEMY ) {
      Enemy* enemy = new Met(physics, (*i).column+0.5f, (*i).row+0.5f);
      enemies.push_back(enemy);
    } else if ( jm.spawnTypes[(*i).type] == PLAYER ) {
      if ( amountPlayers < players.size() ){
        players[amountPlayers]->createBody(&physics,
          (*i).column+0.5f, (*i).row+0.5f);
        amountPlayers++;
      }
    }
  }
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
  zm::proto::Game gs;

  int xmin, xmax, xo;
  int yo = 0;

  xmin = (*players.begin())->getPosition().x;
  xmax = xmin;

  for ( std::vector<Player*>::iterator player = players.begin();
    player != players.end(); ++player ) {
    if ( ((*player)->getPosition().x ) < xmin )
      xmin = (*player)->getPosition().x;
    if ( ((*player)->getPosition().x ) > xmax )
      xmax = (*player)->getPosition().x;
  }
  xo = (xmax + xmin) / 2 - 8;// * PPM;
  if ( xo > XMAX )
    xo = XMAX;
  if ( xo < XMIN )
    xo = XMIN;

  gs.camPos.x = xo;
  gs.camPos.y = yo;

  for ( std::vector<Player*>::iterator player = players.begin();
    player != players.end(); ++player ) {
    zm::proto::Player protoPlayer;
    protoPlayer.pos.x = (*player)->getPosition().x* PPM - xo * PPM;
    protoPlayer.pos.y = (*player)->getPosition().y * -PPM + 768;
    gs.players.push_back(protoPlayer);
  }


  for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
    enemy != enemies.end(); ++enemy ) {
    zm::proto::Enemy protoEnemy;
    protoEnemy.pos.x = (*enemy)->getPosition().x* PPM - xo * PPM;
    protoEnemy.pos.y = (*enemy)->getPosition().y * -PPM + 768;
    gs.enemies.push_back(protoEnemy);
  }

  for ( std::vector<Bullet*>::iterator bullet = bullets.begin();
    bullet != bullets.end(); ++bullet ) {
    zm::proto::Proyectile proyectile;
    proyectile.pos.x = (*bullet)->getPosition().x * PPM - xo * PPM;
    proyectile.pos.y = (*bullet)->getPosition().y * -PPM + 768;
    gs.proyectiles.push_back(proyectile);
  }

  return gs;
}

void Level::jump(int playerNumber){
  players[playerNumber]->jump();
}

void Level::right(int playerNumber){
  players[playerNumber]->right();
}

void Level::left(int playerNumber){
  players[playerNumber]->left();
}

void Level::stopHorizontalMove(int playerNumber){
  players[playerNumber]->stopHorizontalMove();
}

void Level::up(int playerNumber){
  players[playerNumber]->up();
}

void Level::shoot(int playerNumber){
  Bullet* bullet = players[playerNumber]->shoot();
  bullets.push_back(bullet);
}

std::vector<std::string> Level::getImageNames(){
  return jm.imageNames;
}

std::vector<int> Level::getImages(){
  return jm.imageNumbers;
}
