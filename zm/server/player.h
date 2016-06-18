#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Box2D/Box2D.h>
#include <string>

#include "zm/server/physics/world.h"

class Camera;
class Bullet;
namespace zm {
  class Game;
} // zm

class Player{
public:
  zm::Game& game;
  std::string name;
  bool isHost;

	Player(zm::Game& g, std::string name, bool host);
	~Player();

  void setPosition(int x, int y);
  void setCamera(Camera* camera);
  void createBody(Physics* physics, float32 x, float32 y);

  void right();
	void left();
  void stopHorizontalMove();
	void jump();
  void up();
  void shoot();

  void disconnect();
  b2Vec2 getPosition();
  b2Body* getBody();
  bool collide(Bullet *bullet);
  PlayerBody *body;
  bool connected;

private:
  Camera* camera;
};

#endif
