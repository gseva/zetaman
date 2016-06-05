#ifndef __TIMER_H__
#define __TIMER_H__
#include <zm/thread.h>
#include <vector>
#include "zm/server/physics/physics.h"
#include "zm/server/player.h"
#include "zm/server/client_proxy.h"


class Timer : public Thread {
public:
  explicit Timer(Physics& physics, zm::ClientProxy& cp_,
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
  zm::ClientProxy& cp_;
  std::vector<Enemy*>& enemies;
  std::vector<Bullet*>& bullets;
  std::vector<Player*>& players;
};

#endif
