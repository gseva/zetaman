
#include <vector>

#include "zm/server/physics/players.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"


PlayerBody::PlayerBody(Physics& physics, float32 x, float32 y)
  :  Body(physics, x, y, BodyType::Player) {
  body->SetUserData(this);
  b2PolygonShape shape;
  shape.SetAsBox(0.4f, 0.4f);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.0f;
  fixtureDef.filter.categoryBits = PLAYER_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
  health = 50;
}

PlayerBody::~PlayerBody(){
}

void PlayerBody::jump(){
  b2Vec2 vel = body->GetLinearVelocity();
  if ( vel.y == 0 ) {
    vel.y += 6;
    body->SetLinearVelocity(vel);
  }
}

void PlayerBody::right(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 6;
  body->SetLinearVelocity(vel);
}
void PlayerBody::left(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = -6;
  body->SetLinearVelocity(vel);
}

void PlayerBody::stopHorizontalMove(){
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 0;
  body->SetLinearVelocity(vel);
}

bool PlayerBody::up(){
  if ( canGoUp() ) {
    b2Vec2 v = body->GetLinearVelocity();
    v.y = 3;
    body->SetLinearVelocity(v);
    return true;
  }
  return false;
}

bool PlayerBody::canGoUp(){
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
  //Bullet* bullet = new PlayerBullet(this->physics, pos.x, pos.y, signo);
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y, signo, false);
 // Bullet* bullet = guns[selectedGun]->shoot();
  return bullet;
}

bool PlayerBody::collide(Bullet* bullet){
  return bullet->collide(this);
}

b2Body* PlayerBody::getBody(){
  return body;
}

void PlayerBody::toImpact(Bomb* bullet){
  damage(4);
}

void PlayerBody::toImpact(Spark* bullet){
  damage(3);
}

void PlayerBody::toImpact(Ring* bullet){
  damage(3);
}

void PlayerBody::toImpact(Magnet* bullet){
  damage(2);
}

void PlayerBody::toImpact(Fire* bullet){
  damage(4);
}

void PlayerBody::toImpact(Bullet* bullet){
  damage(1);
}

void PlayerBody::damage(int hurt){
  health -= hurt;
  if ( health <= 0 )
    markAsDestroyed();
}
