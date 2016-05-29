#ifndef __PHYSICS_H___
#define __PHYSICS_H___
#include <Box2D/Box2D.h>
#include "zm/json/jsonserializer.h"
#include "zm/thread.h"

class World{
public:
  World();
  ~World();
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
  void destroyBody(b2Body* body);
private:
  b2World* world;
  b2Vec2* gravity;
};

class Ground{
public:
  Ground();
  ~Ground();
  void addBlock(b2Body* blockBody);
private:
  std::vector<b2Body*> blocks;
};


class Physics {
public:
  Physics();
  ~Physics();
  void setMap(const JsonMap& jm);
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
  void destroyBody(b2Body* body);
  std::vector<b2Body*> stairways;
private:
  Mutex mutex;
  World world;
  Ground ground;
};

class Body {
public:
  explicit Body(Physics& physics);
  explicit Body(Physics& physics, float32 x, float32 y);
  virtual ~Body();
  void setPosition(int x, int y);
  b2Vec2 getPosition();
  b2Body* body;
protected:
  Mutex mutex;
  Physics& physics;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
  b2Fixture* fixture;
};

class Bullet : public Body {
public:
  explicit Bullet(Physics& physics, float32 x, float32 y, int signo);
  virtual ~Bullet();
  void move();
protected:
  const b2Vec2 vel;
};

class PlayerBody : public Body {
public:
  explicit PlayerBody(Physics& physics);
  explicit PlayerBody(Physics& physics, float32 x, float32 y);
  virtual ~PlayerBody();
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
  void up();
  Bullet* shoot();
private:
  bool canGoUp();
  bool idle;
};

class Enemy : public Body{
public:
  explicit Enemy(Physics& physics, float32 x, float32 y);
  virtual ~Enemy();
  void lived();
private:
  int amountMoves;
  const int totalMoves;
  int sig;
};

#endif
