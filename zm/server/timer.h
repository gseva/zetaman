#ifndef __TIMER_H__
#define __TIMER_H__
#include <vector>

#include <zm/thread.h>
#include "zm/server/player.h"
#include "zm/server/client_proxy.h"
#include "zm/server/physics/world.h"
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/bullets.h"


class Timer : public Thread {
public:
  explicit Timer(Physics& physics, Server& s_,
    std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets,
    std::vector<Player*>& players);
  virtual ~Timer();
  virtual void run();
private:
  void collides(std::vector<Enemy*>& enemies, std::vector<Bullet*>& bullets);
  bool runLevel();
  void finish();
  Mutex mutexRun;
  bool runContinue;
  Physics& physics;
  Server& s_;
  std::vector<Enemy*>& enemies;
  std::vector<Bullet*>& bullets;
  std::vector<Player*>& players;
};

#endif
