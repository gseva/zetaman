#ifndef __BOSS_H__
#define __BOSS_H__
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/gun.h"

class Boss : public Enemy{
public:
  explicit Boss(Physics& physics, float32 x, float32 y,
    int velocity, int jump, int shootFrecuency, int jumpFrecuency);
  virtual ~Boss()=0;
  virtual Bullet* move();
  virtual zm::proto::Enemy toBean(int xo, int yo)=0;
protected:
  int velocity;
  Gun* gun;
  const int jump;
  const int shootFrecuency;
  const int jumpFrecuency;
private:
  unsigned int tics; 
};

class Bombman : public Boss{
public:
  explicit Bombman(Physics& physics, float32 x, float32 y);
  virtual ~Bombman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Magnetman: public Boss{
public:
  explicit Magnetman(Physics& physics, float32 x, float32 y);
  virtual ~Magnetman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Sparkman : public Boss{
public:
  explicit Sparkman(Physics& physics, float32 x, float32 y);
  virtual ~Sparkman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Ringman : public Boss{
public:
  explicit Ringman(Physics& physics, float32 x, float32 y);
  virtual ~Ringman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Fireman : public Boss{
public:
  explicit Fireman(Physics& physics, float32 x, float32 y);
  virtual ~Fireman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

#endif
