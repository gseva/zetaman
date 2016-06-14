
#include <iostream>
#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/camera.h"
#include "zm/server/server.h"
#include "zm/server/level.h"

#define PLAYER "player"
#define MET "met"
#define BUMBY "bumby"

#define XMAX 47
#define XMIN 0

Level::Level(std::vector<Player*>& connectedPlayers, JsonMap& jsonMap,
    Server& s) : players(connectedPlayers), timer(physics, s, *this),
    jm(jsonMap), camera(players)
{
  physics.setMap(jm);
  unsigned int amountPlayers = 0;
  for (auto&& spawn : jm.spawnsData) {
    std::string type = jm.spawnTypes[spawn.type];
    float32 x = spawn.column + .5f;
    float32 y = spawn.row + .5f;

    if (type == BUMBY) {
        Enemy* enemy = new Bumby(physics, x, y);
        enemies.push_back(enemy);
    } else if (type == MET) {
      Enemy* enemy = new Met(physics, x, y);
      enemies.push_back(enemy);
    } else if ((type == PLAYER) && (amountPlayers < players.size())) {
      std::cout << "Creo jugador: " << amountPlayers << std::endl;
      players[amountPlayers]->createBody(&physics, x, y);
      players[amountPlayers]->setCamera(&camera);
      amountPlayers++;
    }
  }

  std::cout << "Empiezo timer!" << std::endl;
  timer.start();
}

void Level::clean() {
  std::vector<Bullet*>::iterator iBullet;
  for (iBullet = bullets.begin(); iBullet != bullets.end();) {
    if ((*iBullet)->isDestroyed()) {
      delete (*iBullet);
      iBullet = bullets.erase(iBullet);
    } else {
      ++iBullet;
    }
  }

  std::vector<Enemy*>::iterator iEnemy;
  for (iEnemy = enemies.begin(); iEnemy != enemies.end();) {
    if ((*iEnemy)->isDestroyed()) {
      delete (*iEnemy);
      iEnemy = enemies.erase(iEnemy);
    } else {
      ++iEnemy;
    }
  }
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
    if ((*player)->connected) {
      zm::proto::Player protoPlayer;
      protoPlayer.pos.x = (*player)->getPosition().x - xo;
      protoPlayer.pos.y = (*player)->getPosition().y;
      gs.players.push_back(protoPlayer);
    }
  }


  for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
    enemy != enemies.end(); ++enemy ) {
    zm::proto::Enemy protoEnemy = (*enemy)->toBean(xo, yo);
    // std::cout << "Enemy: " << protoEnemy.pos.x << " "
    // << protoEnemy.pos.y <<  std::endl;
    gs.enemies.push_back(protoEnemy);
  }

  for ( std::vector<Bullet*>::iterator bullet = bullets.begin();
    bullet != bullets.end(); ++bullet ) {
    zm::proto::Proyectile proyectile;
    proyectile.pos.x = (*bullet)->getPosition().x - xo;
    proyectile.pos.y = (*bullet)->getPosition().y;
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

void Level::disconnect(int playerNumber){
  players[playerNumber]->disconnect();
}
