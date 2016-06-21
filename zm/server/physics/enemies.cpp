
#include "zm/server/physics/enemies.h"


Enemy::Enemy(Physics& physics, float32 x, float32 y,
            float32 largo, float32 alto)
    : Body(physics, x, y, BodyType::Enemy), orientation(zm::proto::left) {
  b2PolygonShape shape;
  shape.SetAsBox(largo, alto);
  fixtureDef.shape = &shape;
  fixtureDef.friction = 1.0f;
  fixtureDef.filter.categoryBits = ENEMY_TYPE;
  fixtureDef.filter.maskBits = ALL_CONTACT & ~STAIR_TYPE & ~ENEMY_BULLET_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
  body->SetUserData(this);
}

Enemy::~Enemy(){}

void Enemy::setOrientation(zm::proto::Orientation o) {
  orientation = o;
}

zm::proto::Orientation Enemy::getOrientation() {
  return orientation;
}

zm::proto::Enemy Enemy::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy;
  protoEnemy.pos.x = getPosition().x - xo;
  protoEnemy.pos.y = getPosition().y;
  protoEnemy.id = getId();
  protoEnemy.o = getOrientation();

  b2Vec2 vel = body->GetLinearVelocity();
  if (vel.y != 0) {
    protoEnemy.enemyState = zm::proto::EnemyState::jumping;
  } else if (vel.x != 0) {
    protoEnemy.enemyState = zm::proto::EnemyState::moving;
  } else {
    protoEnemy.enemyState = zm::proto::EnemyState::idle;
  }

  return protoEnemy;
}

void Enemy::toImpact(Bomb* bullet){
  markAsDestroyed();
}

void Enemy::toImpact(Spark* bullet){
  markAsDestroyed();
}

void Enemy::toImpact(Ring* bullet){
  markAsDestroyed();
}

void Enemy::toImpact(Magnet* bullet){
  markAsDestroyed();
}

void Enemy::toImpact(Fire* bullet){
  markAsDestroyed();
}

void Enemy::toImpact(Bullet* bullet){
  markAsDestroyed();
}

Met::Met(Physics& physics, float32 x, float32 y) :
  Enemy(physics, x, y, 0.35f, 0.2), period(60*3) {
    shoots = 0;
    tics = 0;
    protected_ = false;
}

Met::~Met(){}

Bullet* Met::move(){
  tics++;
  if ( tics == period )
    tics = 0;
  if ( tics == 0 ) {
    protected_ = !protected_;
  }
  if ( !protected_ && ((tics % 60) == 0 ) ) {
    return shoot();
  }
  return NULL;
}

Bullet* Met::shoot(){
  b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y,signo, true);
  return bullet;
}

zm::proto::Enemy Met::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Met;
  if (protected_) {
    protoEnemy.enemyState = zm::proto::EnemyState::guarded;
  }
  return protoEnemy;
}

void Met::toImpact(Ring* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Met::toImpact(Magnet* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Met::toImpact(Fire* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Met::toImpact(Bullet* bullet){
  if ( !protected_ )
    markAsDestroyed();
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

Bullet* Bumby::move(){
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

Bullet* Bumby::shoot(){
  b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

zm::proto::Enemy Bumby::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bumby;
  return protoEnemy;
}

Sniper::Sniper(Physics& physics, float32 x, float32 y) :
Enemy(physics,x,y){
  health = 2;
  protected_ = false;
  tics = 0;
}

Sniper::~Sniper(){}

Bullet* Sniper::move(){
  tics++;
  Bullet* bullet = NULL;
  if ( tics%(60*3) == 0 )
    protected_ = !protected_;
  if ( tics%(60*2) == 0 && !protected_ ) {
    bullet = shoot();
  }
  return bullet;
}

Bullet* Sniper::shoot(){
  b2Vec2 pos = getPosition();
  b2Vec2 vel = body->GetLinearVelocity();
  int signo = vel.x >=0 ? 1 : -1;
  Bullet* bullet = new Bullet(this->physics, pos.x, pos.y, signo, true);
  return bullet;
}

zm::proto::Enemy Sniper::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Sniper;
  if (protected_) {
    protoEnemy.enemyState = zm::proto::EnemyState::guarded;
  }
  return protoEnemy;
}

void Sniper::toImpact(Bomb* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Sniper::toImpact(Spark* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Sniper::toImpact(Ring* bullet){
  if ( !protected_ ) {
    markAsDestroyed();
  } else {
    health--;
    if ( health <= 0 )
      markAsDestroyed();
  }
}

void Sniper::toImpact(Magnet* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

void Sniper::toImpact(Fire* bullet){
  if ( !protected_ ) {
    markAsDestroyed();
  } else {
    health--;
    if ( health <= 0 )
      markAsDestroyed();
  }
}

void Sniper::toImpact(Bullet* bullet){
  if ( !protected_ )
    markAsDestroyed();
}

JumpingSniper::JumpingSniper(Physics& physics, float32 x, float32 y) :
  Sniper(physics,x,y){
  health = 2;
  protected_ = false;
  tics = 0;
}

JumpingSniper::~JumpingSniper(){}

Bullet* JumpingSniper::move(){
  tics++;
  b2Vec2 vel = body->GetLinearVelocity();
  if ( tics%(60) == 0 && vel.y == 0 ){
    vel.y = 7;
    body->SetLinearVelocity(vel);
  }
  Bullet* bullet = NULL;
  if ( tics%(60*2) == 0 ) {
    bullet = shoot();
  }
  return bullet;
}

zm::proto::Enemy JumpingSniper::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Sniper::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::JumpingSniper;
  if (protected_) {
    protoEnemy.enemyState = zm::proto::EnemyState::guarded;
  }
  return protoEnemy;
}
