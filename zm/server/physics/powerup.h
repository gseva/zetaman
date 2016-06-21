#ifndef __POWERUP_H__
#define __POWERUP_H__

#include <Box2D/Box2D.h>
#include "zm/game_protocol.h"
#include "zm/server/physics/world.h"

class PowerUp : public Body{
public:
  explicit PowerUp(Physics& physics, b2Vec2 pos);
  virtual ~PowerUp();
  virtual zm::proto::PowerUp toBean(int xo, int yo)=0;
};

class SmallEnergy : public PowerUp{
public:
  explicit SmallEnergy(Physics& physics, b2Vec2 pos);
  ~SmallEnergy();
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

#endif
