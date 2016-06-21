#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>
#include <Box2D/Box2D.h>
#include "zm/json/jsonserializer.h"
#include "zm/server/player.h"
#include "zm/server/camera.h"
#include "zm/server/client_proxy.h"
#include "zm/server/physics/world.h"
#include "zm/server/physics/boss.h"
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/bullets.h"
#include "zm/server/physics/powerup.h"


class Level{
public:
  zm::proto::GameState state;
  Boss* boss;

  Level(std::vector<Player*> connectedPlayers, JsonMap& jsonMap);
  ~Level();

  zm::proto::Game getState();
  void addBullet(Bullet* bullet);

  void changeGun(int playerNumber, int gunNumber);

  void step();
  void clean();

  bool checkLoseCondition();
  bool checkWinCondition();

  std::vector<Player*> players;
  std::vector<Enemy*> enemies;
  std::vector<Bullet*> bullets;
  std::vector<PowerUp*> powerUps;

private:
  Physics physics;
  JsonMap& jm;
  Camera camera;
  PowerUp* createPowerUp(b2Vec2 pos);
};
#endif
