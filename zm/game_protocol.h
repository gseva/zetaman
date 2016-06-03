#ifndef __GAME_PROTOCOL_H__
#define __GAME_PROTOCOL_H__

#include <vector>
#include <string>

#include "zm/json/json.hpp"

using json = nlohmann::json;

namespace zm {
namespace proto {


struct JsonSerializable {
  virtual json getJson() = 0;
};


struct Position : JsonSerializable {
  int x, y;
  virtual json getJson();
};


enum class PlayerState { movingLeft=0, movingRight=1, idle=2, shooting=3, jumping=4 };

struct Player {
  Position pos;
  PlayerState ps;
  int health;

  virtual json getJson();
  static Player deserialize(const json& j);
};


enum EnemyType { Met, Bumby, Sniper, JumpingSniper };

enum class EnemyState { movingLeft, movingRight, idle, shooting, jumping };

struct Enemy {
  Position pos;
  EnemyState ps;
  int health;
};


enum ProyectileType { Bomb };

struct Proyectile {
  Position pos;
};


enum PowerUpType { Life, SmallEnergy, LargeEnergy, SmallPlasma, LargePlasma };

struct PowerUp {
  Position pos;
  PowerUpType type;
};


enum GameState { playing, won, lost };

struct Game {
  Game();
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

  std::string serialize();
  static Game deserialize(const std::string& s);
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
