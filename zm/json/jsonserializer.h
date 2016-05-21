#include <vector>
#include <string>
#include <map>
#include "json.hpp"
using json = nlohmann::json;


class JsonSerializer
{
  public:
  std::vector<int> importarMapa(std::string path);
  void exportarMapa(std::string path, std::vector<int> tiles);
  std::map<std::string,int> importarEnte(std::string path);
};
