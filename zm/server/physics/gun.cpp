#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"

#define NORMAL_CONSUME_AMMUNITION 0
#define BOMB_CONSUME_AMMUNITION   1
#define SPARK_CONSUME_AMMUNITION  3
#define RING_CONSUME_AMMUNITION   4
#define MAGNET_CONSUME_AMMUNITION 3
#define FIRE_CONSUME_AMMUNITION   5

Gun::Gun(Body* proprietor, bool isEnemy, int ticsToCharge, Physics& physics,int ammunitions) :
  ticsToCharge(ticsToCharge), physics(physics){
  tics = 0;
  used = false;
  this->isEnemy = isEnemy;
  this->proprietor = proprietor;
  this->ammunitions = ammunitions;
}

Gun::~Gun(){}

Bullet* Gun::shoot(){
  if ( tics != 0 || used )
    return NULL; // si ya se disparo y aun no se recargo
  if (!isEnemy && ammunitions <= 0)
    return NULL;
  return fire();
}

Bullet* Gun::shoot(int direction){
  if ( tics != 0 || used )
    return NULL; // si ya se disparo y aun no se recargo
  if ( !isEnemy && ammunitions <= 0 )
    return NULL;
  consumeAmmunition();
  return fire(direction);
}

void Gun::tic(){
  tics++;
  if ( tics == ticsToCharge ) {
    tics = 0;
    used = false;
  }
}

void Gun::addAmmunitions(int amount){
  ammunitions += amount;
}

Normalgun::Normalgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 2, physics){}

Normalgun::~Normalgun(){}

Bullet* Normalgun::shoot(){
  if ( tics != 0 || used )
    return NULL; // si ya se disparo y aun no se recargo
  return fire();
}

Bullet* Normalgun::shoot(int direction){
  if ( tics != 0 || used )
    return NULL; // si ya se disparo y aun no se recargo
  return fire(direction);
}

Bullet* Normalgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Normalgun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Normalgun::getNumber(){
  return 0;
}

void Normalgun::consumeAmmunition(){
  ammunitions -= NORMAL_CONSUME_AMMUNITION;
}

Bombgun::Bombgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bombgun::~Bombgun(){}

Bullet* Bombgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bomb(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Bombgun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Bomb(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Bombgun::getNumber(){
  return 1;
}

void Bombgun::consumeAmmunition(){
  ammunitions -= BOMB_CONSUME_AMMUNITION; 
}

Magnetgun::Magnetgun(Body* proprietor, bool isEnemy, Physics& physics) :
  Gun(proprietor, isEnemy, 10, physics){}

Magnetgun::~Magnetgun(){}

Bullet* Magnetgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Magnet(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Magnetgun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Magnet(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Magnetgun::getNumber(){
  return 2;
}


void Magnetgun::consumeAmmunition(){
  ammunitions -= MAGNET_CONSUME_AMMUNITION; 
}

Sparkgun::Sparkgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Sparkgun::~Sparkgun(){}

Bullet* Sparkgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Spark(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Sparkgun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Spark(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Sparkgun::getNumber(){
  return 3;
}


void Sparkgun::consumeAmmunition(){
  ammunitions -= MAGNET_CONSUME_AMMUNITION; 
}

Ringgun::Ringgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Ringgun::~Ringgun(){}

Bullet* Ringgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Ring(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Ringgun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Ring(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Ringgun::getNumber(){
  return 4;
}

void Ringgun::consumeAmmunition(){
  ammunitions -= RING_CONSUME_AMMUNITION; 
}

Firegun::Firegun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Firegun::~Firegun(){}

Bullet* Firegun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Fire(this->physics, pos.x, pos.y, signo, isEnemy);
  return bullet;
}

Bullet* Firegun::fire(int direction){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  Bullet* bullet = new Fire(this->physics, pos.x, pos.y, direction, isEnemy);
  return bullet;
}

int Firegun::getNumber(){
  return 5;
}

void Firegun::consumeAmmunition(){
  ammunitions -= FIRE_CONSUME_AMMUNITION; 
}
