#ifndef __GUN_H__
#define __GUN_H__

#include "zm/server/physics/world.h"
#include "zm/server/physics/bullets.h"

class Gun{
public:
  explicit Gun(Body* proprietor, bool isEnemy, int ticsToCharge,
    Physics& physics, int ammunitions = 10);
  virtual ~Gun()=0;
  virtual Bullet* shoot();
  virtual Bullet* shoot(int direction);
  virtual Bullet* fire()=0;
  virtual Bullet* fire(int direction)=0;
  void tic();
  virtual int getNumber()=0;
protected:
  const int ticsToCharge;
  int tics;
  bool used;
  bool isEnemy;
  Body* proprietor;
  Physics& physics;
  int ammunitions;
};

class Normalgun : public Gun{
public:
  explicit Normalgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Normalgun();
  virtual Bullet* shoot();
  virtual Bullet* shoot(int direction);
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

class Bombgun : public Gun{
public:
  explicit Bombgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Bombgun();
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

class Magnetgun : public Gun{
public:
  explicit Magnetgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Magnetgun();
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

class Sparkgun : public Gun{
public:
  explicit Sparkgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Sparkgun();
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

class Ringgun : public Gun{
public:
  explicit Ringgun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Ringgun();
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

class Firegun : public Gun{
public:
  explicit Firegun(Body* proprietor, bool isEnemy, Physics& physics);
  virtual ~Firegun();  
  virtual Bullet* fire();
  virtual Bullet* fire(int direction);
  virtual int getNumber();
};

#endif
