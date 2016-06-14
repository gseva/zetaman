#ifndef __ZM_SERVER_PHYSICS_H___
#define __ZM_SERVER_PHYSICS_H___

#include <Box2D/Box2D.h>

#include "zm/thread.h"
#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"


#define DEFAULT_GRAVITY_X 0.0f
#define DEFAULT_GRAVITY_Y -10.0f
#define PLAYER_TYPE 0x0001
#define ENEMY_TYPE 0x0002
#define BLOCK_TYPE 0x0003
#define STAIR_TYPE 0x0004
#define PLAYER_BULLET_TYPE 0x0005
#define ENEMY_BULLET_TYPE 0x0006
#define NONE_CONTACT 0x0000
#define ALL_CONTACT 0xffff

class PlayerBody;
class Enemy;
class Bullet;

enum class BodyType {
  Enemy, Player, Bullet
};


class World{
public:
  World();
  ~World();

  b2Body* createBody(const b2BodyDef& bodyDef);
  void destroyBody(b2Body* body);

  void step();
  void clean();
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
  explicit Body(Physics& physics, float32 x, float32 y, BodyType t);
  virtual ~Body();
  void setPosition(int x, int y);
  b2Vec2 getPosition();
  b2Body* body;
  virtual bool collide(Bullet* bullet)=0;

  void markAsDestroyed();
  bool isDestroyed();
  void impact();

  BodyType type;
protected:
  Mutex mutex;
  Physics& physics;
  b2BodyDef bodyDef;
  b2FixtureDef fixtureDef;
  b2Fixture* fixture;

  bool destroyed;
};

#endif
