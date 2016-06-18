#include "zm/server/physics/boss.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"

#define BOMBMAN_VEL        2
#define BOMBMAN_JUMP_F     5 * 60
#define BOMBMAN_SHOOT_F    5 * 60
#define BOMBMAN_JUMP       6

#define MAGNETMAN_VEL       4
#define MAGNETMAN_JUMP_F    15 * 60
#define MAGNETMAN_SHOOT_F   5 * 60
#define MAGNETMAN_JUMP      3

#define SPARKMAN_VEL       2 
#define SPARKMAN_JUMP_F    2 * 60
#define SPARKMAN_SHOOT_F   5 * 60
#define SPARKMAN_JUMP      3 

#define RINGMAN_VEL       5 
#define RINGMAN_JUMP_F    10 * 60
#define RINGMAN_SHOOT_F   8 * 60
#define RINGMAN_JUMP      6 

#define FIREMAN_VEL       10
#define FIREMAN_JUMP_F    2 * 60
#define FIREMAN_SHOOT_F   5 * 60
#define FIREMAN_JUMP      6 

Boss::Boss(Physics& physics, float32 x, float32 y,
    int velocity, int jump, int shootFrecuency, int jumpFrecuency) : 
    Enemy(physics, x, y), velocity(velocity), jump(jump),
    shootFrecuency(shootFrecuency), jumpFrecuency(jumpFrecuency){
      tics = 0;
      b2Vec2 vel;
      vel.x = velocity;
      vel.y = 0;
      body->SetLinearVelocity(vel);
}

Boss::~Boss(){
  delete gun;
}

Bullet* Boss::move(){
  tics++;
  gun->tic();
  Bullet* bullet = NULL;
  b2Vec2 vel = body->GetLinearVelocity();
  if ( tics%jumpFrecuency == 0 )
    vel.y = jump;
  if ( tics%shootFrecuency == 0 )
    bullet = gun->shoot();
  if ( tics%(60*3) == 0 )
    velocity = -velocity;
  vel.x = velocity;
  body->SetLinearVelocity(vel);
  return bullet;
}

Bombman::Bombman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, BOMBMAN_VEL, BOMBMAN_JUMP,
  BOMBMAN_SHOOT_F, BOMBMAN_JUMP_F){
  gun = new Bombgun(this, true, physics);
}

Bombman::~Bombman(){}

zm::proto::Enemy Bombman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bombman;
  return protoEnemy;
}


Magnetman::Magnetman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, MAGNETMAN_VEL, MAGNETMAN_JUMP,
  MAGNETMAN_SHOOT_F, MAGNETMAN_JUMP_F){
  gun = new Magnetgun(this, true, physics);
}

Magnetman::~Magnetman(){}

zm::proto::Enemy Magnetman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Magnetman;
  return protoEnemy;
}


Sparkman::Sparkman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, SPARKMAN_VEL, SPARKMAN_JUMP,
  SPARKMAN_SHOOT_F, SPARKMAN_JUMP_F){
  gun = new Sparkgun(this, true, physics);
}

Sparkman::~Sparkman(){}

zm::proto::Enemy Sparkman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Sparkman;
  return protoEnemy;
}


Ringman::Ringman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, RINGMAN_VEL, RINGMAN_JUMP,
  RINGMAN_SHOOT_F, RINGMAN_JUMP_F){
  gun = new Ringgun(this, true, physics);
}

Ringman::~Ringman(){}

zm::proto::Enemy Ringman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Ringman;
  return protoEnemy;
}


Fireman::Fireman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, FIREMAN_VEL, FIREMAN_JUMP,
  FIREMAN_SHOOT_F, FIREMAN_JUMP_F){
  gun = new Firegun(this, true, physics);
}

Fireman::~Fireman(){}

zm::proto::Enemy Fireman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Fireman;
  return protoEnemy;
}

