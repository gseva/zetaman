#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include <string>
#include "zm/server/camera.h"

#include "zm/server/player.h"
#include "zm/server/server.h"
#include "zm/server/physics/players.h"
#include "zm/server/physics/gun.h"
#include "zm/server/physics/bullets.h"



Player::Player(zm::Game& g, std::string n, bool host) : game(g), name(n),
  isHost(host), isAlive(false), isReady(false), orientation(zm::proto::right),
  action(LastAction::idle) {
  connected = true;
}

Player::~Player(){
  if ( body != NULL )
    delete body;
}

void Player::createBody(Physics* physics, float32 x, float32 y){
  body = new PlayerBody(*physics, x, y);
  Gun* gun = new Normalgun(body, false, *physics);
  addGun(gun);
  gun = new Firegun(body, false, *physics);
  addGun(gun);
  gun = new Ringgun(body, false, *physics);
  addGun(gun);
  gun = new Sparkgun(body, false, *physics);
  addGun(gun);
  gun = new Magnetgun(body, false, *physics);
  addGun(gun);
  gun = new Bombgun(body, false, *physics);
  addGun(gun);
  selectedGun = 0;
}

void Player::jump(){
  if (!body->isDestroyed()) {
    body->jump();
    action = LastAction::jump;
  }
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

void Player::setReady() {
  isReady = true;
  game.notifyPlayerReady();
}

void Player::right(){
  orientation = zm::proto::right;
  if ( camera->canMoveRight(this) && !body->isDestroyed() ) {
    body->right();
    action = LastAction::right;
  } else {
    this->stopHorizontalMove();
    action = LastAction::idle;
  }
}

void Player::left(){
  orientation = zm::proto::left;
  if ( camera->canMoveLeft(this) && !body->isDestroyed() ) {
    body->left();
    action = LastAction::left;
  } else {
    this->stopHorizontalMove();
    action = LastAction::right;
  }
}

void Player::stopHorizontalMove(){
  if (!body->isDestroyed() )
    body->stopHorizontalMove();
}

void Player::up(){
  if (body->up() && !body->isDestroyed()) {
    action = LastAction::up;
  } else {
    action = LastAction::idle;
  }
}

void Player::shoot(){
  if (body->isDestroyed() )
    return;
  Gun* gun = guns[selectedGun];
  Bullet* bullet = gun->shoot();
  std::cout << "Creo bala " << bullet << std::endl;
  if (bullet) {
    action = LastAction::shoot;
    game.currentLevel->addBullet(bullet);
  }
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

void Player::changeGun(int numberOfGun) {
  std::map<int, Gun*>::iterator iGun = guns.find(numberOfGun);
  selectedGun = iGun != guns.end() ? numberOfGun : selectedGun;
}

void Player::tic(){
  std::map<int,Gun*>::iterator iGun;
  for ( iGun = guns.begin(); iGun != guns.end(); ++iGun ) {
    (iGun->second)->tic();
  }
}


zm::proto::Player Player::toBean(int xo, int yo) {
  zm::proto::Player player;
  b2Vec2 vel = body->getBody()->GetLinearVelocity();
  zm::proto::PlayerState state;
  if (vel.y != 0) {
    if (action == LastAction::shoot) {
      state = zm::proto::PlayerState::jumpingShooting;
    } else if (action == LastAction::up) {
      state = zm::proto::PlayerState::climbing;
    } else {
      state = zm::proto::PlayerState::jumping;
    }
  } else if (action == LastAction::shoot) {
    state = zm::proto::PlayerState::shooting;
  } else if (vel.x != 0) {
    state = zm::proto::PlayerState::moving;
  } else {
    state = zm::proto::PlayerState::idle;
  }
  player.ps = state;
  player.id = body->getId();
  player.health = body->health;
  player.o = orientation;
  player.pos.x = getPosition().x - xo;
  player.pos.y = getPosition().y;
  return player;
}
