#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <Box2D/Box2D.h>


class Player{
public:
	Player();
	~Player();
	void setPosition(int x, int y);
  void createBody(b2World* world);
  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  b2Vec2 getPosition();
private:
  b2Body* body;
  int vy_;
  bool idle;
  b2BodyDef playerBodyDef;
  b2Body* palyerBody;
};

#endif
