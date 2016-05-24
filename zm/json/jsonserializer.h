#ifndef __JSON_SERIALIZER_H__
#define __JSON_SERIALIZER_H__
#include <vector>
#include <string>
#include <map>
#include "json.hpp"
using json = nlohmann::json;

typedef struct JsonMap
{
  std::vector<int> imageNumbers;
  std::vector<std::string> imageNames;
  std::vector<std::string> physics;
} JsonMap;

class JsonSerializer
{
  public:
  JsonMap importMap(std::string path);
  void exportMap(std::string path, JsonMap m);
  std::map<std::string,int> importarEnte(std::string path);
};
#endif
