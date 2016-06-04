
#include <string>
#include <algorithm>
#include <functional>

#include <iostream>

#include "zm/game_protocol.h"


namespace zm {
namespace proto {


json Position::getJson() {
  return {{"x", x}, {"y", y}};
}


json Player::getJson() {
  json j = pos.getJson();
  j["h"] = health;

  int state;
  switch (ps) {
    case PlayerState::movingLeft: state = 0; break;
    case PlayerState::movingRight: state = 1; break;
    case PlayerState::idle: state = 2; break;
    case PlayerState::shooting: state = 3; break;
    case PlayerState::jumping: state = 4; break;
  }
  j["st"] = state;

  return j;
}

Player Player::deserialize(const json& j) {
  Player p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];
  int state = j["st"];
  p.ps = static_cast<PlayerState>(state);
  return p;
}

Enemy::Enemy(EnemyState state) : es(state) {
}

json Enemy::getJson() {
  json j = pos.getJson();
  j["h"] = health;

  int state;
  switch (es) {
    case EnemyState::movingLeft: state = 0; break;
    case EnemyState::movingRight: state = 1; break;
    case EnemyState::idle: state = 2; break;
    case EnemyState::shooting: state = 3; break;
    case EnemyState::jumping: state = 4; break;
  }
  j["st"] = state;

  return j;
}

Enemy Enemy::deserialize(const json& j) {
  Enemy p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];
  int state = j["st"];
  p.es = static_cast<EnemyState>(state);
  return p;
}


json Proyectile::getJson() {
  return pos.getJson();
}

Proyectile Proyectile::deserialize(const json& j) {
  Proyectile p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];
  return p;
}


Game::Game() : state(GameState::playing) {
}

std::string Game::serialize() {
  int s;
  switch (state) {
    case playing: s = 0; break;
    case won: s = 1; break;
    case lost: s = 2; break;
  }

  json playersJson = json::array();
  for(auto&& player : players) {
    playersJson.push_back(player.getJson());
  }

  json enemiesJson = json::array();
  for(auto&& enemy : enemies) {
    enemiesJson.push_back(enemy.getJson());
  }

  json proyectileJson = json::array();
  for(auto&& proyectile : proyectiles) {
    proyectileJson.push_back(proyectile.getJson());
  }

  json game = {{"x", x}, {"y", y}, {"st", s},
               {"p", playersJson}, {"e", enemiesJson}, {"p", proyectileJson}};
  std::string result = game.dump();
  std::cout << "Serializando game: " << result << "\n";
  return result;
}

Game Game::deserialize(const std::string& s) {
  std::cout << "Deserializando game: " << s << "\n";
  json j = json::parse(s);

  Game game;
  game.x = j["x"];
  game.y = j["y"];
  int st = j["st"];

  for (const json &playerJson : j["p"]) {
    game.players.push_back(Player::deserialize(playerJson));
  }

  for (const json &enemyJson : j["e"]) {
    game.enemies.push_back(Enemy::deserialize(enemyJson));
  }

  for (const json &proyectileJson : j["p"]) {
    game.proyectiles.push_back(Proyectile::deserialize(proyectileJson));
  }

  switch (st) {
    case 0: game.state = playing; break;
    case 1: game.state = won; break;
    case 2: game.state = lost; break;
  }

  return game;
}


std::string ClientEvent::serialize() {
  return "foo";
}

} // proto
} // zm
