#ifndef __ENEMY_H__
#define __ENEMY_H__
#include <Box2D/Box2D.h>
#include "zm/server/physics/physics.h"

class Enemy{
public:
  explicit Enemy(Physics& physics);
  ~Enemy();
  void setPosition(int x, int y);
  void createBody(b2World* world);
  b2Vec2 getPosition();
private:
  EnemyBody body;
};

#endif
