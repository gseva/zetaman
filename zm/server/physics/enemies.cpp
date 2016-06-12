
#include "zm/server/physics/enemies.h"

#define PPM 64


Enemy::Enemy(Physics& physics, float32 x, float32 y) : Body(physics, x, y){
  b2PolygonShape shape;
  shape.SetAsBox(0.4f, 0.4f);
  fixtureDef.shape = &shape;
  fixtureDef.friction = 1.0f;
  fixtureDef.filter.categoryBits = ENEMY_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE & ~ENEMY_BULLET_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
}

Enemy::~Enemy(){}

bool Enemy::collide(Bullet* bullet){
  return bullet->collide(this);
}

zm::proto::Enemy Enemy::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy;
  protoEnemy.pos.x = this->getPosition().x* PPM - xo * PPM;
  protoEnemy.pos.y = this->getPosition().y * -PPM + 768;
  return protoEnemy;
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
  return bullet;
}

zm::proto::Enemy Met::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Met;
  switch ( this->state ) {
    case MetState::protect:
      protoEnemy.enemyState = zm::proto::EnemyState::guarded;
      break;
    case MetState::notProtect:
      protoEnemy.enemyState = zm::proto::EnemyState::unguarded;
      break;
  }
  return protoEnemy;
}

Bumby::Bumby(Physics& physics, float32 x, float32 y) : Enemy(physics, x, y),
  totalMoves(15) , period(60*3){
  shoots = 0;
  tics = 0;
  amountMoves = 0;
  b2Vec2 vel;
  sig = 1;
  vel.x = sig * 3;
  body->SetLinearVelocity(vel);
}

Bumby::~Bumby(){}

EnemyBullet* Bumby::move(){
  if ( amountMoves == totalMoves ) {
    sig *= -1;
    amountMoves = 0;
  } else {
    amountMoves++;
  }
  b2Vec2 vel = body->GetLinearVelocity();
  vel.x = 3 * sig;

  {
    Lock locker(mutex);
    body->SetLinearVelocity(vel);
    body->ApplyForce(b2Vec2(0,-DEFAULT_GRAVITY_Y),
      body->GetWorldCenter(), false);
  }

  tics++;
  if ( tics == period )
    tics = 0;
  if ( ((tics % 60) == 0 ) ) {
    return shoot();
  }
  return NULL;
}

EnemyBullet* Bumby::shoot(){
  b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  EnemyBullet* bullet = new EnemyBullet(this->physics, pos.x, pos.y,signo);
  return bullet;
}

zm::proto::Enemy Bumby::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bumby;
  return protoEnemy;
}
