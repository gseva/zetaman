
#include <iostream>
#include <fstream>

#include "zm/json/json.hpp"
#include "zm/server/config.h"

namespace zm {

namespace config {

int gamePeriod = 1000/60;
int bossLife = 30;


void loadConfigFile(const std::string& file) {
  std::ifstream fs(file);

  nlohmann::json j;

  fs >> j; //Carga el json en memoria

  fs.close();

  gamePeriod = j.value("gamePeriod", gamePeriod);
  std::cout << "Game period: " << gamePeriod << std::endl;

  bossLife = j.value("bossLife", bossLife);
  std::cout << "Boss life: " << bossLife << std::endl;
}


} // config

} // zm
