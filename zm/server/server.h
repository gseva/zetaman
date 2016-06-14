#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include <string>
#include <memory>
#include <Box2D/Box2D.h>

#include "zm/connection.h"
#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/player.h"
#include "zm/server/physics/world.h"
#include "zm/server/timer.h"
#include "zm/server/level.h"


class Server{
private:
  void newPlayer_();
  void newClientProxy_(std::shared_ptr<zm::ProtectedSocket> sock);

public:
  Server();
  ~Server();

  void run();

  void startLevel();
  void stopLevel();

	zm::proto::Game getState();
  void updateState();

  void selectLevel(int level);

  void jump(int playerNumber);
  void right(int playerNumber);
  void left(int playerNumber);
  void stopHorizontalMove(int playerNumber);
  void up(int playerNumber);
  void shoot(int playerNumber);
  void shutdown(int playerNumber);

private:
  Level* level;
  zm::Socket* accepter_;
  std::vector<Player*> players;
  JsonMap jm;
  std::vector<zm::ClientProxy*> proxies;
  std::string port_;
  std::string mapPath_;
  bool accepting_;
  bool playing_;
};

#endif
