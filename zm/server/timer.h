#ifndef __TIMER_H__
#define __TIMER_H__
#include <zm/thread.h>
#include <vector>
#include "zm/server/physics/physics.h"
#include "zm/server/client_proxy.h"


class Timer : public Thread {
public:
  explicit Timer(Physics& physics, Server& s_,
    std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);
  virtual ~Timer();
  virtual void run();
private:
  void collides(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);

  Physics& physics;
  Server& s_;
  std::vector<Enemy*>& enemies;
  std::vector<Bullet*>& bullets;
};

#endif
