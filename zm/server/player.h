#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <Box2D/Box2D.h>
#include "zm/server/physical/physical.h"

class Player{
public:
	explicit Player(Physical& physical);
	~Player();
	void setPosition(int x, int y);
  void createBody(b2World* world);
  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  b2Vec2 getPosition();
private:
  int vy_;
  bool idle;
  Physical& physical;
  PlayerBody body;
};

#endif
