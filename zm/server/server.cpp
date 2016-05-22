#include "zm/server/server.h"
#include <Box2D/Box2D.h>
#include "zm/server/physical/physical.h"
#include <iostream>
Server::Server(ServerProxy& sp) : timer(physical, sp),
  player(physical){
  timer.start();
}

Server::~Server(){
  timer.join();
}

void Server::jump(){
	player.jump();
}


void Server::right(){
  player.right();
}
void Server::left(){
  player.left();
}

void Server::stopHorizontalMove(){
  player.stopHorizontalMove();
}

zm::Game Server::getState(){
  b2Vec2 position = player.getPosition();

  zm::Game gs;
  gs.x = position.x*100;
  gs.y = position.y*(-100)+400;
  return gs;
}
