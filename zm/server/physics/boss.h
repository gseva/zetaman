#ifndef __BOSS_H__
#define __BOSS_H__
#include "zm/server/physics/enemies.h"
#include "zm/server/physics/gun.h"
#include "zm/server/player.h"
#include <vector>

class Boss : public Enemy{
public:
  explicit Boss(Physics& physics, float32 x, float32 y,
    int velocity, int jump, int shootFrecuency, int jumpFrecuency,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Boss()=0;
  virtual Bullet* move();
  virtual zm::proto::Enemy toBean(int xo, int yo)=0;
  float getMaxJumpHeight();
  void determinePositionsToGo();
  void choosePosition();
  float moveTowardsPosition();
  bool gotCloseEnough();
  zm::proto::Position getPlayersAveragePosition();
protected:
  int velocity;
  Gun* gun;
  const int jump;
  const int shootFrecuency;
  const int jumpFrecuency;
  /*Posicion a la que va a moverse*/
  float positionToGo;
  bool moving;
  /*Posiciones a las que puede moverse en x*/
  std::vector<float> positionsCanGo;
  JsonMap jm;
  std::vector<Player*>& players;
private:
  unsigned int tics;
};

class Bombman : public Boss{
public:
  explicit Bombman(Physics& physics, float32 x, float32 y,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Bombman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Magnetman: public Boss{
public:
  explicit Magnetman(Physics& physics, float32 x, float32 y,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Magnetman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Sparkman : public Boss{
public:
  explicit Sparkman(Physics& physics, float32 x, float32 y,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Sparkman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Ringman : public Boss{
public:
  explicit Ringman(Physics& physics, float32 x, float32 y,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Ringman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

class Fireman : public Boss{
public:
  explicit Fireman(Physics& physics, float32 x, float32 y,
    JsonMap jm, std::vector<Player*>& players);
  virtual ~Fireman();
  virtual zm::proto::Enemy toBean(int xo, int yo);
};

#endif
