#ifndef __CAMARA_H__
#define __CAMARA_H__
#include <vector>
#include "zm/server/player.h"
#include "zm/server/physics/world.h" //body.h

class Camera
{
public:
  explicit Camera(std::vector<Player*>& players);
  virtual ~Camera();
  bool canMoveRight(Player* player);
  bool canMoveLeft(Player* player);
  bool isInFrame(Body* body);
  zm::proto::Position calculatePosition();
private:
  zm::proto::Position getPosition();
  bool isInCenter(PlayerBody* playerBody);
  std::vector<Player*>& players;
  zm::proto::Position position;
  zm::proto::Position bossPosition;
};

#endif
