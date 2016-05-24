#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_GRAVITY_X 0.0f
#define DEFAULT_GRAVITY_Y -10.0f
#define ALTO_TOTAL 12
#define ANCHO_TOTAL 16
#define AIRE 0
#define PISO 1


Physics::Physics() {}//: ground(world){}

Physics::~Physics() {}

void Physics::setMap(const JsonMap& jm){
  std::vector<int> matriz = jm.imageNumbers;
  for ( int j = 0; j < ALTO_TOTAL; ++j ) {
    for ( int i = 0; i < ANCHO_TOTAL; ++i ) {
      switch ( matriz[i + j*ANCHO_TOTAL] ){
        case PISO:
          b2BodyDef blockBodyDef;// = new b2BodyDef();
          b2Body* blockBody;
          b2PolygonShape blockBox;// = new b2PolygonShape();
          blockBodyDef.position.Set(i - 0.5f, ALTO_TOTAL - j - 0.5f); //centro
          blockBody = world.createBody(blockBodyDef);
          blockBox.SetAsBox(0.5f, 0.5f);
          blockBody->CreateFixture(&blockBox, 0.0f);
          break;
      }
    }
  }
}


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

/*Ground::Ground(World& world) : world(world){
  //debe leer el JSON y crear el piso correspondiente
  groundBodyDef.position.Set(0.0f, -10.0f); //centro
  groundBody = world.createBody(groundBodyDef);
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);
}*/

//Ground::~Ground(){}

Body::Body(Physics& physics) : physics(physics){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(2.5f, 1.5f);
  body = this->physics.createBody(bodyDef);
  b2PolygonShape shape;
  shape.SetAsBox(0.5f, 0.5f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  body->CreateFixture(&fixtureDef);
}

Body::~Body(){}

b2Vec2 Body::getPosition(){
  return body->GetPosition();
}

void Body::setPosition(int x, int y){
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}

PlayerBody::PlayerBody(Physics& physics) : Body(physics){}
PlayerBody::~PlayerBody(){}

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
