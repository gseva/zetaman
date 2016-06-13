#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"


Gun::Gun(Body* proprietor, bool isEnemy, int ticsToCharge, Physics& physics) :
  ticsToCharge(ticsToCharge), physics(physics){
  tics = 0;
  used = false;
  this->isEnemy = isEnemy;
  this->proprietor = proprietor;
}

Gun::~Gun(){}

Bullet* Gun::shoot(){
  if ( tics != 0 || used )
    return NULL; // si ya se disparo y aun no se recargo
  return fire();
}

void Gun::tic(){
  tics++;
  if ( tics == ticsToCharge ) {
    tics = 0;
    used = false;
  }
}

Boombgun::Boombgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bullet* Boombgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Boomb(this->physics, pos.x, pos.y, signo);
  return bullet;
}

Magnetgun::Magnetgun(Body* proprietor, bool isEnemy, Physics& physics) :
  Gun(proprietor, isEnemy, 10, physics){}

Bullet* Magnetgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Magnet(this->physics, pos.x, pos.y, signo);
  return bullet;
}

Sparkgun::Sparkgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bullet* Sparkgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Spark(this->physics, pos.x, pos.y, signo);
  return bullet;
}

Ringgun::Ringgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bullet* Ringgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Ring(this->physics, pos.x, pos.y, signo);
  return bullet;
}

Firegun::Firegun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bullet* Firegun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Fire(this->physics, pos.x, pos.y, signo);
  return bullet;
}
