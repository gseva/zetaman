#include "powerup.h"
#include <Box2D/Box2D.h>

#include "zm/server/player.h"

#define SMALL_ENERGY_AMOUNT 10
#define LARGE_ENERGY_AMOUNT 20
#define SMALL_PLASMA_AMOUNT 15
#define LARGE_PLASMA_AMOUNT 30

PowerUp::PowerUp(Physics& physics, b2Vec2 pos, float size) :
Body(physics, pos.x, pos.y, BodyType::PowerUp){
  body->SetUserData(this);
  b2PolygonShape shape;
  shape.SetAsBox(size, size);
  fixtureDef.shape = &shape;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 100.0f;
  fixtureDef.filter.categoryBits = POWERUP_TYPE;
  fixtureDef.filter.maskBits = NONE_CONTACT | PLAYER_TYPE;
  fixture = body->CreateFixture(&fixtureDef);
}

PowerUp::~PowerUp(){}

zm::proto::PowerUp PowerUp::toBean(int xo, int yo){
  zm::proto::PowerUp protoPowerUp;
  protoPowerUp.id = getId();
  protoPowerUp.pos.x = getPosition().x - xo;
  protoPowerUp.pos.y = getPosition().y;
  return protoPowerUp;
}

SmallEnergy::SmallEnergy(Physics& physics, b2Vec2 pos)
    : PowerUp(physics, pos, 0.05f)
{}

SmallEnergy::~SmallEnergy(){}

bool SmallEnergy::applyTo(Player* p) {
  return p->addHealth(SMALL_ENERGY_AMOUNT);
}

zm::proto::PowerUp SmallEnergy::toBean(int xo, int yo){
  zm::proto::PowerUp protoSmallEnergy = PowerUp::toBean(xo, yo);
  protoSmallEnergy.type = zm::proto::SmallEnergy;
  return protoSmallEnergy;
}


LargeEnergy::LargeEnergy(Physics& physics, b2Vec2 pos)
    : PowerUp(physics, pos, 0.1f)
{}

LargeEnergy::~LargeEnergy(){}

bool LargeEnergy::applyTo(Player* p) {
  return p->addHealth(LARGE_ENERGY_AMOUNT);
}

zm::proto::PowerUp LargeEnergy::toBean(int xo, int yo){
  zm::proto::PowerUp protoLargeEnergy = PowerUp::toBean(xo, yo);
  protoLargeEnergy.type = zm::proto::LargeEnergy;
  return protoLargeEnergy;
}


Life::Life(Physics& physics, b2Vec2 pos)
    : PowerUp(physics, pos, 0.1f)
{}

Life::~Life(){}

bool Life::applyTo(Player* p) {
  return p->addLife();
}

zm::proto::PowerUp Life::toBean(int xo, int yo){
  zm::proto::PowerUp protoLife = PowerUp::toBean(xo, yo);
  protoLife.type = zm::proto::Life;
  return protoLife;
}


SmallPlasma::SmallPlasma(Physics& physics, b2Vec2 pos)
    : PowerUp(physics, pos, 0.05f)
{}

SmallPlasma::~SmallPlasma(){}

bool SmallPlasma::applyTo(Player* p) {
  return p->addAmmo(SMALL_PLASMA_AMOUNT);
}

zm::proto::PowerUp SmallPlasma::toBean(int xo, int yo){
  zm::proto::PowerUp protoSmallPlasma = PowerUp::toBean(xo, yo);
  protoSmallPlasma.type = zm::proto::SmallPlasma;
  return protoSmallPlasma;
}


LargePlasma::LargePlasma(Physics& physics, b2Vec2 pos)
    : PowerUp(physics, pos, 0.1f)
{}

LargePlasma::~LargePlasma(){}

bool LargePlasma::applyTo(Player* p) {
  return p->addAmmo(LARGE_PLASMA_AMOUNT);
}

zm::proto::PowerUp LargePlasma::toBean(int xo, int yo){
  zm::proto::PowerUp protoLargePlasma = PowerUp::toBean(xo, yo);
  protoLargePlasma.type = zm::proto::LargePlasma;
  return protoLargePlasma;
}
