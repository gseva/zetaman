#include "json.hpp"
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "jsonserializer.h"
// for convenience
using json = nlohmann::json;

int main(int argc, char** argv)
{
  JsonSerializer serializer;

  std::vector<int> v = serializer.importarMapa("ejemplo.json");

  for (unsigned int i=0; i<v.size(); i++)
  {
    std::cout << v.at(i) << std::endl;
  }
  
  serializer.exportarMapa("conClase.json", v);

  std::map<std::string, int> caracteristicasBichito;

  caracteristicasBichito = serializer.importarEnte("ente.json");

  return 0;
}
