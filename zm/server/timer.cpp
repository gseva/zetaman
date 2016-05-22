#include "zm/server/timer.h"
#include "zm/server/physical/physical.h"
#include "zm/server_proxy.h"
#include <iostream>
#include <chrono>
#include <thread>

Timer::Timer(Physical& physical, ServerProxy& sp) : physical(physical),
  sp(sp){}

Timer::~Timer(){}

void Timer::run(){
  while ( true ) {
    physical.step();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    sp.updateState(sp.getState());
  }
  return;
}
