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
#include "zm/server/level.h"

class ServerProxy;

class Server{
public:
	explicit Server(ServerProxy& sp);
	~Server();
  void newPlayer();
  void startLevel();

	void jump(int playerNummber);
	zm::proto::Game getState();
  void right(int playerNummber);
  void left(int playerNummber);
  void stopHorizontalMove(int playerNummber);
  void up(int playerNummber);

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

private:
  Level* level;
  std::vector<Player*> players;
  JsonMap jm;
  ServerProxy& sp;
};

#endif
