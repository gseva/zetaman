#include "zm/server/server.h"
#include <Box2D/Box2D.h>
#include "zm/server/physical/physical.h"
#include <iostream>
Server::Server(ServerProxy& sp) : timer(physical, sp),
  player(physical){
  //creo el mundo del nivel, y seteo la gravedad
  //gravity = new b2Vec2(0.0f, -10.0f);
  //world = new b2World(*gravity);
  //creo el piso
/*  groundBodyDef.position.Set(0.0f, -10.0f);
  groundBody = world->CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);*/
  
  //this->player.createBody(world.world);

  timer.start();
}

Server::~Server(){
  //delete gravity;
  //delete world;
  timer.join();
}

void Server::jump(){
	player.jump();
}


void Server::right(){
  player.right();  
}
void Server::left(){
  player.right();  
}

void Server::stopHorizontalMove(){
  player.stopHorizontalMove();  
}


GameState Server::getState(){
  b2Vec2 position = player.getPosition();

  GameState gs;
  gs.x = position.x*100;
  gs.y = position.y*(-100)+400;
  return gs;
}
