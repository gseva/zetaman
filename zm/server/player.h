#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <Box2D/Box2D.h>
#include <string>
#include <map>
#include <set>

#include "zm/game_protocol.h"
#include "zm/server/physics/world.h"

class Camera;
class Gun;
class Bullet;

namespace zm {
  class Game;
} // zm


enum class LastAction { left, right, up, jump, shoot, idle };


class Player{
public:
  zm::Game& game;
  std::string name;
  bool isHost;
  bool isAlive;
  bool isReady;

  Player(zm::Game& g, std::string name, bool host);
  ~Player();

  void setPosition(int x, int y);
  void setCamera(Camera* camera);
  void createBody(Physics* physics, float32 x, float32 y);
  void setReady();

  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  void up();
  void shoot();

  void destroy();

  void disconnect();
  b2Vec2 getPosition();
  b2Body* getBody();
  bool collide(Bullet *bullet);

  PlayerBody *body;
  bool connected;

  void deleteGuns();
  void addNewGun(int gun);
  void addGun(Gun* gun);
  void changeGun(int numberOfGun);
  Gun* getCurrentGun();
  void tic();

  bool addHealth(int amount);
  bool addAmmo(int amount);
  bool addLife();

  zm::proto::Player toBean(int xo, int yo);

private:
  int lifes;
  Camera* camera;
  std::map<int, Gun*> guns;
  std::set<zm::proto::ProyectileType> gunsWon;
  zm::proto::ProyectileType selectedGun;
  zm::proto::Orientation orientation;
  LastAction action;
};

#endif
