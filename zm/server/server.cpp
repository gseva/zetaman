#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include "zm/server/server.h"
#include "zm/server/physics/physics.h"
#include "zm/json/jsonserializer.h"

#define DEFAULT_PATH "assets/maps/mapita.json"

Server::Server(ServerProxy& sp) : timer(physics, sp),
  player(physics) {
  JsonSerializer js;
  jm = js.importMap(DEFAULT_PATH);
  physics.setMap(jm);
}

void Server::startTimer() {
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

std::vector<std::string> Server::getImageNames(){
  return jm.imageNames;
}

std::vector<int> Server::getImages() {
  return jm.imageNumbers;
}
