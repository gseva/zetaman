
#ifndef __ZM_SERVER_PLAYERS_H__
#define __ZM_SERVER_PLAYERS_H__

#include "zm/server/physics/world.h"
#include "zm/server/physics/bullets.h"

class Bullet;
class Player;

class PlayerBody : public Body {
public:
  explicit PlayerBody(Physics& physics, float32 x, float32 y);
  virtual ~PlayerBody();
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
  bool up();
  Bullet* shoot();
  b2Body* getBody();

  virtual void toImpact(Bomb* bullet);
  virtual void toImpact(Spark* bullet);
  virtual void toImpact(Ring* bullet);
  virtual void toImpact(Magnet* bullet);
  virtual void toImpact(Fire* bullet);
  virtual void toImpact(Bullet* bullet);

  int health;
private:
  bool canGoUp();
  void damage(int hurt);
};

#endif
