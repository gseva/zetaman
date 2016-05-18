#include "zm/server/server.h"



Server::Server(ServerProxy& sp) //: serverProxy(sp)
{
	this->player.setPosition(1024/2, 768-40-64);
}

Server::~Server(){}

void Server::jump(){
	player.jump();
}

GameState Server::getState(){
	player.nextPosition();
  GameState gs;
  gs.x = player.x_;
  gs.y = player.y_;
  return gs;
}
