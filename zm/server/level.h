#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#include "zm/server/player.h"
#include "zm/server/physics/physics.h"
#include "zm/server/timer.h"
#include "zm/server/client_proxy.h"
#include "zm/json/jsonserializer.h"

class Server;

class Level{
public:
  explicit Level(std::vector<Player*>& connectedPlayers,
    const std::string& path, ClientProxy& cp);
  ~Level();

  void jump(int playerNumber);
  zm::proto::Game getState();
  void right(int playerNumber);
  void left(int playerNumber);
  void stopHorizontalMove(int playerNumber);
  void up(int playerNumber);
  void shoot(int playerNumber);

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();
private:
  Physics physics;
  Timer timer;
  std::vector<Player*>& players;
  std::vector<Enemy*> enemies;
  std::vector<Bullet*> bullets;
  JsonMap jm;
};
#endif
