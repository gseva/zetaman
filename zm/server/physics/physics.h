#ifndef __PHYSICS_H___
#define __PHYSICS_H___
#include <Box2D/Box2D.h>


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
  explicit Ground(World& world);
  ~Ground();
private:
  b2BodyDef groundBodyDef;
  b2Body* groundBody;
  b2PolygonShape groundBox;
  World& world;
  b2FixtureDef fixtureDef;
};


class Physics{
public:
  Physics();
  ~Physics();
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
private:
  World world;
  Ground ground;
};

class PlayerBody{
public:
  explicit PlayerBody(Physics& physics);
  ~PlayerBody();
  b2Vec2 getPosition();
  void setPosition(int x, int y);
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
private:
  Physics physics;
  bool idle;
  b2Body* body;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
};

class EnemyBody{
public:
  explicit EnemyBody(Physics& physics);
  ~EnemyBody();
  b2Vec2 getPosition();
  void setPosition(int x, int y);
private:
  Physics physics;
  bool idle;
  b2Body* body;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
};

#endif
