
#ifndef __ZM_SERVER_PHYSICS_H__
#define __ZM_SERVER_PHYSICS_H__


#include "zm/server/physics/world.h"
#include "zm/server/physics/bullets.h"


class Enemy : public Body {
public:
  explicit Enemy(Physics& physics, float32 x, float32 y);
  virtual ~Enemy();
  virtual Bullet* move()=0;
  virtual bool collide(Bullet* bullet);
  virtual zm::proto::Enemy toBean(int xo, int yo)=0;
  virtual void toImpact(Bomb* bullet);
  virtual void toImpact(Spark* bullet);
  virtual void toImpact(Ring* bullet);
  virtual void toImpact(Magnet* bullet);
  virtual void toImpact(Fire* bullet);
  virtual void toImpact(Bullet* bullet);
};


class Met : public Enemy{
public:
  explicit Met(Physics& physics, float32 x, float32 y);
  virtual ~Met();
  virtual Bullet* move();
  Bullet* shoot();
  virtual zm::proto::Enemy toBean(int xo, int yo);
  virtual void toImpact(Ring* bullet);
  virtual void toImpact(Magnet* bullet);
  virtual void toImpact(Fire* bullet);
  virtual void toImpact(Bullet* bullet);
private:
  bool protected_;
  const float32 period;
  int tics;
  int shoots;
 };


 class Bumby : public Enemy{
 public:
    explicit Bumby(Physics& physics, float32 x, float32 y);
    virtual ~Bumby();
    virtual Bullet* move();
    Bullet* shoot();
    virtual zm::proto::Enemy toBean(int xo, int yo);
private:
  int amountMoves;
  const int totalMoves;
  int sig;
  const float32 period;
  int tics;
  int shoots;
 };

 class Sniper : public Enemy{
 public:
   explicit Sniper(Physics& physics, float32 x, float32 y);
   virtual ~Sniper();
   virtual Bullet* move();
    Bullet* shoot();
    virtual zm::proto::Enemy toBean(int xo, int yo);
    virtual void toImpact(Bomb* bullet);
    virtual void toImpact(Spark* bullet);
    virtual void toImpact(Magnet* bullet);
    virtual void toImpact(Bullet* bullet);
  private:
    bool protected_;
    int tics;
 };

 class JumpingSniper : public Enemy{
 public:
   explicit JumpingSniper(Physics& physics, float32 x, float32 y);
   virtual ~JumpingSniper();
   virtual Bullet* move();
    Bullet* shoot();
    virtual zm::proto::Enemy toBean(int xo, int yo);
    virtual void toImpact(Bomb* bullet);
    virtual void toImpact(Spark* bullet);
    virtual void toImpact(Magnet* bullet);
    virtual void toImpact(Bullet* bullet);  
 private:
    bool protected_;
    int tics;
 };

#endif
