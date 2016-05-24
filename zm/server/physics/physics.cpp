#include <Box2D/Box2D.h>
#include <iostream>
#include "zm/server/physics/physics.h"
#define DEFAULT_GRAVITY_X 0.0f
#define DEFAULT_GRAVITY_Y -10.0f

Physics::Physics() : ground(world){}

Physics::~Physics(){}

void Physics::step(){
  world.step();
}

b2Body* Physics::createBody(const b2BodyDef& bodyDef){
  return world.createBody(bodyDef);
}


World::World(){
  gravity = new b2Vec2(DEFAULT_GRAVITY_X, DEFAULT_GRAVITY_Y);
  world = new b2World(*gravity);
}

World::~World(){
  delete gravity;
  delete world;
}

b2Body* World::createBody(const b2BodyDef& bodyDef){
  return world->CreateBody(&bodyDef);
}

void World::step(){
  float32 timeStep = 1.0f / 60.0f; //60Hz
  int32 velocityIterations = 8; //valores sugeridos
  int32 positionIterations = 3; 
  world->Step(timeStep, velocityIterations, positionIterations);
}

Ground::Ground(World& world) : world(world){
  //debe leer el JSON y crear el piso correspondiente
  groundBodyDef.position.Set(0.0f, -10.0f); //centro
  groundBody = world.createBody(groundBodyDef);
  groundBox.SetAsBox(50.0f, 10.0f);
  fixtureDef.shape = &groundBox;
  fixtureDef.density = 0.0f;
  fixtureDef.filter.categoryBits = 0x0003;
  //fixtureDef.filter.maskBits = 0x0001 | 0x0002;
  groundBody->CreateFixture(&fixtureDef);
}

Ground::~Ground(){}

PlayerBody::PlayerBody(Physics& physics) : physics(physics){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(2.0f, 4.0f);
  body = this->physics.createBody(bodyDef);
  b2CircleShape dynamicCircle;
  dynamicCircle.m_p.Set(2.0f, 3.0f);
  dynamicCircle.m_radius = 0.5f;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  fixtureDef.filter.categoryBits = 0x0001;
  fixtureDef.filter.maskBits = 0xffff & ~0x0002;
  body->CreateFixture(&fixtureDef);
}

PlayerBody::~PlayerBody(){}

b2Vec2 PlayerBody::getPosition(){
  return body->GetPosition();
}

void PlayerBody::setPosition(int x, int y){
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}

void PlayerBody::jump(){
  b2Vec2 vel = body->GetLinearVelocity();
  if ( vel.y == 0 ) {
    vel.y += 10;
    body->SetLinearVelocity(vel);
    idle = false;
  }
}

void PlayerBody::right(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 10;
  body->SetLinearVelocity(vel);
}
void PlayerBody::left(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = -10;
  body->SetLinearVelocity(vel);
}
void PlayerBody::stopHorizontalMove(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 0;
  body->SetLinearVelocity(vel);
}

EnemyBody::EnemyBody(Physics& physics) : physics(physics){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(3.0f, 4.0f);
  body = this->physics.createBody(bodyDef);
  b2CircleShape dynamicCircle;
  dynamicCircle.m_p.Set(3.0f, 3.0f);
  dynamicCircle.m_radius = 0.5f;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  fixtureDef.filter.categoryBits = 0x0002;
  fixtureDef.filter.maskBits = 0xffff & ~0x0001;
  body->CreateFixture(&fixtureDef);
}

EnemyBody::~EnemyBody(){}

b2Vec2 EnemyBody::getPosition(){
  return body->GetPosition();
}

void EnemyBody::setPosition(int x, int y){
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}
