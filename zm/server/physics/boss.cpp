#include <vector>

#include "zm/config.h"
#include "zm/server/physics/boss.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"

#define BOMBMAN_VEL        2
#define BOMBMAN_JUMP_F     5 * 60
#define BOMBMAN_SHOOT_F    5 * 60
#define BOMBMAN_JUMP       6
#define BOMBMAN_PREF_DIST  5
#define BOMBMAN_LARGO      0.4f
#define BOMBMAN_ALTO       0.7f

#define MAGNETMAN_VEL       4
#define MAGNETMAN_JUMP_F    15 * 60
#define MAGNETMAN_SHOOT_F   5 * 60
#define MAGNETMAN_JUMP      3
#define MAGNETMAN_PREF_DIST 5
#define MAGNETMAN_LARGO     0.4f
#define MAGNETMAN_ALTO      0.6f

#define SPARKMAN_VEL        2
#define SPARKMAN_JUMP_F     2 * 60
#define SPARKMAN_SHOOT_F    5 * 60
#define SPARKMAN_JUMP       3
#define SPARKMAN_PREF_DIST  5
#define SPARKMAN_LARGO      1.0f
#define SPARKMAN_ALTO       0.6f

#define RINGMAN_VEL       5
#define RINGMAN_JUMP_F    10 * 60
#define RINGMAN_SHOOT_F   8 * 60
#define RINGMAN_JUMP      6
#define RINGMAN_PREF_DIST 5
#define RINGMAN_LARGO     0.4f
#define RINGMAN_ALTO      0.7f

#define FIREMAN_VEL       10
#define FIREMAN_JUMP_F    10 * 60
#define FIREMAN_SHOOT_F   5 * 60
#define FIREMAN_JUMP      6
#define FIREMAN_PREF_DIST 5
#define FIREMAN_LARGO     0.55
#define FIREMAN_ALTO      0.7

Boss::Boss(Physics& physics, float32 x, float32 y,
    int velocity, int jump, int shootFrecuency, int jumpFrecuency,
    int prefDistance, JsonMap jm, std::vector<Player*>& players,
    float32 largo, float32 alto) :
    Enemy(physics, x, y, largo, alto), velocity(velocity), jump(jump),
    shootFrecuency(shootFrecuency), jumpFrecuency(jumpFrecuency),
    prefDistance(prefDistance), jm(jm), players(players) {
      tics = 0;
      determinePositionsToGo();
	  health = zm::config::bossLife;
}

void Boss::determinePositionsToGo(){
  float left;
  /*12 es el alto fijo del mapa*/
  unsigned int mapLen = jm.imageNumbers.size() / 12;
  /*16 es al ancho fijo del mapa*/
  unsigned int screenCount = mapLen / 16;

  /* Posicion mas a la izquierda de su recamara*/
  left = (screenCount - 1) * 16;

  for (int i=0; i<14; i++)
  {
    float position;
    position = left + i;
    positionsCanGo.push_back(position);
  }
}

zm::proto::Position Boss::getPlayersAveragePosition()
{
  zm::proto::Position average;

  for (unsigned int i=0; i<players.size(); i++)
  {
    average.x += players[i]->getPosition().x;
    average.y += players[i]->getPosition().y;
  }
  average.x = average.x / players.size();
  average.y = average.y / players.size();
  return average;
}

void Boss::choosePosition(){
  int idealPositionNumber = 0;
  int bestDifX = 0;
  bestDifX = std::abs(positionsCanGo[0] - getPlayersAveragePosition().x);
  for (unsigned int i=1; i<positionsCanGo.size(); i++)
  {
    int difX = std::abs(positionsCanGo[i] - getPlayersAveragePosition().x);
    //Si la posicion se acerca mas a la ideal quiere esa
    if (std::abs(prefDistance-difX) < std::abs(prefDistance-bestDifX))
    {
      idealPositionNumber = i;
      bestDifX = difX;
    }
  }
  positionToGo = positionsCanGo[idealPositionNumber];
}

