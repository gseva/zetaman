#include <vector>
#include <string>
#include <map>
#include "json.hpp"
using json = nlohmann::json;

typedef struct Map
{
  std::vector<int> imageNumbers;
  std::vector<std::string> imageNames;
  std::vector<std::string> physics;
} Map;

class JsonSerializer
{
  public:
  Map importMap(std::string path);
  void exportMap(std::string path, Map m);
  std::map<std::string,int> importarEnte(std::string path);
};
