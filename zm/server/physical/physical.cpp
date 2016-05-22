#define DEFAULT_GRAVITY_X 0.0f
#define DEFAULT_GRAVITY_Y -10.0f
#include "zm/server/physical/physical.h"
#include <Box2D/Box2D.h>
#include <iostream>

Physical::Physical() : gorund(world){}

Physical::~Physical(){}

void Physical::step(){
  world.step();
}

b2Body* Physical::createBody(const b2BodyDef& bodyDef){
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
  //sleep(1/timeStep);
}

Ground::Ground(World& world) : world(world){
  //debe leer el JSON y crear el piso correspondiente
  groundBodyDef.position.Set(0.0f, -10.0f); //centro
  groundBody = world.createBody(groundBodyDef);
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);
}

Ground::~Ground(){}

PlayerBody::PlayerBody(Physical& physical) : physical(physical){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(2.0f, 4.0f);
  body = physical.createBody(bodyDef);
  b2CircleShape dynamicCircle;
  dynamicCircle.m_p.Set(2.0f, 3.0f);
  dynamicCircle.m_radius = 0.5f;
  fixtureDef.shape = &dynamicCircle;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  body->CreateFixture(&fixtureDef);
}

PlayerBody::~PlayerBody(){}

b2Vec2 PlayerBody::getPosition(){
  std::cout << body->GetPosition().y<<"\n--- "<< body->GetLinearVelocity().y;
  return body->GetPosition();
}

void PlayerBody::jump(){
  b2Vec2 vel = body->GetLinearVelocity();
  std::cout << "velocidad del jugador" << vel.y << "\n";
  b2Vec2 pos = body->GetPosition();
  std::cout << "velocidad del jugador" << pos.y << "\n";

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
