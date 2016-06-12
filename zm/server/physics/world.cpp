#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>

#include "zm/thread.h"
#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/physics/world.h"

#define ALTO_TOTAL 12
#define ANCHO_TOTAL 64
#define AIRE 0
#define SOLID "solid"
#define STAIR "stair"


Physics::Physics() {}//: ground(world){}

Physics::~Physics() {}

void Physics::setMap(const JsonMap& jm){
  std::vector<int> matriz = jm.imageNumbers;
  for ( int j = 0; j < ALTO_TOTAL; ++j ) {
    for ( int i = 0; i < ANCHO_TOTAL; ++i ) {
      if ( jm.physics[matriz[i + j*ANCHO_TOTAL]] == SOLID ) {
          b2BodyDef blockBodyDef;// = new b2BodyDef();
          b2Body* blockBody;
          blockBodyDef.position.Set(i - 0.5f, ALTO_TOTAL - j - 0.5f); //centro
          blockBody = world.createBody(blockBodyDef);

          b2PolygonShape blockBox;// = new b2PolygonShape();
          blockBox.SetAsBox(0.5f, 0.5f);
          blockBox.m_radius = 0.1f;
          b2FixtureDef fixtureDef;
          fixtureDef.shape = &blockBox;
          fixtureDef.density = 0.0f;
          fixtureDef.friction = 0.0f;
          fixtureDef.filter.categoryBits = BLOCK_TYPE;
          fixtureDef.filter.maskBits = ALL_CONTACT;
          blockBody->CreateFixture(&fixtureDef);
          ground.addBlock(blockBody);
      } else if ( jm.physics[matriz[i + j*ANCHO_TOTAL]] == STAIR ) {
          b2BodyDef stairBodyDef;// = new b2BodyDef();
          b2Body* stairBody;
          stairBodyDef.position.Set(i - 0.5f, ALTO_TOTAL - j - 0.5f); //centro
          stairBody = world.createBody(stairBodyDef);

          b2PolygonShape stairshape;
          stairshape.SetAsBox(0.5f, 0.5f);
          b2FixtureDef stairFixtureDef;
          stairFixtureDef.shape = &stairshape;
          stairFixtureDef.density = 1.0f;
          stairFixtureDef.friction = 1.0f;
          stairFixtureDef.filter.categoryBits = STAIR_TYPE;
          stairFixtureDef.filter.maskBits = NONE_CONTACT | BLOCK_TYPE;
          stairBody->CreateFixture(&stairFixtureDef);
          stairways.push_back(stairBody);
      }
    }
  }
}


void Physics::step(){
  Lock locker(mutex);
  world.step();
}

b2Body* Physics::createBody(const b2BodyDef& bodyDef){
  Lock locker(mutex);
  return world.createBody(bodyDef);
}

void Physics::destroyBody(b2Body* body){
  world.destroyBody(body);
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

void World::destroyBody(b2Body* body){
  world->DestroyBody(body);
}

void World::step(){
  float32 timeStep = 1.0f / 60.0f; //60Hz
  int32 velocityIterations = 8; //valores sugeridos
  int32 positionIterations = 3;
  world->Step(timeStep, velocityIterations, positionIterations);
}

Ground::Ground(){}

Ground::~Ground(){}

void Ground::addBlock(b2Body* blockBody){
  blocks.push_back(blockBody);
}

Body::Body(Physics& physics) : physics(physics){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(2.5f, 1.5f);
  body = this->physics.createBody(bodyDef);
}

Body::Body(Physics& physics, float32 x, float32 y) : physics(physics){
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(x, y);
  body = this->physics.createBody(bodyDef);
 }

Body::~Body(){
  this->physics.destroyBody(body);
}

b2Vec2 Body::getPosition(){
  // Lock locker(mutex);
  return body->GetPosition();
}

void Body::setPosition(int x, int y){
  Lock locker(mutex);
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}
