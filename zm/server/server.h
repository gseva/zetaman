#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include <string>
#include <Box2D/Box2D.h>

#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/server/player.h"
#include "zm/server/physics/physics.h"
#include "zm/server/timer.h"
#include "zm/json/jsonserializer.h"

class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();

  void startTimer();

	void jump();
	zm::Game getState();
  void right();
  void left();
  void stopHorizontalMove();

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

private:
  Physics physics;
  Timer timer;
  Player player;
  JsonMap jm;
};

#endif
