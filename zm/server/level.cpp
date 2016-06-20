
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

#define XMAX 47
#define XMIN 0

Level::Level(std::vector<Player*> connectedPlayers, JsonMap& jsonMap)
  : state(zm::proto::playing), players(connectedPlayers), jm(jsonMap),
    camera(players)
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

    Enemy* enemy;
    if ( type == BUMBY ) {
      enemy = new Bumby(physics, x, y);
    } else if ( type == MET ) {
      enemy = new Met(physics, x, y);
    } else if ( type == SNIPER ) {
      enemy = new Sniper(physics, x, y);
    } else if ( type == JUMPINGSNIPER ) {
      enemy = new JumpingSniper(physics, x, y);
    } else if ( type == BOMBMAN ) {
      enemy = new Bombman(physics, x, y, jm, players);
    } else if ( type == FIREMAN ) {
      enemy = new Fireman(physics, x, y, jm, players);
    } else if ( type == MAGNETMAN ) {
      enemy = new Magnetman(physics, x, y, jm, players);
    } else if ( type == SPARKMAN ) {
      enemy = new Sparkman(physics, x, y, jm, players);
    } else if ( type == RINGMAN ) {
      enemy = new Ringman(physics, x, y, jm, players);
    }
    enemies.push_back(enemy);
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
  std::vector<Bullet*>::iterator j;
  for ( j = bullets.begin(); j != bullets.end(); ++j ) {
    (*j)->move();
  }

  for ( auto&& player : players ) {
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
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    if ( (*iPlayer)->body->isDestroyed() ) {
      delete (*iPlayer)->body;
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
}

zm::proto::Game Level::getState(){
  zm::proto::Game gs;

  gs.camPos = camera.calculatePosition();
  int xo = gs.camPos.x;
  int yo = gs.camPos.y;

  for ( std::vector<Player*>::iterator player = players.begin();
    player != players.end(); ++player ) {
    if ((*player)->connected && !(*player)->body->isDestroyed()) {
      zm::proto::Player protoPlayer;
      protoPlayer.pos.x = (*player)->getPosition().x - xo;
      protoPlayer.pos.y = (*player)->getPosition().y;
      gs.players.push_back(protoPlayer);
    }
  }

  for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
    enemy != enemies.end(); ++enemy ) {
    zm::proto::Enemy protoEnemy = (*enemy)->toBean(xo, yo);
    gs.enemies.push_back(protoEnemy);
  }

  for ( std::vector<Bullet*>::iterator bullet = bullets.begin();
    bullet != bullets.end(); ++bullet ) {
    zm::proto::Proyectile proyectile = (*bullet)->toBean(xo, yo);
    gs.proyectiles.push_back(proyectile);
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
  std::vector<Player*>::iterator iPlayer;
  for ( iPlayer = players.begin(); iPlayer != players.end(); ++iPlayer ) {
    if ( !(*iPlayer)->body->isDestroyed() )
      lose = false;
  }

  return lose;
}

bool Level::checkWinCondition() {
  return !enemies.size();
}

void Level::addBullet(Bullet* bullet){
  bullets.push_back(bullet);
}
