#include "jsonserializer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <string>

JsonMap JsonSerializer::importMap(std::string path)
{
  std::fstream fs;

  std::map<std::string, int> tiles;

  fs.open(path, std::fstream::in | std::fstream::out | std::fstream::app);

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

  return mapa;  
}

void JsonSerializer::exportMap(std::string path, JsonMap m)
{
  json exportable;

  exportable= {{"tiles",m.imageNumbers},{"images",m.imageNames},
                {"physics",m.physics},{"spawnTypes", m.spawnTypes}};

  json exportableSpawns;

  for (unsigned int i=0; i<m.spawnsData.size(); i++)
  {
    if (exportableSpawns!=NULL)
    {
      exportableSpawns = {{{"x",m.spawnsData[i].column},
      {"y",m.spawnsData[i].row},{"type",m.spawnsData[i].type}},
       exportableSpawns};  
    } else {
      exportableSpawns = {{"x",m.spawnsData[i].column},
      {"y",m.spawnsData[i].row},{"type",m.spawnsData[i].type}};
    } 
  }

  exportable["spawns"] = exportableSpawns;

  std::cout << exportable << std::endl;

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
