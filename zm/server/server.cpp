#include <Box2D/Box2D.h>
#include <iostream>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "zm/mapita.json"

Server::Server(ServerProxy& sp) : timer(physics, sp),
  player(physics){
  JsonSerializer js;
  JsonMap jm = js.importMap(DEFAULT_PATH);
  physics.setMap(jm);
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
