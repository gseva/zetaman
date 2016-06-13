
#ifndef __ZM_SERVER_PHYSICS_BULLETS_H__
#define __ZM_SERVER_PHYSICS_BULLETS_H__


#include "zm/server/physics/world.h"


class Enemy;
class PlayerBody;


class Bullet : public Body {
public:
  explicit Bullet(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Bullet();
  void move();
  virtual bool collide(Enemy* enemy);
  virtual bool collide(PlayerBody* player);
  virtual bool collide(Bullet* bullet);
protected:
  const b2Vec2 vel;
  const bool isEnemy;
};

/*class PlayerBullet : public Bullet {
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
*/

class Boomb : public Bullet{
public:
  explicit Boomb(Physics& physics, float32 x, float32 y, int signo);
  virtual ~Boomb();
  
};

class Magnet : public Bullet{
public:
  explicit Magnet(Physics& physics, float32 x, float32 y, int signo);
  virtual ~Magnet();
  
};

class Spark : public Bullet{
public:
  Spark(Physics& physics, float32 x, float32 y, int signo);
  ~Spark();
  
};

class Ring : public Bullet{
public:
  explicit Ring(Physics& physics, float32 x, float32 y, int signo);
  virtual ~Ring();
  
};

class Fire : public Bullet{
public:
  explicit Fire(Physics& physics, float32 x, float32 y, int signo);
  virtual ~Fire();
  
};

#endif
