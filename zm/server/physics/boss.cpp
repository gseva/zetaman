#include "zm/server/physics/boss.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"
#include <vector>

#define BOMBMAN_VEL        2
#define BOMBMAN_JUMP_F     5 * 60
#define BOMBMAN_SHOOT_F    5 * 60
#define BOMBMAN_JUMP       6
#define BOMBMAN_PREF_DIST 3

#define MAGNETMAN_VEL       4
#define MAGNETMAN_JUMP_F    15 * 60
#define MAGNETMAN_SHOOT_F   5 * 60
#define MAGNETMAN_JUMP      3
#define MAGNETMAN_PREF_DIST 3

#define SPARKMAN_VEL       2 
#define SPARKMAN_JUMP_F    2 * 60
#define SPARKMAN_SHOOT_F   5 * 60
#define SPARKMAN_JUMP      3 
#define SPARKMAN_PREF_DIST 3

#define RINGMAN_VEL       5 
#define RINGMAN_JUMP_F    10 * 60
#define RINGMAN_SHOOT_F   8 * 60
#define RINGMAN_JUMP      6 
#define RINGMAN_PREF_DIST 3

#define FIREMAN_VEL       10
#define FIREMAN_JUMP_F    10 * 60
#define FIREMAN_SHOOT_F   5 * 60
#define FIREMAN_JUMP      6 
#define FIREMAN_PREF_DIST 3

Boss::Boss(Physics& physics, float32 x, float32 y,
    int velocity, int jump, int shootFrecuency, int jumpFrecuency,
    JsonMap jm, std::vector<Player*>& players) : 
    Enemy(physics, x, y), velocity(velocity), jump(jump),
    shootFrecuency(shootFrecuency), jumpFrecuency(jumpFrecuency),
    jm(jm), players(players){
      tics = 0;
      moving = false;
      determinePositionsToGo();
}
 
void Boss::determinePositionsToGo(){
  float left;
  /*12 es el alto fijo del mapa*/
  unsigned int mapLen = jm.imageNumbers.size() / 12;
  /*16 es al ancho fijo del mapa*/
  unsigned int screenCount = mapLen / 16;

  /*Faltaria multiplicarlo por metroPorTile*/
  left = (screenCount - 1) * 16;

  for (int i=1; i<14; i++)
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
  int idealDifX = 10;  
  int idealPositionNumber = 0;
  int bestDifX = 0;
  bestDifX = abs(positionsCanGo[0] - getPlayersAveragePosition().x);
  for (unsigned int i=1; i<positionsCanGo.size(); i++)
  {
    int difX = abs(positionsCanGo[i] - getPlayersAveragePosition().x);
    //Si la posicion se acerca mas a la ideal quiere esa
    if (abs(idealDifX-difX) < abs(idealDifX-bestDifX))
    {
      idealPositionNumber = i;
      bestDifX = difX;
    }
  }
  positionToGo = positionsCanGo[idealPositionNumber];
}

bool Boss::gotCloseEnough(){
  bool gotClose = false;
  float difX = getPosition().x - positionToGo;
  float delta = 1;

  if (abs(difX) < delta)
    gotClose = true;

  return gotClose; 
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
    bullet = gun->shoot();
  }
  //if ( tics%(60*3) == 0 )
    //{
      //if (!moving)
      //{
        choosePosition();
      //  moving = true;
      //}
      //if (moving)
      //{
        vel.x = moveTowardsPosition();
      //}
    //}

  //if (gotCloseEnough())
  //{
  //  moving = false;
  //}  

  std::cout << "Posicion actual en x " << getPosition().x << std::endl;
  std::cout << "Posicion que quiero en x " << positionToGo << std::endl;
  std::cout << "Posicion player x " << players[0]->getPosition().x << std::endl;

  body->SetLinearVelocity(vel);
  return bullet;
}

Bombman::Bombman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) : 
Boss(physics, x, y, BOMBMAN_VEL, BOMBMAN_JUMP,
  BOMBMAN_SHOOT_F, BOMBMAN_JUMP_F, jm, players){
  gun = new Bombgun(this, true, physics);
}

Bombman::~Bombman(){}

zm::proto::Enemy Bombman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bombman;
  return protoEnemy;
}


Magnetman::Magnetman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) : 
Boss(physics, x, y, MAGNETMAN_VEL, MAGNETMAN_JUMP,
  MAGNETMAN_SHOOT_F, MAGNETMAN_JUMP_F, jm, players){
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
Boss(physics, x, y, SPARKMAN_VEL, SPARKMAN_JUMP,
  SPARKMAN_SHOOT_F, SPARKMAN_JUMP_F, jm, players){
  gun = new Sparkgun(this, true, physics);
}

Sparkman::~Sparkman(){}

zm::proto::Enemy Sparkman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Sparkman;
  return protoEnemy;
}


Ringman::Ringman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) : 
Boss(physics, x, y, RINGMAN_VEL, RINGMAN_JUMP,
  RINGMAN_SHOOT_F, RINGMAN_JUMP_F, jm, players){
  gun = new Ringgun(this, true, physics);
}

Ringman::~Ringman(){}

zm::proto::Enemy Ringman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Ringman;
  return protoEnemy;
}


Fireman::Fireman(Physics& physics, float32 x, float32 y,
  JsonMap jm, std::vector<Player*>& players) : 
Boss(physics, x, y, FIREMAN_VEL, FIREMAN_JUMP,
  FIREMAN_SHOOT_F, FIREMAN_JUMP_F, jm, players){
  gun = new Firegun(this, true, physics);
}

Fireman::~Fireman(){}

zm::proto::Enemy Fireman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Fireman;
  return protoEnemy;
}

