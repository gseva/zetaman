#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <Box2D/Box2D.h>
#include <map>
#include "zm/server/physics/world.h"

class Camera;
class Gun;
class Bullet;

class Player{
public:
	Player();
	~Player();
	void setPosition(int x, int y);
  void setCamera(Camera* camera);
  void createBody(Physics* physics, float32 x, float32 y);
  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  void up();
  void disconnect();
  Bullet* shoot();
  b2Vec2 getPosition();
  b2Body* getBody();
  bool collide(Bullet *bullet);
  PlayerBody *body;
  bool connected;
  void addGun(Gun* gun);
  void changeGun(int numberOfGun);
  void tic();

private:
  Camera* camera;
  std::map<int, Gun*> guns;
  int selectedGun;
};

#endif
