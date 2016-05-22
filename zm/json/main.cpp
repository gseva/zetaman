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

  JsonMap m = serializer.importMap("ejemplo.json");

  for (unsigned int i=0; i<m.imageNumbers.size(); i++)
  {
    std::cout << m.imageNumbers.at(i) << std::endl;
  }

  serializer.exportMap("exportando.json", m);

  /*std::map<std::string, int> caracteristicasBichito;

  caracteristicasBichito = serializer.importarEnte("ente.json");*/

  return 0;
}