float Boss::moveTowardsPosition(){
  float xVel;

  if (getPosition().x < positionToGo)
  {
    xVel = velocity;
  } else {
    xVel = -velocity;
  }

  return xVel;
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
  {
    int direction;
    if (getPlayersAveragePosition().x > getPosition().x)
    {
      direction = 1;
    } else {
      direction = -1;
    }
    bullet = gun->shoot(direction);
  }
  choosePosition();
  vel.x = moveTowardsPosition();

  body->SetLinearVelocity(vel);
  return bullet;
}

void Boss::toImpact(Bomb* bullet){
   damage(3);
}
void Boss::toImpact(Spark* bullet){
   damage(3);
}
void Boss::toImpact(Ring* bullet){
   damage(3);
}
void Boss::toImpact(Magnet* bullet){
   damage(3);
}
void Boss::toImpact(Fire* bullet){
   damage(3);
}
void Boss::toImpact(Bullet* bullet){
   damage(1);
}

void Boss::damage(int hurt){
  health -= hurt;
  if ( health <= 0 )
    markAsDestroyed();
}

zm::proto::Enemy Boss::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  if (body->GetLinearVelocity().x >= 0) {
    protoEnemy.o = zm::proto::right;
  } else {
    protoEnemy.o = zm::proto::left;
  }
  return protoEnemy;
}

int Boss::getGunNumber() {
  return gun->getNumber();
}

Bombman::Bombman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) :
Boss(physics, x, y+BOMBMAN_ALTO, BOMBMAN_VEL, BOMBMAN_JUMP,
  BOMBMAN_SHOOT_F, BOMBMAN_JUMP_F, BOMBMAN_PREF_DIST,
   jm, players, BOMBMAN_LARGO, BOMBMAN_ALTO){
  gun = new Bombgun(this, true, physics);
}

Bombman::~Bombman(){}


zm::proto::Enemy Bombman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Boss::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bombman;
  return protoEnemy;
}


Magnetman::Magnetman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) :
Boss(physics, x, y+MAGNETMAN_ALTO, MAGNETMAN_VEL, MAGNETMAN_JUMP,
  MAGNETMAN_SHOOT_F, MAGNETMAN_JUMP_F, MAGNETMAN_PREF_DIST,
   jm, players,MAGNETMAN_LARGO, MAGNETMAN_ALTO){
  gun = new Magnetgun(this, true, physics);
}

Magnetman::~Magnetman(){}


zm::proto::Enemy Magnetman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Magnetman;
  return protoEnemy;
}


Sparkman::Sparkman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) :
Boss(physics, x, y+SPARKMAN_ALTO, SPARKMAN_VEL, SPARKMAN_JUMP,
  SPARKMAN_SHOOT_F, SPARKMAN_JUMP_F, SPARKMAN_PREF_DIST,
   jm, players, SPARKMAN_LARGO, SPARKMAN_ALTO){
  gun = new Sparkgun(this, true, physics);
}

Sparkman::~Sparkman(){}


zm::proto::Enemy Sparkman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Boss::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Sparkman;
  return protoEnemy;
}


Ringman::Ringman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) :
Boss(physics, x, y+RINGMAN_ALTO, RINGMAN_VEL, RINGMAN_JUMP,
  RINGMAN_SHOOT_F, RINGMAN_JUMP_F, RINGMAN_PREF_DIST,
   jm, players, RINGMAN_LARGO, RINGMAN_ALTO){
  gun = new Ringgun(this, true, physics);
}

Ringman::~Ringman(){}


zm::proto::Enemy Ringman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Boss::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Ringman;
  return protoEnemy;
}


Fireman::Fireman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) :
Boss(physics, x, y+FIREMAN_ALTO, FIREMAN_VEL, FIREMAN_JUMP,
  FIREMAN_SHOOT_F, FIREMAN_JUMP_F, FIREMAN_PREF_DIST,
   jm, players, FIREMAN_LARGO, FIREMAN_ALTO){
  gun = new Firegun(this, true, physics);
}

Fireman::~Fireman(){}


zm::proto::Enemy Fireman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Boss::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Fireman;
  return protoEnemy;
}

