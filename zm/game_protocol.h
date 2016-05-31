#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__

#include <vector>
#include <string>

namespace zm {
namespace proto {


class Position {
public:
  int x, y;
};

template <typename T>
struct StatefulEntity {
  T state;
  StatefulEntity() {}
  explicit StatefulEntity(T s) : state(s) {
  }
};

enum class PlayerState { movingLeft, movingRight, idle, shooting, jumping };


class Player {
public:
  Position pos;
  PlayerState ps;
  int health;
};


enum EnemyType { Met, Bumby, Sniper, JumpingSniper };

enum class EnemyState { movingLeft, movingRight, idle, shooting, jumping };

class Enemy {
public:
  Position pos;
  EnemyState ps;
  int health;
};


enum ProyectileType { Bomb };

class Proyectile {
public:
  Position pos;
};


enum PowerUpType { Life, SmallEnergy, LargeEnergy, SmallPlasma, LargePlasma };

class PowerUp {
public:
  Position pos;
  PowerUpType type;
};


enum GameState { playing, won, lost };

class Game {
public:
  int x, y;

  // Esta posicion es para que el cliente entienda que tiles dibujar.
  // Hay que ver como mandar esto.
  Position camPos;

  GameState state;

  // la idea es en vez de cambiar x e y del GameState, cambiar los de cada
  // player
  std::vector<Player> players;
  std::vector<Enemy> enemies;
  std::vector<Proyectile> proyectiles;
  std::vector<PowerUp> powerUps;
};


enum ClientEventType { moveLeft, moveRight, jump,
                       stopMoving, shoot, shutdown };

struct ClientEvent {
  ClientEventType state;
  ClientEvent() {}
  explicit ClientEvent(ClientEventType s) : state(s) {
  }
  std::string serialize();
};

} // p
} // zm

#endif
