
#include <string>

#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"

namespace zm {
namespace proto {


std::string ClientEvent::serialize() {
  return "foo";
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
  json game = {{"x", x}, {"y", y}, {"st", s}};
  return game.dump();
}

Game Game::deserialize(const std::string& s) {
  json j = json::parse(s);

  Game game;
  game.x = j["x"];
  game.y = j["y"];
  int st = j["st"];

  switch (st) {
    case 0: game.state = playing; break;
    case 1: game.state = won; break;
    case 2: game.state = lost; break;
  }

  return game;
}


} // proto
} // zm
