
#include <string>

#include "zm/game_protocol.h"
#include "zm/json/jsonserializer.h"

namespace zm {
namespace proto {


std::string ClientEvent::serialize() {
  return "foo";
}


std::string Game::serialize() {
  json game = {{"x", x}, {"y", y}};
  return game.dump();
}

Game Game::deserialize(const std::string& s) {
  json j = json::parse(s);

  Game game;
  game.x = j["x"];
  game.y = j["y"];
  return game;
}


} // proto
} // zm
