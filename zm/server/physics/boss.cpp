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
    int velocity, int jump, int shootFrecuency, int jumpFrecuency,
    JsonMap jm) : 
    Enemy(physics, x, y), velocity(velocity), jump(jump),
    shootFrecuency(shootFrecuency), jumpFrecuency(jumpFrecuency),
    jm(jm){
      tics = 0;
      b2Vec2 vel;
      vel.x = velocity;
      vel.y = 0;
      body->SetLinearVelocity(vel);
      moving = false;
      canJump = true;
      determinePositionsToGo();
}

float Boss::getMaxJumpHeight(){
  return 0.25;
}
 
void Boss::determinePositionsToGo(){
  zm::proto::Position bottomLeft;
  zm::proto::Position topRight;
  /*12 es el alto fijo del mapa*/
  unsigned int mapLen = jm.imageNumbers.size() / 12;
  std::cout << "TAMANIO ASD" << mapLen << std::endl;
  /*16 es al ancho fijo del mapa*/
  unsigned int screenCount = mapLen / 16;

  /*Faltaria multiplicarlo por metroPorTile*/
  bottomLeft.x = (screenCount - 1) * 8 + .5f;
  bottomLeft.y = .5f;
  topRight.x = (screenCount * 16) - 3;
  topRight.y = getMaxJumpHeight() + 0.5f;

  /*Por ahora solo puede moverse a estas dos posiciones*/
  positionsCanGo.push_back(bottomLeft);
  positionsCanGo.push_back(topRight);
}

void Boss::choosePosition(){
  if (positionToGo.x == positionsCanGo[0].x)
  {
    positionToGo.x = positionsCanGo[1].x;
    positionToGo.y = positionsCanGo[1].y;  
  } else {
    positionToGo.x = positionsCanGo[0].x;
    positionToGo.y = positionsCanGo[0].y;
  }
}

bool Boss::gotCloseEnough(){
  bool gotClose = false;
  float difX = getPosition().x - positionToGo.x;
  float difY = getPosition().y - positionToGo.x;

  std::cout << "Posicion actual en x" << getPosition().x << std::endl;
  std::cout << "Posicion actual en y" << getPosition().y << std::endl;
  std::cout << "Posicion que quiero en x" << positionToGo.x << std::endl;
  std::cout << "Posicion que quiero en y" << positionToGo.y << std::endl;

  if ((difX < 2 || difX > -2) && (difY < 2 || difY > -2))
    gotClose = true;

    return gotClose; 
}

b2Vec2 Boss::moveTowardsPosition(){
  b2Vec2 vel;

  bool needsToJump = false;
  if (getPosition().y > positionToGo.y)
    needsToJump = true;

  bool crashesOnJump = false;

  if (needsToJump && canJump && !crashesOnJump)
  {
    vel.y = jump;
  }

  if (getPosition().x < positionToGo.x)
  {
    vel.x = velocity;
  } else {
    vel.x = -velocity;
  }

  if (gotCloseEnough())
  {
    moving = false;  
  }

  return vel;
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
    canJump=true;
  if ( tics%shootFrecuency == 0 )
    bullet = gun->shoot();
  if ( tics%(60*3) == 0 )
    {
      if (!moving)
      {
        choosePosition();
        moving = true;
      }
      if (moving)
      {
        vel = moveTowardsPosition();
      }
    }
  body->SetLinearVelocity(vel);
  return bullet;
}

Bombman::Bombman(Physics& physics, float32 x, float32 y,
  JsonMap jm) : 
Boss(physics, x, y, BOMBMAN_VEL, BOMBMAN_JUMP,
  BOMBMAN_SHOOT_F, BOMBMAN_JUMP_F, jm){
  gun = new Bombgun(this, true, physics);
}

Bombman::~Bombman(){}

zm::proto::Enemy Bombman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Bombman;
  return protoEnemy;
}


Magnetman::Magnetman(Physics& physics, float32 x, float32 y,
  JsonMap jm) : 
Boss(physics, x, y, MAGNETMAN_VEL, MAGNETMAN_JUMP,
  MAGNETMAN_SHOOT_F, MAGNETMAN_JUMP_F, jm){
  gun = new Magnetgun(this, true, physics);
}

Magnetman::~Magnetman(){}

zm::proto::Enemy Magnetman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Magnetman;
  return protoEnemy;
}


Sparkman::Sparkman(Physics& physics, float32 x, float32 y,
  JsonMap jm) : 
Boss(physics, x, y, SPARKMAN_VEL, SPARKMAN_JUMP,
  SPARKMAN_SHOOT_F, SPARKMAN_JUMP_F, jm){
  gun = new Sparkgun(this, true, physics);
}

Sparkman::~Sparkman(){}

zm::proto::Enemy Sparkman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Sparkman;
  return protoEnemy;
}


Ringman::Ringman(Physics& physics, float32 x, float32 y,
  JsonMap jm) : 
Boss(physics, x, y, RINGMAN_VEL, RINGMAN_JUMP,
  RINGMAN_SHOOT_F, RINGMAN_JUMP_F, jm){
  gun = new Ringgun(this, true, physics);
}

Ringman::~Ringman(){}

zm::proto::Enemy Ringman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Ringman;
  return protoEnemy;
}


Fireman::Fireman(Physics& physics, float32 x, float32 y,
  JsonMap jm) : 
Boss(physics, x, y, FIREMAN_VEL, FIREMAN_JUMP,
  FIREMAN_SHOOT_F, FIREMAN_JUMP_F, jm){
  gun = new Firegun(this, true, physics);
}

Fireman::~Fireman(){}

zm::proto::Enemy Fireman::toBean(int xo, int yo){
  zm::proto::Enemy protoEnemy = Enemy::toBean(xo, yo);
  protoEnemy.enemyType = zm::proto::EnemyType::Fireman;
  return protoEnemy;
}

