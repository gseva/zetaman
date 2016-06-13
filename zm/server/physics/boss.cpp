#include "zm/server/physics/boss.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"

#define BOOMBMAN_VEL        2
#define BOOMBMAN_JUMP_F     5 * 60
#define BOOMBMAN_SHOOT_F    5 * 60
#define BOOMBMAN_JUMP       6

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
    vel.x = -vel.x;
  body->SetLinearVelocity(vel);
  return bullet;
}

Boombman::Boombman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, BOOMBMAN_VEL, BOOMBMAN_JUMP,
  BOOMBMAN_SHOOT_F, BOOMBMAN_JUMP_F){
  gun = new Boombgun(this, true, physics);
}

Magnetman::Magnetman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, MAGNETMAN_VEL, MAGNETMAN_JUMP,
  MAGNETMAN_SHOOT_F, MAGNETMAN_JUMP_F){
  gun = new Magnetgun(this, true, physics);
}

Sparkman::Sparkman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, SPARKMAN_VEL, SPARKMAN_JUMP,
  SPARKMAN_SHOOT_F, SPARKMAN_JUMP_F){
  gun = new Sparkgun(this, true, physics);
}

Ringman::Ringman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, RINGMAN_VEL, RINGMAN_JUMP,
  RINGMAN_SHOOT_F, RINGMAN_JUMP_F){
  gun = new Ringgun(this, true, physics);
}

Fireman::Fireman(Physics& physics, float32 x, float32 y) : 
Boss(physics, x, y, FIREMAN_VEL, FIREMAN_JUMP,
  FIREMAN_SHOOT_F, FIREMAN_JUMP_F){
  gun = new Firegun(this, true, physics);
}
