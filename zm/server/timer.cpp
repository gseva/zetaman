#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "zm/server/server.h"
#include "zm/server/timer.h"
#include "zm/server/physics/physics.h"

Timer::Timer(Physics& physics, Server& s,
             std::vector<Enemy*>& enemies,
             std::vector<Bullet*>& bullets,
             std::vector<Player*>& players) :
  physics(physics), s_(s), enemies(enemies), bullets(bullets),
  players(players) {
    runContinue = true;
}

Timer::~Timer(){}

void Timer::run(){
  // Unused
  players.size();

  while ( runLevel() ) {
    physics.step();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
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
    collides(enemies, bullets);
    s_.updateState();
  }
  return;
}

void Timer::collides(std::vector<Enemy*>& enemies,
  std::vector<Bullet*>& bullets){
  std::vector<std::vector<Player*>::iterator> playersToDestroy;
  std::vector<std::vector<Enemy*>::iterator> enemiesToDestroy;
  std::vector<std::vector<Bullet*>::iterator> bulletsToDestroy;

  for (std::vector<Bullet*>::iterator bullet = bullets.begin();
    bullet != bullets.end(); ++bullet ) {
    for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
      enemy != enemies.end(); ++enemy ) {
      if ( b2TestOverlap(
        (*enemy)->body->GetFixtureList()[0].GetShape(), 0,
        (*bullet)->body->GetFixtureList()[0].GetShape(),0,
        (*enemy)->body->GetTransform(), (*bullet)->body->GetTransform())
        &&
        (*enemy)->collide(*bullet) ) {
        enemiesToDestroy.push_back(enemy);
        bulletsToDestroy.push_back(bullet);
      }
    }
    /*for ( std::vector<Player*>::iterator player = players.begin();
      player != players.end(); ++player ) {
      if ( b2TestOverlap(
        (*player)->getBody()->GetFixtureList()[0].GetShape(), 0,
        (*bullet)->body->GetFixtureList()[0].GetShape(),0,
        (*player)->getBody()->GetTransform(),
        (*bullet)->body->GetTransform())
        &&
        (*player)->collide(*bullet) ) {
        playersToDestroy.push_back(player);
        bulletsToDestroy.push_back(bullet);
      }
    } */
  }
  std::vector<std::vector<Enemy*>::iterator>::iterator enemyToDestroy;
  for ( enemyToDestroy = enemiesToDestroy.begin();
    enemyToDestroy != enemiesToDestroy.end(); ++enemyToDestroy ) {
    delete **enemyToDestroy;
    enemies.erase(*enemyToDestroy);
  }
  std::vector<std::vector<Bullet*>::iterator>::iterator bulletToDestroy;
  for ( bulletToDestroy = bulletsToDestroy.begin();
    bulletToDestroy != bulletsToDestroy.end(); ++bulletToDestroy ) {
    delete **bulletToDestroy;
    bullets.erase(*bulletToDestroy);
  }
}

bool Timer::runLevel(){
  Lock locker(mutexRun);
  return runContinue;
}

void Timer::finish(){
  Lock locker(mutexRun);
  runContinue = false;
}
