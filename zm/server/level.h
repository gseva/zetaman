#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#include "zm/json/jsonserializer.h"
#include "zm/server/player.h"
#include "zm/server/timer.h"
#include "zm/server/camera.h"
#include "zm/server/client_proxy.h"
#include "zm/server/physics/world.h"
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/bullets.h"

class Server;

class Level{
public:
  explicit Level(std::vector<Player*>& connectedPlayers,
    JsonMap& jsonMap, Server& s);
  ~Level();

  void jump(int playerNumber);
  zm::proto::Game getState();
  void right(int playerNumber);
  void left(int playerNumber);
  void stopHorizontalMove(int playerNumber);
  void up(int playerNumber);
  void shoot(int playerNumber);
  void disconnect(int playerNumber);

  void clean();

  std::vector<Player*>& players;
  std::vector<Enemy*> enemies;
  std::vector<Bullet*> bullets;

private:
  Physics physics;
  Timer timer;
  JsonMap& jm;
  Camera camera;
};
#endif
