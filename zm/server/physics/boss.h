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
    int prefDistance, JsonMap jm, std::vector<Player*>& players);
  virtual ~Boss()=0;
  virtual Bullet* move();
  virtual zm::proto::Enemy toBean(int xo, int yo)=0;
  void determinePositionsToGo();
  void choosePosition();
  float moveTowardsPosition();
  zm::proto::Position getPlayersAveragePosition();
  virtual void toImpact(Bomb* bullet);
  virtual void toImpact(Spark* bullet);
  virtual void toImpact(Ring* bullet);
  virtual void toImpact(Magnet* bullet);
  virtual void toImpact(Fire* bullet);
  virtual void toImpact(Bullet* bullet);

protected:
  int velocity;
  Gun* gun;
  const int jump;
  const int shootFrecuency;
  const int jumpFrecuency;
  const int prefDistance; 
  /*Posicion a la que va a moverse*/
  float positionToGo;
  /*Posiciones a las que puede moverse en x*/
  std::vector<float> positionsCanGo;
  JsonMap jm;
  std::vector<Player*>& players;
private:
  void damage (int healt);
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
