#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "zm/game_protocol.h"
#include "zm/server/player.h"
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
  Physics physics;
  Timer timer;
  Player player;
};

#endif
