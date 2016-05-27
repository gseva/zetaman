#ifndef __PHYSICS_H___
#define __PHYSICS_H___
#include <Box2D/Box2D.h>
#include "zm/json/jsonserializer.h"

class World{
public:
  World();
  ~World();
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
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
private:
  World world;
  Ground ground;
  std::vector<b2Body*> stairways;
};

class Body {
public:
  explicit Body(Physics& physics);
  explicit Body(Physics& physics, float32 x, float32 y);
  virtual ~Body();
  void setPosition(int x, int y);
  b2Vec2 getPosition();
protected:
  Physics& physics;
  b2Body* body;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
};

class PlayerBody : public Body {
public:
  explicit PlayerBody(Physics& physics);
  virtual ~PlayerBody();
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
private:
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
