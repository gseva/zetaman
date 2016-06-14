
#ifndef __ZM_SERVER_PLAYERS_H__
#define __ZM_SERVER_PLAYERS_H__


#include "zm/server/physics/world.h"

class Bullet;


class PlayerBody : public Body {
public:
  // explicit PlayerBody(Physics& physics);
  explicit PlayerBody(Physics& physics, float32 x, float32 y);
  virtual ~PlayerBody();
  void jump();
  void right();
  void left();
  void stopHorizontalMove();
  void up();
  Bullet* shoot();
  virtual bool collide(Bullet* bullet);
  b2Body* getBody();
private:
  bool canGoUp();
};

#endif
