#include <stdlib.h>
#include "zm/server/physics/bullets.h"
#include "zm/server/physics/players.h"
#include "zm/server/physics/enemies.h"
#define RING_COLLISIONS 3

bool randomBool() {
  return rand() % 2 == 1;
}

Bullet::Bullet(Physics& physics, float32 x, float32 y, int signo, bool isEnemy,
  float32 largo, float32 alto)
    : Body(physics, x, y, BodyType::Bullet), vel(6*signo,0), isEnemy(isEnemy) {
  b2PolygonShape shape;
  shape.SetAsBox(largo, alto);
  fixtureDef.shape = &shape;

  int mask, category;
  if ( !isEnemy ){
    mask = ALL_CONTACT & ~STAIR_TYPE & ~PLAYER_TYPE;
    category = PLAYER_BULLET_TYPE;
  } else {
    mask = ALL_CONTACT & ~STAIR_TYPE & ~ENEMY_TYPE;
    category = ENEMY_BULLET_TYPE;
  }

  fixtureDef.filter.categoryBits = category;
  fixtureDef.filter.maskBits = mask;
  fixture = body->CreateFixture(&fixtureDef);
  body->SetLinearVelocity(vel);
  body->ApplyForce(b2Vec2(0, -DEFAULT_GRAVITY_Y),
                  body->GetWorldCenter(), false);
  body->SetUserData(this);
}

Bullet::Bullet(Physics& physics, float32 x, float32 y, int signo, bool isEnemy)
  : Bullet(physics, x, y, signo, isEnemy, 0.01f, 0.01f) {}

Bullet::~Bullet(){
}

void Bullet::move() {
  if (!destroyed) {
    body->ApplyForce(b2Vec2(0, -DEFAULT_GRAVITY_Y),
                     body->GetWorldCenter(), false);
  }
}

bool Bullet::collide(Enemy* enemy){
  return !isEnemy;
}

bool Bullet::collide(PlayerBody* player){
  return isEnemy;
}

bool Bullet::collide(Bullet* player){
  return false;
}

void Bullet::impact(){
  markAsDestroyed();
}

void Bullet::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}

void Bullet::impact(PlayerBody* impactBody){
  impactBody->toImpact(this);
}

zm::proto::Proyectile Bullet::toBean(float32 xo, float32 yo,
  zm::proto::ProyectileType type){
  zm::proto::Proyectile proyectile;
  proyectile.type = type;
  proyectile.pos.x = getPosition().x - xo;
  proyectile.pos.y = getPosition().y;
  return proyectile;
}

zm::proto::Proyectile Bullet::toBean(float32 xo, float32 yo){
  zm::proto::ProyectileType type = zm::proto::ProyectileType::Normal;
  zm::proto::Proyectile proyectile = toBean(xo, yo, type);
  return proyectile;
}

Bomb::Bomb(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Bullet(physics, x, y, signo, isEnemy) {
  vel.x = signo == 1 ? 6 : -6;
  vel.y = 2;
  body->SetLinearVelocity(vel);
}

void Bomb::move() {}

Bomb::~Bomb(){}

zm::proto::Proyectile Bomb::toBean(float32 xo, float32 yo){
 zm::proto::Proyectile proyectile = Bullet::toBean(xo, yo,
    zm::proto::ProyectileType::Bomb);
  return proyectile;
}

void Bomb::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}

Magnet::Magnet(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Bullet(physics,x,y,signo,isEnemy){}

Magnet::~Magnet(){}

zm::proto::Proyectile Magnet::toBean(float32 xo, float32 yo){
  zm::proto::Proyectile proyectile = Bullet::toBean(xo, yo,
    zm::proto::ProyectileType::Magnet);
  return proyectile;
}

void Magnet::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}

Spark::Spark(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Bullet(physics,x,y,signo,isEnemy){
  vel.x = signo == 1 ? 6 : -6;
  vel.y = randomBool() ? 0 : 6;
  body->SetLinearVelocity(vel);
}

Spark::~Spark(){}

zm::proto::Proyectile Spark::toBean(float32 xo, float32 yo){
  zm::proto::Proyectile proyectile = Bullet::toBean(xo, yo,
    zm::proto::ProyectileType::Spark);
  return proyectile;
}

void Spark::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}

Ring::Ring(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Bullet(physics,x,y,signo,isEnemy){
  restCollisions = RING_COLLISIONS;
}

Ring::~Ring(){}

zm::proto::Proyectile Ring::toBean(float32 xo, float32 yo){
  zm::proto::Proyectile proyectile = Bullet::toBean(xo, yo,
    zm::proto::ProyectileType::Ring);
  return proyectile;
}

void Ring::impact(){
  if ( restCollisions == 0 ) {
    markAsDestroyed();
  } else {
    restCollisions --;
    b2Vec2 vel = body->GetLinearVelocity();
    vel.x = -vel.x;
    body->SetLinearVelocity(vel);
  }
}

void Ring::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}

Fire::Fire(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Bullet(physics,x,y,signo,isEnemy,2.0f,0.01f){}

Fire::~Fire(){}

zm::proto::Proyectile Fire::toBean(float32 xo, float32 yo){
  zm::proto::Proyectile proyectile = Bullet::toBean(xo, yo,
    zm::proto::ProyectileType::Fire);
  return proyectile;
}

void Fire::impact(Enemy* impactBody){
  impactBody->toImpact(this);
}
