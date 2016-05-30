#include "zm/server/player.h"
#include "zm/server/physics/physics.h"
#include <Box2D/Box2D.h>
#include <iostream>


#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Player::Player(){}

Player::~Player(){
  if ( body != NULL )
    delete body;
}

void Player::createBody(Physics* physics, float32 x, float32 y){
  body = new PlayerBody(*physics,x,y);
}

void Player::jump(){
  body->jump();
}

b2Vec2 Player::getPosition(){
  return body->getPosition();
}

void Player::setPosition(int x, int y){
  body->setPosition(x,y);
}

void Player::right(){
  body->right();
}

void Player::left(){
  body->left();
}

void Player::stopHorizontalMove(){
  body->stopHorizontalMove();
}

void Player::up(){
  body->up();
}

Bullet* Player::shoot(){
  Bullet* bullet = body->shoot();
  return bullet;
}
