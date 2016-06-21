#include "powerup.h"
#include <Box2D/Box2D.h>

PowerUp::PowerUp(Physics& physics, b2Vec2 pos) :
Body(physics, pos.x, pos.y, BodyType::PowerUp){
  body->SetUserData(this);
  b2PolygonShape shape;
  shape.SetAsBox(0.05f, 0.05f);
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
  protoPowerUp.pos.x = getPosition().x - xo;
  protoPowerUp.pos.y = getPosition().y;
  return protoPowerUp;
}

SmallEnergy::SmallEnergy(Physics& physics, b2Vec2 pos) : PowerUp(physics, pos)
{}

SmallEnergy::~SmallEnergy(){}

zm::proto::PowerUp SmallEnergy::toBean(int xo, int yo){
  zm::proto::PowerUp protoSmallEnergy = PowerUp::toBean(xo, yo);
  protoSmallEnergy.type = zm::proto::SmallEnergy;
  return protoSmallEnergy;
}
