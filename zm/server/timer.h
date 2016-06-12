#ifndef __TIMER_H__
#define __TIMER_H__
#include <vector>

#include <zm/thread.h>
#include "zm/server/player.h"
#include "zm/server/client_proxy.h"
#include "zm/server/physics/world.h"


class Level;

class Timer : public Thread {
public:
  explicit Timer(Physics& physics, Server& s_, Level& l_);
  virtual ~Timer();
  virtual void run();
private:
  bool runLevel();
  void finish();
  Mutex mutexRun;
  bool runContinue;
  Physics& physics;
  Server& s_;
  Level& l_;
};

#endif
