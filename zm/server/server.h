#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>
#include <memory>
#include <map>
#include <Box2D/Box2D.h>

#include "zm/connection.h"
#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/json/jsonserializer.h"
#include "zm/server/physics/world.h"
#include "zm/server/level.h"


class Player;


class Server{
public:
  Server();
  ~Server();

  void run();

	zm::proto::Game getState();
  void updateState();

  void stopAccepting();

private:
  zm::Socket* accepter_;
  std::string port_;
};


namespace zm {

class Game {
Server& server_;
JsonMap currentMap_;
std::string mapPath_;

std::map<std::string, Player*> players;
std::map<std::string, zm::ClientProxy*> proxies;

bool accepting_;
bool playing_;

public:
  Level* currentLevel;

  explicit Game(Server& s);
  ~Game();

  void acceptHost(zm::Socket* accepter);
  void acceptPlayers(zm::Socket* accepter);

  void newPlayer(std::shared_ptr<zm::ProtectedSocket> sock);

  void selectLevel(int level);
  void selectMap();
  void startLevel();
  void gameLoop();

  void updateState();

  void shutdown(const std::string& playerName);
};

} // zm

#endif
