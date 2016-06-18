#ifndef __ZM_SERVER_PHYSICS_BULLETS_H__
#define __ZM_SERVER_PHYSICS_BULLETS_H__

#include "zm/server/physics/world.h"
#include "zm/game_protocol.h"

class Enemy;
class PlayerBody;


class Bullet : public Body {
public:
  explicit Bullet(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy, float32 largo, float32 alto);
  explicit Bullet(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Bullet();
  virtual void move();
  virtual bool collide(Enemy* enemy);
  virtual bool collide(PlayerBody* player);
  virtual bool collide(Bullet* bullet);
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo,
    zm::proto::ProyectileType type=zm::proto::ProyectileType::Normal);
  virtual void impact();
protected:
  b2Vec2 vel;
  const bool isEnemy;
};

class Bomb : public Bullet{
public:
  explicit Bomb(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Bomb();
  virtual void move();
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
};

class Magnet : public Bullet{
public:
  explicit Magnet(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Magnet();
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
};

class Spark : public Bullet{
public:
  Spark(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  ~Spark();
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
};

class Ring : public Bullet{
public:
  explicit Ring(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Ring();
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
  virtual void impact();
private:
  int restCollisions;
};

class Fire : public Bullet{
public:
  explicit Fire(Physics& physics, float32 x, float32 y, int signo,
    bool isEnemy);
  virtual ~Fire();
  virtual zm::proto::Proyectile toBean(float32 xo, float32 yo);
};

#endif
