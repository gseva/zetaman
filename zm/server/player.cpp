#include "zm/server/player.h"
#include "zm/server/physical/physical.h"
#include <Box2D/Box2D.h>
#include <iostream>

#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Player::Player(Physical& physical) : vy_(0), idle(true),
  physical(physical), body(physical){}

Player::~Player(){}

void Player::createBody(b2World* world){}

void Player::jump(){
  body.jump();
}

void Player::setPosition(int x, int y){
  //playerBody->SetTransform(b2Vec2(x,y),playerBody->GetAngle());
}

void Player::right(){
  body.right();
  //b2Vec2 vel = body->GetLinearVelocity();
  //vel.x = 10;
  //body->SetLinearVelocity(vel);
}

void Player::left(){
  body.left();
  //b2Vec2 vel = body->GetLinearVelocity();
  //vel.x = -10;
  //body->SetLinearVelocity(vel);
}

void Player::stopHorizontalMove(){
  body.stopHorizontalMove();
  //b2Vec2 vel = body->GetLinearVelocity();
  //vel.x = 0;
  //body->SetLinearVelocity(vel); 
}

b2Vec2 Player::getPosition(){
  return body.getPosition();
  //b2Vec2 a(1,1);
  //return a;
}
