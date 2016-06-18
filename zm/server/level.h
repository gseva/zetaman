#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#include "zm/json/jsonserializer.h"
#include "zm/server/camera.h"
#include "zm/server/client_proxy.h"
#include "zm/server/player.h"
#include "zm/server/physics/world.h"
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/bullets.h"


class Level{
public:
  zm::proto::GameState state;

  Level(std::vector<Player*> connectedPlayers, JsonMap& jsonMap);
  ~Level();

  zm::proto::Game getState();
  void addBullet(Bullet* bullet);

  void step();
  void clean();

  bool checkLoseCondition();
  bool checkWinCondition();

  std::vector<Player*> players;
  std::vector<Enemy*> enemies;
  std::vector<Bullet*> bullets;

private:
  Physics physics;
  JsonMap& jm;
  Camera camera;
};
#endif
