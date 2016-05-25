#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <string>

#include "zm/server/player.h"
#include "zm/server/physics/physics.h"
#include "zm/server/timer.h"
#include "zm/json/jsonserializer.h"

class Server;

class Level{
public:
  explicit Level(std::vector<Player*>& connectedPlayers,
    const std::string& path, ServerProxy& sp);
  ~Level();

  void jump(int playerNummber);
  zm::proto::Game getState();
  void right(int playerNummber);
  void left(int playerNummber);
  void stopHorizontalMove(int playerNummber);

  std::vector<std::string> getImageNames();
  std::vector<int> getImages();
private:
  Physics physics;
  Timer timer;
  std::vector<Player*>& players;
  JsonMap jm;
};
#endif
