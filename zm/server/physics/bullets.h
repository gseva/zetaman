
#ifndef __ZM_SERVER_PHYSICS_BULLETS_H__
#define __ZM_SERVER_PHYSICS_BULLETS_H__


#include "zm/server/physics/world.h"


class Enemy;
class PlayerBody;


class Bullet : public Body {
public:
  explicit Bullet(Physics& physics, float32 x, float32 y, int signo,
    int mask, int category);
  virtual ~Bullet()=0;
  void move();
  virtual bool collide(Enemy* enemy)=0;
  virtual bool collide(PlayerBody* player)=0;
  virtual bool collide(Bullet* bullet);

  bool isDestroyed();
  void destroy();
protected:
  const b2Vec2 vel;
  bool destroyed;
};

class PlayerBullet : public Bullet {
public:
  explicit PlayerBullet(Physics& physics, float32 x, float32 y, int signo);
  virtual ~PlayerBullet();
  virtual bool collide(Enemy* enemy);
  virtual bool collide(PlayerBody* player);
};

class EnemyBullet : public Bullet {
public:
  explicit EnemyBullet(Physics& physics, float32 x, float32 y, int signo);
  ~EnemyBullet();
  virtual bool collide(Enemy* enemy);
  virtual bool collide(PlayerBody* player);
};


#endif
