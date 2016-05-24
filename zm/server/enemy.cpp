#include "zm/server/enemy.h"
#include "zm/server/physics/physics.h"
#include <Box2D/Box2D.h>
#include <iostream>

#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Enemy::Enemy(Physics& physics) : body(physics){}

Enemy::~Enemy(){}

void Enemy::createBody(b2World* world){}

b2Vec2 Enemy::getPosition(){
  return body.getPosition();
}

void Enemy::setPosition(int x, int y){
  body.setPosition(x,y);
  //playerBody->SetTransform(b2Vec2(x,y),playerBody->GetAngle());
}
