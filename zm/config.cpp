
#include <iostream>
#include <fstream>
#include <string>

#include "zm/json/json.hpp"
#include "zm/config.h"

namespace zm {

namespace config {

int gamePeriod = 1000/60;
int bossLife = 30;
int playerLife = 50;
int lifes = 50;


void loadConfigFile(const std::string& file) {
  std::ifstream fs(file);

  nlohmann::json j;

  fs >> j; //Carga el json en memoria

  fs.close();

  gamePeriod = j.value("gamePeriod", gamePeriod);
  std::cout << "Game period: " << gamePeriod << std::endl;

  bossLife = j.value("bossLife", bossLife);
  std::cout << "Boss life: " << bossLife << std::endl;

  playerLife = j.value("playerLife", playerLife);
  std::cout << "Player life: " << playerLife << std::endl;

  lifes = j.value("lifes", lifes);
  std::cout << "lifes: " << lifes << std::endl;
}


} // config

} // zm
