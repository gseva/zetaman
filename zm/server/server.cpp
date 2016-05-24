#include "zm/server/server.h"
#include <Box2D/Box2D.h>
#include "zm/server/physics/physics.h"
#include <iostream>
Server::Server(ServerProxy& sp) : timer(physics, sp),
  player(physics), enemy(physics){
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

  b2Vec2 enemyPosition = enemy.getPosition();
  

  zm::Game gs;
  gs.x = position.x*100;
  gs.y = position.y*(-100)+400;
  if (gs.enemies.size()==0)
  {
    zm::Enemy enemigo;
    enemigo.pos.x = enemyPosition.x*100;
    enemigo.pos.y = enemyPosition.y*(-100)+400;
    gs.enemies.push_back(enemigo);  
  } else {
    gs.enemies.at(0).pos.x = enemyPosition.x;
    gs.enemies.at(0).pos.y = enemyPosition.y;
  }
  

  std::cout << "Enemigo x:" << gs.enemies.at(0).pos.x << std::endl;
  std::cout << "Enemigo y:" << gs.enemies.at(0).pos.y << std::endl;
  std::cout << "Jugador x:" << gs.x << std::endl;
  std::cout << "Jugador y:" << gs.y << std::endl;



  return gs;
}
