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
  Position();
  int x, y;
  virtual json getJson();
};


enum class PlayerState { movingLeft=0, movingRight=1, idle=2,
                         shooting=3, jumping=4 };

struct Player : JsonSerializable {
  Position pos;
  PlayerState ps;
  int health;

  virtual json getJson();
  static Player deserialize(const json& j);
};


enum EnemyType { Met=0, Bumby=1, Sniper=2, JumpingSniper=3 };

enum class EnemyState { movingLeft=0, movingRight=1, idle=2,
                        shooting=3, jumping=4, guarded=5, unguarded=6 };

struct Enemy : JsonSerializable {
  Position pos;
  EnemyState enemyState;
  EnemyType enemyType;
  int health;
  explicit Enemy(EnemyState state=EnemyState::idle);

  virtual json getJson();
  static Enemy deserialize(const json& j);
};


enum ProyectileType { Bomb };

struct Proyectile : JsonSerializable {
  Position pos;

  virtual json getJson();
  static Proyectile deserialize(const json& j);
};


enum PowerUpType { Life, SmallEnergy, LargeEnergy, SmallPlasma, LargePlasma };

struct PowerUp {
  Position pos;
  PowerUpType type;
};


enum GameState { playing, won, lost };

struct Game {
  Game();
  explicit Game(GameState gs);

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


enum ClientEventType { moveLeft, moveRight, jump, moveUp, moveDown,
                       stopMoving, shoot, shutdown, selectLevel1, selectLevel2,
                       selectLevel3, selectLevel4, selectLevel5 };

struct ClientEvent {
  ClientEventType state;

  ClientEvent() {}
  explicit ClientEvent(ClientEventType s) : state(s) {
  }

  std::string serialize();
  static ClientEvent deserialize(const std::string& s);
};


enum ServerEventType { connected, connectedAsHost };

struct ServerEvent {
  ServerEventType state;

  ServerEvent() {}
  explicit ServerEvent(ServerEventType s) : state(s) {
  }

  std::string serialize();
  static ServerEvent deserialize(const std::string& s);
};

} // p
} // zm

#endif
