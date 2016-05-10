#include "player.h"

#define T 10
#define VY_MIN_LIM -10
#define VY_MAX_LIM 10

Player::Player() : vx_(0), vy_(0), idle(true) {}
Player::~Player(){}

void Player::jump(){
  if (idle) {
  	vy_ = VY_MAX_LIM;
    idle = false;
  }
}

void Player::setPosition(int x, int y){
	x_ = x;
	y_ = y;
}

void Player::nextPosition(){
	y_ = y_ - vy_ * T;
	if (vy_ > VY_MIN_LIM && !idle) vy_ -= 2;
  else if (vy_ == VY_MIN_LIM) {
    vy_ = 0;
    idle = true;
  }
}
