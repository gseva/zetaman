

#include "zm/server/physics/bullets.h"


Bullet::Bullet(Physics& physics, float32 x, float32 y, int signo,
  bool isEnemy) : Body(physics, x, y), vel(6*signo,0), isEnemy(isEnemy) {
  b2PolygonShape shape;
  shape.SetAsBox(0.01f, 0.01f);
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
  body->ApplyForce(b2Vec2(0,-DEFAULT_GRAVITY_Y), body->GetWorldCenter(), false);
}

Bullet::~Bullet(){}

void Bullet::move(){
  Lock locker(mutex);
  body->ApplyForce(b2Vec2(0,-DEFAULT_GRAVITY_Y), body->GetWorldCenter(), false);
}

bool Bullet::collide(Enemy* enemy){
  return !isEnemy;
}

bool Bullet::collide(PlayerBody* player){
  return isEnemy;
}

bool Bullet::collide(Bullet* bullet){
  return false;
}

/*PlayerBullet::PlayerBullet(Physics& physics, float32 x, float32 y, int signo) :
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
*/
