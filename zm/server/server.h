#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "zm/game_protocol.h"
#include "zm/server/player.h"
#include <Box2D/Box2D.h>
#include <zm/thread.h>
#include "zm/server/physical/physical.h"
#include "zm/server/timer.h"
class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();
	void jump();
	GameState getState();
  void right();
  void left();
  void stopHorizontalMove();

private:
  //World world;
  Physical physical;
  Timer timer;
  Player player;
  /*b2World* world;
  b2Vec2* gravity;

  b2BodyDef groundBodyDef;
  b2Body* groundBody;
*/
};

#endif
