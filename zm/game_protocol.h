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
  float x, y;
  virtual json getJson();
};

enum Orientation { left=1, right=2 };

enum class PlayerState { moving=0, idle=1, shooting=2, jumping=3,
                         jumpingShooting=4, climbing=5 };

enum ProyectileType { Normal=0, Bomb=1, Spark=2,
  Magnet=3, Ring=4, Fire=5 };

struct Player : JsonSerializable {
  Position pos;
  PlayerState ps;
  int health;
  int id;
  Orientation o;
  ProyectileType weapon;

  virtual json getJson();
  static Player deserialize(const json& j);
};


enum EnemyType { Met=0, Bumby=1, Sniper=2, JumpingSniper=3,
  Bombman=4, Magnetman=6, Sparkman=7, Ringman=8, Fireman=9 };

enum class EnemyState { moving=0, idle=1, shooting=2, jumping=3, guarded=4 };

struct Enemy : JsonSerializable {
  Position pos;
  EnemyState enemyState;
  EnemyType enemyType;
  Orientation o;
  int health;
  int id;
  explicit Enemy(EnemyState state=EnemyState::idle);

  virtual json getJson();
  static Enemy deserialize(const json& j);
};

struct Proyectile : JsonSerializable {
  Position pos;
  ProyectileType type;
  int id;
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

  // Id del jugador al que se le envian los datos
  int playerId;

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
                       selectLevel3, selectLevel4, selectLevel5,
                       selectGun1, selectGun2, selectGun3,
                       selectGun4, selectGun5, selectGun6,
                       readyToPlay };

struct ClientEvent {
  ClientEventType state;

  ClientEvent() {}
  explicit ClientEvent(ClientEventType s) : state(s) {
  }

  std::string serialize();
  static ClientEvent deserialize(const std::string& s);
};


enum ServerEventType { connected, connectedAsHost, playerConnected,
  mapSelected, gameStart, levelWon, levelWonHost };

struct ServerEvent {
  ServerEventType state;
  std::string payload;

  ServerEvent();
  explicit ServerEvent(ServerEventType s);

  std::string serialize();
  static ServerEvent deserialize(const std::string& s);
};

} // p
} // zm

#endif
