#include "player.h"

#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Player::Player(){}
Player::~Player(){}

void Player::jump(){
	vy_ = VY_MAX_LIM;
}

void Player::setPosition(int x, int y){
	x_ = x;
	y_ = y;
}

void Player::nextPosition(){
	y_ = y_ - vy_*T;
	if ( vy_ > 0  && vy_ > VY_MIN_LIM) vy_--;
}
