#include "jsonserializer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <string>



JsonMap JsonSerializer::importMap(std::string path)
{
  std::ifstream fs(path);

  json j;

  fs >> j; //Carga el json en memoria

  std::cout << j << std::endl;
  fs.close();

  //Carga en el vector todo lo contenido de "tiles"
  std::vector<int> imageNumbersVector = j["tiles"];
  std::vector<std::string> imageNamesVector = j["images"];
  std::vector<std::string> physicsVector = j["physics"];

  JsonMap mapa;

  mapa.imageNumbers = imageNumbersVector;
  mapa.imageNames = imageNamesVector;
  mapa.physics = physicsVector;

  return mapa;
}

void JsonSerializer::exportMap(std::string path, JsonMap m)
{
  json exportable;

  exportable= {{"tiles",m.imageNumbers},{"images",m.imageNames},
                {"physics",m.physics}};

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
