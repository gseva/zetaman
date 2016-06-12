#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "zm/server/timer.h"
#include "zm/server/level.h"
#include "zm/server/server.h"
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/bullets.h"


Timer::Timer(Physics& physics, Server& s, Level& l) :
  physics(physics), s_(s), l_(l) {
    runContinue = true;
}

Timer::~Timer(){}

void Timer::run(){
  while ( runLevel() ) {
    physics.step();
    l_.clean();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    std::vector<Enemy*>::iterator i;
    for ( i = l_.enemies.begin(); i != l_.enemies.end(); ++i ) {
      Bullet* bullet = (*i)->move();
      if ( bullet != NULL ) {
        l_.bullets.push_back(bullet);
      }
    }
    std::vector<Bullet*>::iterator j;
    for ( j = l_.bullets.begin(); j != l_.bullets.end(); ++j ) {
      (*j)->move();
    }
    s_.updateState();
  }
  return;
}

bool Timer::runLevel(){
  Lock locker(mutexRun);
  return runContinue;
}

void Timer::finish(){
  Lock locker(mutexRun);
  runContinue = false;
}
