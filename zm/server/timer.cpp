#include "zm/server/timer.h"
#include "zm/server/physics/physics.h"
#include "zm/server_proxy.h"
#include <iostream>
#include <chrono>
#include <thread>

Timer::Timer(Physics& physics, ServerProxy& sp) : physics(physics),
  sp(sp){}

Timer::~Timer(){}

void Timer::run(){
  while ( true ) {
    physics.step();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    sp.updateState(sp.getState());
  }
  return;
}
