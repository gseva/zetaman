#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include "zm/server/camera.h"

#include "zm/server/player.h"
#include "zm/server/server.h"
#include "zm/server/physics/players.h"
#include "zm/server/physics/bullets.h"


Player::Player(zm::Game& g, std::string n, bool host)
  : game(g), name(n), isHost(host), isAlive(false) {
  connected = true;
}

Player::~Player(){
  if ( body != NULL )
    delete body;
}

void Player::createBody(Physics* physics, float32 x, float32 y) {
  body = new PlayerBody(*physics, x, y);
}

void Player::jump(){
  body->jump();
}

b2Vec2 Player::getPosition(){
  return body->getPosition();
}

void Player::setPosition(int x, int y){
  body->setPosition(x,y);
}

void Player::setCamera(Camera* camera){
  this->camera = camera;
}

void Player::right(){
  if ( camera->canMoveRight(this) )
    body->right();
  else
    this->stopHorizontalMove();
}

void Player::left(){
  if ( camera->canMoveLeft(this) )
    body->left();
  else
    this->stopHorizontalMove();
}

void Player::stopHorizontalMove(){
  body->stopHorizontalMove();
}

void Player::up(){
  body->up();
}

void Player::shoot(){
  Bullet* bullet = body->shoot();
  game.currentLevel->addBullet(bullet);
}

b2Body* Player::getBody(){
  return body->getBody();
}

bool Player::collide(Bullet *bullet){
  return body->collide(bullet);
}

void Player::disconnect(){
  connected = false;
}
