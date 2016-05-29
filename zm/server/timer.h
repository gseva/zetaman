#ifndef __TIMER_H__
#define __TIMER_H__
#include <zm/thread.h>
#include <vector>
#include "zm/server/physics/physics.h"
//#include "zm/server_proxy.h"

class ServerProxy;

class Timer : public Thread{
public:
  explicit Timer(Physics& physics, ServerProxy& sp,
    std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);
  virtual ~Timer();
  virtual void run();
private:
  void collides(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);
  Physics& physics;
  ServerProxy& sp;
  std::vector<Enemy*>& enemies;
  std::vector<Bullet*>& bullets;
};

#endif
