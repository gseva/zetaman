#include "server.h"



Server::Server(ServerProxy& sp) : serverProxy(sp) {
	this->player.setPosition(1024/2, 768/2-40);
}

Server::~Server(){}

void Server::jump(){
	player.jump();
}

GameState Server::getState(){
	player.nextPosition();
	state_.x=player.x_;
	state_.y=player.y_;
}
