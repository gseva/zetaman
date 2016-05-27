#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <Box2D/Box2D.h>
#include "zm/server/physics/physics.h"

class Player{
public:
	Player();
	~Player();
	void setPosition(int x, int y);
  void createBody(Physics* physics);
  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  void up();
  b2Vec2 getPosition();
private:
  PlayerBody *body;
};

#endif
