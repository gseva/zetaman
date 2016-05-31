#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include <string>
#include <Box2D/Box2D.h>

#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/player.h"
#include "zm/server/physics/physics.h"
#include "zm/server/timer.h"
#include "zm/server/level.h"


class Server{
public:
	Server();
	~Server();

  void run();

  void newPlayer();
  void startLevel();
  void stopLevel();

	void jump(int playerNumber);
	zm::proto::Game getState();
  void right(int playerNummber);
  void left(int playerNumber);
  void stopHorizontalMove(int playerNumber);
  void up(int playerNumber);
  void shoot(int playerNumber);

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();

private:
  Level* level;
  std::vector<Player*> players;
  JsonMap jm;
  zm::ClientProxy* cp;
};

#endif
