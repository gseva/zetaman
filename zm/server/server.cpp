#include "zm/server/server.h"
#include <Box2D/Box2D.h>

Server::Server(ServerProxy& sp){
  //creo el mundo del nivel, y seteo la gravedad
  gravity = new b2Vec2(0.0f, -10.0f);
  world = new b2World(*gravity);

  //creo el piso
  groundBodyDef.position.Set(0.0f, -10.0f);
  groundBody = world->CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);
  groundBody->CreateFixture(&groundBox, 0.0f);
  
  this->player.createBody(world);
}

Server::~Server(){
  delete gravity;
  delete world;
}

void Server::jump(){
	player.jump();
}

/*void Server::right(){
  player.right();
}*/

GameState Server::getState(){
  float32 timeStep = 1.0f / 60.0f; //60Hz
  int32 velocityIterations = 8; //valores sugeridos
  int32 positionIterations = 3; 
  world->Step(timeStep, velocityIterations, positionIterations);

  b2Vec2 position = player.body->GetPosition();

  GameState gs;
  gs.x = position.x*100;
  gs.y = position.y*(-100)+400;
  return gs;
}
