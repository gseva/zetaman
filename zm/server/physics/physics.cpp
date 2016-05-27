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
#define STAIR 2
#define PLAYER_TYPE 0x0001
#define ENEMY_TYPE 0x0002
#define BLOCK_TYPE 0x0003
#define STAIR_TYPE 0x0004
#define NONE_CONTACT 0x0000
#define ALL_CONTACT 0xffff


Physics::Physics() {}//: ground(world){}

Physics::~Physics() {}

void Physics::setMap(const JsonMap& jm){
  std::vector<int> matriz = jm.imageNumbers;
  for ( int j = 0; j < ALTO_TOTAL; ++j ) {
    for ( int i = 0; i < ANCHO_TOTAL; ++i ) {
      if ( matriz[i + j*ANCHO_TOTAL] == PISO ) {
          b2BodyDef blockBodyDef;// = new b2BodyDef();
          b2Body* blockBody;
          blockBodyDef.position.Set(i - 0.5f, ALTO_TOTAL - j - 0.5f); //centro
          blockBody = world.createBody(blockBodyDef);
          
          b2PolygonShape blockBox;// = new b2PolygonShape();
          blockBox.SetAsBox(0.5f, 0.5f);
          b2FixtureDef fixtureDef;
          fixtureDef.shape = &blockBox;
          fixtureDef.density = 1.0f;
          fixtureDef.friction = 1.0f;
          fixtureDef.filter.categoryBits = BLOCK_TYPE;
          fixtureDef.filter.maskBits = ALL_CONTACT;
          blockBody->CreateFixture(&fixtureDef);
          ground.addBlock(blockBody);
      } else if ( matriz[i + j*ANCHO_TOTAL] == STAIR ) {
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

Body::~Body(){}

b2Vec2 Body::getPosition(){
  return body->GetPosition();
}

void Body::setPosition(int x, int y){
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}

PlayerBody::PlayerBody(Physics& physics) : Body(physics){
  b2PolygonShape shape;
  shape.SetAsBox(0.5f, 0.5f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 1.0f;
  fixtureDef.filter.categoryBits = PLAYER_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE;
  body->CreateFixture(&fixtureDef);
}
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

Enemy::Enemy(Physics& physics, float32 x, float32 y) : Body(physics, x, y), 
  totalMoves(15) {
  b2PolygonShape shape;
  shape.SetAsBox(0.5f, 0.5f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 1.0f;
  fixtureDef.filter.categoryBits = ENEMY_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE;
  body->CreateFixture(&fixtureDef);
  amountMoves = 0;
  b2Vec2 vel;
  sig = 1;
  vel.x = sig * 3;
  body->SetLinearVelocity(vel);
}

Enemy::~Enemy(){}

void Enemy::lived(){
  if ( amountMoves == totalMoves ) {
    sig *= -1;
    amountMoves = 0;
  } else {
    amountMoves++;
  }
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = 3 * sig;
    body->SetLinearVelocity(vel);
}
