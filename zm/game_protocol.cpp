
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
  j["i"] = id;
  j["h"] = health;

  int state;
  switch (ps) {
    case PlayerState::moving: state = 0; break;
    case PlayerState::idle: state = 1; break;
    case PlayerState::shooting: state = 2; break;
    case PlayerState::jumping: state = 3; break;
    case PlayerState::jumpingShooting: state = 4; break;
    case PlayerState::climbing: state = 5; break;
  }
  j["st"] = state;
  j["o"] = static_cast<int>(o);

  return j;
}

Player Player::deserialize(const json& j) {
  Player p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];
  int state = j["st"];
  p.ps = static_cast<PlayerState>(state);
  int orientation = j["o"];
  p.o = static_cast<Orientation>(orientation);
  p.id = j["i"];
  return p;
}

Enemy::Enemy(EnemyState state) : enemyState(state) {
}

json Enemy::getJson() {
  json j = pos.getJson();
  j["h"] = health;

  int state;
  switch (enemyState) {
    case EnemyState::moving: state = 0; break;
    case EnemyState::idle: state = 1; break;
    case EnemyState::shooting: state = 2; break;
    case EnemyState::jumping: state = 3; break;
    case EnemyState::guarded: state = 4; break;
  }
  j["s"] = state;

  j["t"] = static_cast<int>(enemyType);
  j["o"] = static_cast<int>(o);
  j["i"] = id;

  return j;
}

Enemy Enemy::deserialize(const json& j) {
  Enemy e;
  e.pos.x = j["x"];
  e.pos.y = j["y"];
  e.health = j["h"];

  int state = j["s"];
  e.enemyState = static_cast<EnemyState>(state);

  int type = j["t"];
  e.enemyType = static_cast<EnemyType>(type);

  int orientation = j["o"];
  e.o = static_cast<Orientation>(orientation);

  e.id = j["i"];

  return e;
}


json Proyectile::getJson() {
  json j = pos.getJson();
  j["t"] = static_cast<int>(type);
  j["i"] = id;
  return j;
}

Proyectile Proyectile::deserialize(const json& j) {
  Proyectile p;
  p.pos.x = j["x"];
  p.pos.y = j["y"];
  p.id = j["i"];
  int type = j["t"];
  p.type = static_cast<ProyectileType>(type);
  return p;
}


Game::Game() : state(GameState::playing) {
}

Game::Game(GameState gs) : state(gs) {
}

std::string Game::serialize() {
  int s = static_cast<int>(state);

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

  json game = {{"st", s}, {"cp", camPos.getJson()},
               {"p", playersJson}, {"e", enemiesJson}, {"pr", proyectileJson}};
  std::string result = game.dump();
  return result;
}


Game Game::deserialize(const std::string& s) {
  json j = json::parse(s);

  Game game;
  game.camPos.x = j["cp"]["x"];
  game.camPos.y = j["cp"]["y"];

  int st = j["st"];
  game.state = static_cast<GameState>(st);

  for (const json &playerJson : j["p"]) {
    game.players.push_back(Player::deserialize(playerJson));
  }

  for (const json &enemyJson : j["e"]) {
    game.enemies.push_back(Enemy::deserialize(enemyJson));
  }

  for (const json &proyectileJson : j["pr"]) {
    game.proyectiles.push_back(Proyectile::deserialize(proyectileJson));
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


ServerEvent::ServerEvent() : payload("") {
}

ServerEvent::ServerEvent(ServerEventType s) : state(s), payload("") {
}


std::string ServerEvent::serialize() {
  json j = {{"e", static_cast<int>(state)}};

  if (payload != "") {
    j["p"] = payload;
  }

  std::string ev = j.dump();
  return ev;
}

ServerEvent ServerEvent::deserialize(const std::string& s) {
  json j = json::parse(s);
  int state = j["e"];

  ServerEvent ce(static_cast<ServerEventType>(state));

  if (j.find("p") != j.end()) {
    ce.payload = j["p"];
  }

  return ce;
}

} // proto
} // zm
