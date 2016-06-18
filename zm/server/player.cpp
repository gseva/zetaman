#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include "zm/server/camera.h"

#include "zm/server/player.h"
#include "zm/server/physics/players.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"

Player::Player(){
  connected = true;
}

Player::~Player(){
  if ( body != NULL )
    delete body;
}

void Player::createBody(Physics* physics, float32 x, float32 y){
  body = new PlayerBody(*physics,x,y,*this);
  Gun* gun = new Normalgun(body, false, *physics); 
  addGun(gun);
  gun = new Firegun(body, false, *physics);
  addGun(gun);
  gun = new Ringgun(body, false, *physics);
  addGun(gun);
  gun = new Sparkgun(body, false, *physics);
  addGun(gun);
  selectedGun = 0;
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

Bullet* Player::shoot(){
  //Bullet* bullet = body->shoot();
  Gun* gun = guns[selectedGun];
  Bullet* bullet = gun->shoot();
  return bullet;
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

void Player::addGun(Gun* gun){
  guns[gun->getNumber()] = gun;
}

void Player::changeGun(int numberOfGun){
  std::map<int, Gun*>::iterator iGun = guns.find(numberOfGun);
  selectedGun = iGun != guns.end() ? numberOfGun : selectedGun;
}

void Player::tic(){
  std::map<int,Gun*>::iterator iGun;
  for ( iGun = guns.begin(); iGun != guns.end(); ++iGun ) {
    (iGun->second)->tic();
  }
}
