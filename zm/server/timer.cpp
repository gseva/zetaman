#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "zm/server/timer.h"
#include "zm/server/physics/physics.h"

Timer::Timer(Physics& physics, ClientProxy& cp, std::vector<Enemy*>& enemies,
  std::vector<Bullet*>& bullets) : physics(physics), cp_(cp), enemies(enemies),
  bullets(bullets){}

Timer::~Timer(){}

void Timer::run(){
  while ( true ) {
    physics.step();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    std::vector<Enemy*>::iterator i;
    for ( i = enemies.begin(); i != enemies.end(); ++i ) {
      (*i)->lived();
    }
    std::vector<Bullet*>::iterator j;
    for ( j = bullets.begin(); j != bullets.end(); ++j ) {
      (*j)->move();
    }
    collides(enemies, bullets);
    cp_.updateState(cp_.getState());
  }
  return;
}

void Timer::collides(std::vector<Enemy*>& enemies,
  std::vector<Bullet*>& bullets){
  std::vector<std::vector<Enemy*>::iterator> enemiesToDestroy;
  std::vector<std::vector<Bullet*>::iterator> bulletsToDestroy;

  for (std::vector<Bullet*>::iterator bullet = bullets.begin();
    bullet != bullets.end(); ++bullet ) {
    for ( std::vector<Enemy*>::iterator enemy = enemies.begin();
      enemy != enemies.end(); ++enemy ) {
      if ( b2TestOverlap(
        (*enemy)->body->GetFixtureList()[0].GetShape(), 0,
        (*bullet)->body->GetFixtureList()[0].GetShape(),0,
        (*enemy)->body->GetTransform(), (*bullet)->body->GetTransform()) ) {
        enemiesToDestroy.push_back(enemy);
        bulletsToDestroy.push_back(bullet);
      }
    }
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
