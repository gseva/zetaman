
#include <string>
#include <algorithm>
#include <functional>

#include <iostream>

#include "zm/game_protocol.h"


namespace zm {
namespace proto {


zm::proto::Position::Position(){
  x = 0;
  y = 0;
}

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

Enemy::Enemy(EnemyState state) : enemyState(state) {
}

json Enemy::getJson() {
  json j = pos.getJson();
  j["h"] = health;

  int state;
  switch (enemyState) {
    case EnemyState::movingLeft: state = 0; break;
    case EnemyState::movingRight: state = 1; break;
    case EnemyState::idle: state = 2; break;
    case EnemyState::shooting: state = 3; break;
    case EnemyState::jumping: state = 4; break;
    case EnemyState::guarded: state = 5; break;
    case EnemyState::unguarded: state = 6; break;
  }
  j["s"] = state;

  j["t"] = static_cast<int>(enemyType);

  return j;
}

Enemy Enemy::deserialize(const json& j) {
  Enemy p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];

  int state = j["s"];
  p.enemyState = static_cast<EnemyState>(state);

  int type = j["t"];
  p.enemyType = static_cast<EnemyType>(type);
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

Game::Game(GameState gs) : state(gs) {
}

std::string Game::serialize() {
  int s;
  switch (state) {
    case playing: s = 0; break;
    case won: s = 1; break;
    case lost: s = 2; break;
  }

  json playersJson = json::array();
  for (auto&& player : players) {
    playersJson.push_back(player.getJson());
  }

  json enemiesJson = json::array();
  for (auto&& enemy : enemies) {
    enemiesJson.push_back(enemy.getJson());
  }

  json proyectileJson = json::array();
  for (auto&& proyectile : proyectiles) {
    proyectileJson.push_back(proyectile.getJson());
  }

  json game = {{"x", x}, {"y", y}, {"st", s}, {"cp", camPos.getJson()},
               {"p", playersJson}, {"e", enemiesJson}, {"pr", proyectileJson}};
  std::string result = game.dump();
  return result;
}


Game Game::deserialize(const std::string& s) {
  json j = json::parse(s);

  Game game;
  game.x = j["x"];
  game.y = j["y"];
  game.camPos.x = j["cp"]["x"];
  game.camPos.y = j["cp"]["y"];

  int st = j["st"];

  for (const json &playerJson : j["p"]) {
    game.players.push_back(Player::deserialize(playerJson));
  }

  for (const json &enemyJson : j["e"]) {
    game.enemies.push_back(Enemy::deserialize(enemyJson));
  }

  for (const json &proyectileJson : j["pr"]) {
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
  json j = {{"e", static_cast<int>(state)}};
  std::string ev = j.dump();
  return ev;
}

ClientEvent ClientEvent::deserialize(const std::string& s) {
  json j = json::parse(s);
  int state = j["e"];
  ClientEvent ce(static_cast<ClientEventType>(state));
  return ce;
}


std::string ServerEvent::serialize() {
  json j = {{"e", static_cast<int>(state)}};
  std::string ev = j.dump();
  return ev;
}

ServerEvent ServerEvent::deserialize(const std::string& s) {
  json j = json::parse(s);
  int state = j["e"];
  ServerEvent ce(static_cast<ServerEventType>(state));
  return ce;
}

} // proto
} // zm
