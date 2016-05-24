#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "zm/game_protocol.h"
#include "zm/server/player.h"
#include "zm/server/enemy.h"
#include <Box2D/Box2D.h>
#include <zm/thread.h>
#include "zm/server/physics/physics.h"
#include "zm/server/timer.h"
class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();
	void jump();
	zm::Game getState();
  void right();
  void left();
  void stopHorizontalMove();

private:
  //World world;
  Physics physics;
  Timer timer;
  Player player;
  Enemy enemy;
  /*b2World* world;
  b2Vec2* gravity;

  b2BodyDef groundBodyDef;
  b2Body* groundBody;
*/
};

#endif
