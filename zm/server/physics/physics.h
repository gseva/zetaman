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

/*class Ground{
public:
  explicit Ground(World& world);
  ~Ground();
private:
  b2BodyDef groundBodyDef;
  b2Body* groundBody;
  b2PolygonShape groundBox;
  World& world;
};*/


class Physics {
public:
  Physics();
  ~Physics();
  void setMap(const JsonMap& jm);
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
private:
  World world;
//  Ground ground;
};

class Body {
public:
  explicit Body(Physics& physics);
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

#endif
