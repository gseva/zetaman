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
  const int velocity;
  Gun* gun;
  const int jump;
  const int shootFrecuency;
  const int jumpFrecuency;
private:
  unsigned int tics; 
};

class Boombman : public Boss{
public:
  explicit Boombman(Physics& physics, float32 x, float32 y);
  virtual ~Boombman();
};

class Magnetman: public Boss{
public:
  explicit Magnetman(Physics& physics, float32 x, float32 y);
  virtual ~Magnetman();
};

class Sparkman : public Boss{
public:
  explicit Sparkman(Physics& physics, float32 x, float32 y);
  virtual ~Sparkman();
};

class Ringman : public Boss{
public:
  explicit Ringman(Physics& physics, float32 x, float32 y);
  virtual ~Ringman();
};

class Fireman : public Boss{
public:
  explicit Fireman(Physics& physics, float32 x, float32 y);
  virtual ~Fireman();
};

#endif
