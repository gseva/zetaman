#include <Box2D/Box2D.h>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include "zm/config.h"
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
  lifes = zm::config::lifes;
  gunsWon.insert(zm::proto::Normal);
}

Player::~Player(){
  if ( body != NULL )
    delete body;

  deleteGuns();
}

void Player::deleteGuns() {
  for (auto&& pair : guns) {
    delete pair.second;
  }
}

void Player::createBody(Physics* physics, float32 x, float32 y){
  deleteGuns();
  isAlive = true;

  body = new PlayerBody(*physics, x, y, this);

  for (auto&& proyectileType : gunsWon) {
    Gun* g;
    switch (proyectileType) {
      case zm::proto::Normal: g = new Normalgun(body, false, *physics); break;
      case zm::proto::Fire: g = new Firegun(body, false, *physics); break;
      case zm::proto::Ring: g = new Ringgun(body, false, *physics); break;
      case zm::proto::Spark: g = new Sparkgun(body, false, *physics); break;
      case zm::proto::Magnet: g = new Magnetgun(body, false, *physics); break;
      case zm::proto::Bomb: g = new Bombgun(body, false, *physics); break;
    }
    addGun(g);
  }
  selectedGun = zm::proto::Normal;
}

void Player::jump(){
  if (isAlive) {
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
  if ( isAlive && camera->canMoveRight(this) ) {
    body->right();
    action = LastAction::right;
  } else {
    this->stopHorizontalMove();
    action = LastAction::idle;
  }
}

void Player::left() {
  orientation = zm::proto::left;
  if ( isAlive && camera->canMoveLeft(this) ) {
    body->left();
    action = LastAction::left;
  } else if (isAlive) {
    this->stopHorizontalMove();
    action = LastAction::right;
  }
}

void Player::stopHorizontalMove(){
  if (isAlive)
    body->stopHorizontalMove();
}

void Player::up(){
  if ( isAlive &&  body->up() ) {
    action = LastAction::up;
  } else {
    action = LastAction::idle;
  }
}

Gun* Player::getCurrentGun() {
  return guns[static_cast<int>(selectedGun)];
}

void Player::shoot(){
  if (!isAlive)
    return;
  Gun* gun = getCurrentGun();
  Bullet* bullet = gun->shoot();
  std::cout << "Creo bala " << bullet << std::endl;
  if (bullet) {
    action = LastAction::shoot;
    game.currentLevel->addBullet(bullet);
  }
}

void Player::destroy() {
  lifes--;
  isAlive = false;
  delete body;
}


b2Body* Player::getBody(){
  return body->getBody();
}

void Player::disconnect(){
  connected = false;
}

void Player::addNewGun(int gun) {
  gunsWon.insert(static_cast<zm::proto::ProyectileType>(gun));
}

void Player::addGun(Gun* gun){
  guns[gun->getNumber()] = gun;
}

void Player::changeGun(int numberOfGun) {
  std::map<int, Gun*>::iterator iGun = guns.find(numberOfGun);
  int gun = iGun != guns.end() ? numberOfGun : selectedGun;
  selectedGun = static_cast<zm::proto::ProyectileType>(gun);
}

void Player::tic(){
  std::map<int,Gun*>::iterator iGun;
  for ( iGun = guns.begin(); iGun != guns.end(); ++iGun ) {
    (iGun->second)->tic();
  }
}


bool Player::addLife() {
  if (lifes == zm::config::lifes) return false;
  lifes++;
  return true;
}

bool Player::addAmmo(int amount) {
  Gun* gun = getCurrentGun();
  int actualAmount = gun->getAmmunition();
  if (actualAmount >= 100) return false;

  gun->addAmmunitions(std::min(amount, 100 - actualAmount));
  return true;
}

bool Player::addHealth(int amount) {
  if (body->health >= zm::config::playerLife) return false;

  body->addHealth(std::min(amount, zm::config::playerLife - body->health));
  return true;
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

  player.weapon = selectedGun;
  player.health = body->health;
  player.o = orientation;
  player.ammo = getCurrentGun()->getAmmunition();
  player.pos.x = getPosition().x - xo;
  player.pos.y = getPosition().y;
  return player;
}
