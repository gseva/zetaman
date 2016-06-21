#ifndef __POWERUP_H__
#define __POWERUP_H__

#include <Box2D/Box2D.h>
#include "zm/game_protocol.h"
#include "zm/server/physics/world.h"

class Player;

class PowerUp : public Body{
public:
  explicit PowerUp(Physics& physics, b2Vec2 pos, float size);
  virtual ~PowerUp();

  virtual bool applyTo(Player* p) = 0;
  virtual zm::proto::PowerUp toBean(int xo, int yo)=0;
};

class SmallEnergy : public PowerUp{
public:
  explicit SmallEnergy(Physics& physics, b2Vec2 pos);
  ~SmallEnergy();

  virtual bool applyTo(Player* p);
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

class LargeEnergy : public PowerUp{
public:
  explicit LargeEnergy(Physics& physics, b2Vec2 pos);
  ~LargeEnergy();

  virtual bool applyTo(Player* p);
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

class Life : public PowerUp{
public:
  explicit Life(Physics& physics, b2Vec2 pos);
  ~Life();

  virtual bool applyTo(Player* p);
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

class SmallPlasma : public PowerUp{
public:
  explicit SmallPlasma(Physics& physics, b2Vec2 pos);
  ~SmallPlasma();

  virtual bool applyTo(Player* p);
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

class LargePlasma : public PowerUp{
public:
  explicit LargePlasma(Physics& physics, b2Vec2 pos);
  ~LargePlasma();

  virtual bool applyTo(Player* p);
  virtual zm::proto::PowerUp toBean(int xo, int yo);
};

#endif
