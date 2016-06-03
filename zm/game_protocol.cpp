
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


std::string ClientEvent::serialize() {
  return "foo";
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

  json game = {{"x", x}, {"y", y}, {"st", s}, {"p", playersJson}};
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

  switch (st) {
    case 0: game.state = playing; break;
    case 1: game.state = won; break;
    case 2: game.state = lost; break;
  }

  return game;
}


} // proto
} // zm
