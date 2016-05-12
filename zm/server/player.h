#ifndef __PLAYER_H__
#define __PLAYER_H__


class Player{
public:
	Player();
	~Player();
	void setPosition(int x, int y);
	void nextPosition();
	void jump();
	int x_,y_;
private:
	int vy_;
  bool idle;
};

#endif
