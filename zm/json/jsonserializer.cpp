#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <string>

#include "zm/json/jsonserializer.h"


std::string JsonMap::getReducedString() {
  json map;

  map = {{"tiles", imageNumbers}, {"images", imageNames},
         {"background", backgroundImage}};

  return map.dump();
}

void JsonMap::fromReducedString(const std::string& s) {
  setlocale(LC_NUMERIC, "C");
  json j = json::parse(s);

  std::vector<int> imageNumbersVector = j["tiles"];
  std::vector<std::string> imageNamesVector = j["images"];

  imageNumbers = imageNumbersVector;
  imageNames = imageNamesVector;

  if (j.find("background") != j.end()) {
    backgroundImage = j["background"];
  } else {
    backgroundImage = "";
  }
}


JsonMap JsonSerializer::importMap(std::string path)
{
  std::ifstream fs(path);

  json j;

  fs >> j; //Carga el json en memoria

  fs.close();

  JsonMap mapa;

  //Carga en el vector todo lo contenido de "tiles"
  std::vector<int> imageNumbersVector = j["tiles"];
  std::vector<std::string> imageNamesVector = j["images"];
  std::vector<std::string> physicsVector = j["physics"];
  std::vector<std::string> spawnTypesVector = j["spawnTypes"];
  json spawns = j["spawns"];

  for (json::iterator it = spawns.begin(); it != spawns.end(); ++it) {
    json singleSpawn = *it;
    SpawnData spawnData;
    spawnData.column = singleSpawn["x"];
    spawnData.row = singleSpawn["y"];
    spawnData.type = singleSpawn["type"];
    mapa.spawnsData.push_back(spawnData);
  }

  mapa.imageNumbers = imageNumbersVector;
  mapa.imageNames = imageNamesVector;
  mapa.physics = physicsVector;
  mapa.spawnTypes = spawnTypesVector;

  if (j.find("background") != j.end()) {
    mapa.backgroundImage = j["background"];
  } else {
    mapa.backgroundImage = "";
  }

  return mapa;
}

void JsonSerializer::exportMap(std::string path, const JsonMap& m)
{
  json exportable;

  exportable = {{"background", ""},
                {"tiles", m.imageNumbers}, {"images", m.imageNames},
                {"physics", m.physics}, {"spawnTypes", m.spawnTypes}};

  json exportableSpawns = json::array();

  for (unsigned int i=0; i<m.spawnsData.size(); i++)
  {
    exportableSpawns.push_back({
      {"x", m.spawnsData[i].column},
      {"y", m.spawnsData[i].row},
      {"type", m.spawnsData[i].type}
    });
  }

  exportable["spawns"] = exportableSpawns;

  std::fstream newFile;

  newFile.open(path, std::fstream::in | std::fstream::out | std::fstream::app);

  exportable >> newFile; //Guardo json

  newFile.close();
}

std::map<std::string,int> JsonSerializer::importarEnte(std::string path)
{
  std::fstream fs;
  fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app);

  json j;

  fs >> j;

  fs.close();

  std::map<std::string,int> caracteristicasEnte;

  for (json::iterator it = j.begin(); it != j.end(); ++it)
  {
    caracteristicasEnte.insert(std::pair<std::string,int>(it.key(),
                                                          it.value()));
  }

  return caracteristicasEnte;
}
