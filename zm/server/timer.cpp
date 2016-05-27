#include "zm/server/timer.h"
#include "zm/server/physics/physics.h"
#include "zm/server_proxy.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

Timer::Timer(Physics& physics, ServerProxy& sp, std::vector<Enemy*>& enemies) :
physics(physics), sp(sp), enemies(enemies){}

Timer::~Timer(){}

void Timer::run(){
  while ( true ) {
    physics.step();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    std::vector<Enemy*>::iterator i;
    for ( i = enemies.begin(); i != enemies.end(); ++i ) {
      (*i)->lived();
    }
    sp.updateState(sp.getState());
  }
  return;
}
