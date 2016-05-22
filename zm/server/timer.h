#ifndef __TIMER_H__
#define __TIMER_H__
#include <zm/thread.h>
#include "zm/server/physics/physics.h"
//#include "zm/server_proxy.h"

class ServerProxy;

class Timer : public Thread{
public:
  explicit Timer(Physics& physics, ServerProxy& sp);
  virtual ~Timer();
  virtual void run();
private:
  Physics& physics;
  ServerProxy& sp;
};

#endif
