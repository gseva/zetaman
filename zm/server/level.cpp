
#include <iostream>
#include <string>
#include <vector>

#include <Box2D/Box2D.h>

#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/camera.h"
#include "zm/server/server.h"
#include "zm/server/level.h"
#include "zm/server/physics/boss.h"
#include "zm/server/physics/players.h"

#define PLAYER "player"
#define MET "met"
#define BUMBY "bumby"
#define SNIPER "sniper"
#define JUMPINGSNIPER "jumpingsniper"
#define BOMBMAN "bombman"
#define FIREMAN "fireman"
#define MAGNETMAN "magnetman"
#define SPARKMAN "sparkman"
#define RINGMAN "ringman"


Level::Level(std::vector<Player*> connectedPlayers, JsonMap& jsonMap)
  : state(zm::proto::playing), players(connectedPlayers), jm(jsonMap),
    camera(players, jm)
{
  physics.setMap(jm);
  unsigned int amountPlayers = 0;
  for (auto&& spawn : jm.spawnsData) {
    std::string type = jm.spawnTypes[spawn.type];
    float32 x = spawn.column + .5f;
    float32 y = spawn.row + .5f;

    if ( type == PLAYER ) {
      if ( amountPlayers < players.size() ){
        std::cout << "Creo jugador: " << amountPlayers << std::endl;
        players[amountPlayers]->createBody(&physics,
          x, y);
        players[amountPlayers]->setCamera(&camera);
        amountPlayers++;
      }
      continue;
    }

    if ( type == BUMBY ) {
      enemies.push_back(new Bumby(physics, x, y));
    } else if ( type == MET ) {
      enemies.push_back(new Met(physics, x, y));
    } else if ( type == SNIPER ) {
      enemies.push_back(new Sniper(physics, x, y));
    } else if ( type == JUMPINGSNIPER ) {
      enemies.push_back(new JumpingSniper(physics, x, y));
    } else if ( type == BOMBMAN ) {
      boss = new Bombman(physics, x, y, jm, players);
    } else if ( type == FIREMAN ) {
      boss = new Fireman(physics, x, y, jm, players);
    } else if ( type == MAGNETMAN ) {
      boss = new Magnetman(physics, x, y, jm, players);
    } else if ( type == SPARKMAN ) {
      boss = new Sparkman(physics, x, y, jm, players);
    } else if ( type == RINGMAN ) {
      boss = new Ringman(physics, x, y, jm, players);
    }
  }
}

void Level::step() {
  physics.step();
  clean();

  std::vector<Enemy*>::iterator i;
  for ( i = enemies.begin(); i != enemies.end(); ++i ) {
    Bullet* bullet = (*i)->move();
    if ( bullet != NULL ) {
      bullets.push_back(bullet);
    }
  }

  Bullet* bullet = boss->move();
  if ( bullet != NULL ) {
    bullets.push_back(bullet);
  }

  std::vector<Bullet*>::iterator j;
  for ( j = bullets.begin(); j != bullets.end(); ++j ) {
    (*j)->move();
  }

  for (auto&& player : players) {
    player->tic();
  }
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

  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ) {
    if ((*iPlayer)->body->isDestroyed()) {
      (*iPlayer)->destroy();
      iPlayer = players.erase(iPlayer);
    } else {
      ++iPlayer;
    }
  }
}

Level::~Level() {
  std::cout << "Borrando enemigos" << std::endl;
  std::vector<Enemy*>::iterator iEnemy;
  for ( iEnemy = enemies.begin(); iEnemy != enemies.end(); ) {
    delete (*iEnemy);
    iEnemy = enemies.erase(iEnemy);
  }

  std::cout << "Borrando jugadores" << std::endl;
  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); iPlayer++ ) {
    physics.destroyBody((*iPlayer)->body->getBody());
  }

  std::cout << "Borrando balas" << std::endl;
  std::vector<Bullet*>::iterator iBullet;
  for ( iBullet = bullets.begin(); iBullet != bullets.end(); ) {
    delete (*iBullet);
    iBullet = bullets.erase(iBullet);
  }

  delete boss;
}

zm::proto::Game Level::getState(){
  zm::proto::Game gs;

  gs.camPos = camera.calculatePosition();
  int xo = gs.camPos.x;
  int yo = gs.camPos.y;


  for (auto&& player : players) {
    if (player->connected && !player->body->isDestroyed()) {
      gs.players.push_back(player->toBean(xo, yo));
    }
  }

  for (auto&& enemy : enemies) {
    gs.enemies.push_back(enemy->toBean(xo, yo));
  }

  gs.enemies.push_back(boss->toBean(xo, yo));

  for (auto&& bullet : bullets) {
    zm::proto::Proyectile pr = bullet->toBean(xo, yo);
    gs.proyectiles.push_back(pr);
  }

  if (checkLoseCondition()) {
    state = zm::proto::lost;
  } else if (checkWinCondition()) {
    state = zm::proto::won;
  }
  gs.state = state;

  return gs;
}

bool Level::checkLoseCondition() {
  bool lose = true;

  for(auto&& player : players) {
    if (!player->body->isDestroyed()) {
      lose = false;
    }
  }
  return lose;
}

bool Level::checkWinCondition() {
  return boss->isDestroyed();
}

void Level::addBullet(Bullet* bullet){
  bullets.push_back(bullet);
}
