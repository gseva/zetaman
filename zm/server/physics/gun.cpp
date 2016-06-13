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

Bombgun::Bombgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Bombgun::~Bombgun(){}

Bullet* Bombgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bomb(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

Magnetgun::Magnetgun(Body* proprietor, bool isEnemy, Physics& physics) :
  Gun(proprietor, isEnemy, 10, physics){}

Magnetgun::~Magnetgun(){}

Bullet* Magnetgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Magnet(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

Sparkgun::Sparkgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Sparkgun::~Sparkgun(){}

Bullet* Sparkgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Spark(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

Ringgun::Ringgun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Ringgun::~Ringgun(){}

Bullet* Ringgun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Ring(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

Firegun::Firegun(Body* proprietor, bool isEnemy, Physics& physics) : 
  Gun(proprietor, isEnemy, 10, physics){}

Firegun::~Firegun(){}

Bullet* Firegun::fire(){
  used = true;
  b2Vec2 pos = proprietor->getPosition();
  b2Vec2 vel = proprietor->body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Fire(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}
