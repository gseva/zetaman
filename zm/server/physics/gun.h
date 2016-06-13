#ifndef __GUN_H__
#define __GUN_H__

#include "zm/server/physics/world.h"
#include "zm/server/physics/bullets.h"

class Gun{
public:
  explicit Gun(Body* proprietor, bool isEnemy, int ticsToCharge,
    Physics& physics);
  virtual ~Gun()=0;
  Bullet* shoot();
  virtual Bullet* fire()=0;
  void tic();
protected:
  const int ticsToCharge;
  int tics;
  bool used;
  bool isEnemy;
  Body* proprietor;
  Physics& physics;
};

class Bombgun : public Gun{
public:
  explicit Bombgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Bombgun();
  virtual Bullet* fire();
};

class Magnetgun : public Gun{
public:
  explicit Magnetgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Magnetgun();
  virtual Bullet* fire();
};

class Sparkgun : public Gun{
public:
  explicit Sparkgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Sparkgun();
  virtual Bullet* fire();
};

class Ringgun : public Gun{
public:
  explicit Ringgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Ringgun();
  virtual Bullet* fire();
};

class Firegun : public Gun{
public:
  explicit Firegun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Firegun();  
  virtual Bullet* fire();
};

#endif
