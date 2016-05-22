#ifndef __PHYSICAL_H___
#define __PHYSICAL_H___
#include <Box2D/Box2D.h>


class World{
public:
  World();
  ~World();
  void step();
  b2World* world;
  b2Body* createBody(const b2BodyDef& bodyDef);
private:
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
};


class Physical{
public:
  Physical();
  ~Physical();
  void step();
  b2Body* createBody(const b2BodyDef& bodyDef);
private:
  World world;
  Ground gorund;
};

class PlayerBody{
public:
  explicit PlayerBody(Physical& physical);
  ~PlayerBody();
  b2Vec2 getPosition();
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
private:
  Physical physical;
  bool idle;
  b2Body* body;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
};

#endif
