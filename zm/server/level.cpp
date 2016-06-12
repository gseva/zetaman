
#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/camera.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/level.h"
#include "zm/game_protocol.h"

#define PPM 64
#define PLAYER "player"
#define MET "met"
#define BUMBY "bumby"

#define XMAX 47
#define XMIN 0

Level::Level(std::vector<Player*>& connectedPlayers, const std::string& path,
  Server& s) : timer(physics, s, enemies, bullets, connectedPlayers),
  players(connectedPlayers), camera(players) {
  JsonSerializer js;
  jm = js.importMap(path);
  physics.setMap(jm);
  unsigned int amountPlayers = 0;
  for ( std::vector<SpawnData>::iterator i = jm.spawnsData.begin();
    i != jm.spawnsData.end(); ++i ) {
    if ( jm.spawnTypes[(*i).type] == BUMBY ) {
      Enemy* enemy = new Bumby(physics, (*i).column+0.5f, (*i).row+0.5f);
      enemies.push_back(enemy);
    } else if ( jm.spawnTypes[(*i).type] == MET ) {
      Enemy* enemy = new Met(physics, (*i).column+0.5f, (*i).row+0.5f);
      enemies.push_back(enemy);
    } else if ( jm.spawnTypes[(*i).type] == PLAYER ) {
      if ( amountPlayers < players.size() ){
        std::cout << "Creo jugador: " << amountPlayers << std::endl;
        players[amountPlayers]->createBody(&physics,
          (*i).column+0.5f, (*i).row+0.5f);
        players[amountPlayers]->setCamera(&camera);
        amountPlayers++;
      }
    }
  }
  std::cout << "Empiezo timer!" << std::endl;
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
  std::vector<Bullet*>::iterator iBullet;
  for ( iBullet = bullets.begin(); iBullet != bullets.end(); ++iBullet ) {
    delete (*iBullet);
  }

  timer.join();
}

zm::proto::Game Level::getState(){
  zm::proto::Game gs;

  gs.camPos = camera.calculatePosition();
  int xo = gs.camPos.x;
  int yo = gs.camPos.y;

  for ( std::vector<Player*>::iterator player = players.begin();
    player != players.end(); ++player ) {
    zm::proto::Player protoPlayer;
    protoPlayer.pos.x = (*player)->getPosition().x* PPM - xo * PPM;
    protoPlayer.pos.y = (*player)->getPosition().y * -PPM + 768;
    gs.players.push_back(protoPlayer);
  }


  for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
    enemy != enemies.end(); ++enemy ) {
    zm::proto::Enemy protoEnemy = (*enemy)->toBean(xo, yo);
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
