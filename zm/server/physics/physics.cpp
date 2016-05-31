#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"
#include "zm/thread.h"

#define DEFAULT_GRAVITY_X 0.0f
#define DEFAULT_GRAVITY_Y -10.0f
#define ALTO_TOTAL 12
#define ANCHO_TOTAL 64
#define AIRE 0
#define SOLID "solid"
#define STAIR "stair"
#define PLAYER_TYPE 0x0001
#define ENEMY_TYPE 0x0002
#define BLOCK_TYPE 0x0003
#define STAIR_TYPE 0x0004
#define PLAYER_BULLET_TYPE 0x0005
#define ENEMY_BULLET_TYPE 0x0006
#define NONE_CONTACT 0x0000
#define ALL_CONTACT 0xffff


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
  Lock locker(mutex);
  return body->GetPosition();
}

void Body::setPosition(int x, int y){
  Lock locker(mutex);
  body->SetTransform(b2Vec2(x,y), body->GetAngle());
}

PlayerBody::PlayerBody(Physics& physics) : Body(physics){
  b2PolygonShape shape;
  shape.SetAsBox(0.4f, 0.4f);
  shape.m_radius = 0.1f;
  fixtureDef.shape = &shape;
  fixtureDef.density = 100.0f;
  fixtureDef.friction = 100.0f;
  fixtureDef.filter.categoryBits = PLAYER_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE & ~PLAYER_BULLET_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
}

PlayerBody::PlayerBody(Physics& physics, float32 x, float32 y) : 
  Body(physics, x, y){
  b2PolygonShape shape;
  shape.SetAsBox(0.4f, 0.4f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  fixtureDef.filter.categoryBits = PLAYER_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE;
  fixture = body->CreateFixture(&fixtureDef); 
}

PlayerBody::~PlayerBody(){}

void PlayerBody::jump(){
  Lock locker(mutex);
  b2Vec2 vel = body->GetLinearVelocity();
  if ( vel.y == 0 ) {
    vel.y += 6;
    body->SetLinearVelocity(vel);
    idle = false;
  }
}

void PlayerBody::right(){
  Lock locker(mutex);
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 6;
  body->SetLinearVelocity(vel);
}
void PlayerBody::left(){
  Lock locker(mutex);
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = -6;
  body->SetLinearVelocity(vel);
}

void PlayerBody::stopHorizontalMove(){
  Lock locker(mutex);
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 0;
  body->SetLinearVelocity(vel);
}

void PlayerBody::up(){
  if ( canGoUp() ) {
    b2Vec2 v = body->GetLinearVelocity();
    v.y = 3;
    Lock locker(mutex);
    body->SetLinearVelocity(v);
  }
}

bool PlayerBody::canGoUp(){
  Lock locker(mutex);
  std::vector<b2Body*>::iterator i;
  std::vector<b2Body*> stairways = this->physics.stairways;
  for ( i = stairways.begin(); i != stairways.end(); ++i ) {
    if ( b2TestOverlap((*i)->GetFixtureList()[0].GetShape(),
      0,fixture->GetShape(),0, (*i)->GetTransform(), body->GetTransform()) )
      return true;
  }
  return false;
}

Bullet* PlayerBody::shoot(){
  b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new PlayerBullet(this->physics, pos.x, pos.y,signo);
  return bullet;
}

bool PlayerBody::collide(Bullet* bullet){
  return bullet->collide(this);
}


Enemy::Enemy(Physics& physics, float32 x, float32 y) : Body(physics, x, y), 
  totalMoves(15) {
  b2PolygonShape shape;
  shape.SetAsBox(0.4f, 0.4f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 1.0f;
  fixtureDef.filter.categoryBits = ENEMY_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE & ~ENEMY_BULLET_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
  amountMoves = 0;
  b2Vec2 vel;
  sig = 1;
  vel.x = sig * 3;
  body->SetLinearVelocity(vel);
}

Enemy::~Enemy(){}

EnemyBullet* Enemy::move(){
  Lock locker(mutex);
  if ( amountMoves == totalMoves ) {
    sig *= -1;
    amountMoves = 0;
  } else {
    amountMoves++;
  }
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = 3 * sig;
    body->SetLinearVelocity(vel);
    return NULL;
}

bool Enemy::collide(Bullet* bullet){
  return bullet->collide(this);
}

Met::Met(Physics& physics, float32 x, float32 y) : 
  Enemy(physics, x, y), period(60*3){
    shoots = 0;
    tics = 0;
    state = notProtect;
}

Met::~Met(){}

EnemyBullet* Met::move(){
  tics++;
  if ( tics == period )
    tics = 0;
  if ( tics == 0 ){
    switch ( state ) {
      case notProtect: state = protect;
      break;
      case protect: state = notProtect;
      break;
    }
  }
  if ( state == notProtect && ((tics % 60) == 0 ) ) {
    return shoot();
  }
  return NULL;
}

EnemyBullet* Met::shoot(){
 b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  EnemyBullet* bullet = new EnemyBullet(this->physics, pos.x, pos.y,signo);
  //bullet->move();
  return bullet;
}

Bullet::Bullet(Physics& physics, float32 x, float32 y, int signo,
  int mask, int category) : Body(physics, x, y), vel(6*signo,0) {
  b2PolygonShape shape;
  shape.SetAsBox(0.01f, 0.01f);
  fixtureDef.shape = &shape;
  fixtureDef.filter.categoryBits = category;
  fixtureDef.filter.maskBits = mask;
  fixture = body->CreateFixture(&fixtureDef);
  body->SetLinearVelocity(vel);
  body->ApplyForce(b2Vec2(0,-DEFAULT_GRAVITY_Y), body->GetWorldCenter(), false);
}

Bullet::~Bullet(){}

void Bullet::move(){
  Lock locker(mutex);
  body->ApplyForce(b2Vec2(0,-DEFAULT_GRAVITY_Y), body->GetWorldCenter(), false);
}

bool Bullet::collide(Bullet* bullet){
  return false;
}

PlayerBullet::PlayerBullet(Physics& physics, float32 x, float32 y, int signo) :
  Bullet(physics, x, y, signo,
    ALL_CONTACT & ~STAIR_TYPE & ~PLAYER_TYPE, PLAYER_BULLET_TYPE){}

PlayerBullet::~PlayerBullet(){}


bool PlayerBullet::collide(Enemy* enemy){
  return true;
}

bool PlayerBullet::collide(PlayerBody* player){
  return false;
}

EnemyBullet::EnemyBullet(Physics& physics, float32 x, float32 y, int signo) :
  Bullet(physics, x, y, signo,
    ALL_CONTACT & ~STAIR_TYPE & ~ENEMY_TYPE, ENEMY_BULLET_TYPE){}

EnemyBullet::~EnemyBullet(){}

bool EnemyBullet::collide(Enemy* enemy){
  return false;
}

bool EnemyBullet::collide(PlayerBody* player){ 
  return true;
}
