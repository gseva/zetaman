#include "zm/server/player.h"
#include <Box2D/Box2D.h>

#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Player::Player() : vy_(0), idle(true) {}
Player::~Player(){}

void Player::createBody(b2World* world){
  playerBodyDef.type = b2_dynamicBody;
  playerBodyDef.position.Set(2.0f, 4.0f);
  palyerBody = world->CreateBody(&playerBodyDef);
  b2CircleShape dynamicCircle;
  dynamicCircle.m_p.Set(2.0f, 3.0f);
  dynamicCircle.m_radius = 0.5f;
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  palyerBody->CreateFixture(&fixtureDef);
  this->body = palyerBody;
  this->setPosition(2.0f, 3.0f);
}

void Player::jump(){
  b2Vec2 vel = body->GetLinearVelocity();
  if ( vel.y == 0 ) {
    vel.y += 10;
    body->SetLinearVelocity(vel);
  	vy_ = VY_MAX_LIM;
    idle = false;
  }
}

void Player::setPosition(int x, int y){
	x_ = x;
	y_ = y;
}

/*void Player::right(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 10;
  body->SetLinearVelocity(vel);
}*/
