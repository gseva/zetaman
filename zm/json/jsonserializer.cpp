#include "jsonserializer.h"
#include <iostream>
#include <fstream>

std::vector<int> JsonSerializer::importarMapa(std::string path)
{
  std::fstream fs;

  std::map<std::string, int> tiles;

  fs.open (path, std::fstream::in | std::fstream::out | std::fstream::app);

  json j;

  fs >> j; //Carga el json en memoria

  fs.close();

  std::vector<int> tilesVector = j["tiles"]; //Carga en el vector todo lo contenido de "tiles"

  return tilesVector;  
}

void JsonSerializer::exportarMapa(std::string path, std::vector<int> tiles)
{
  std::map<std::string, std::vector<int>> tilesParaGuardar;
  
  tilesParaGuardar.insert(std::pair<std::string,std::vector<int>>("tiles",tiles));  // Crea el mapa para guardarlo con el nombre "tiles" y no solo el vector

  json exportable(tilesParaGuardar); // Transformo el mapa en un json

  std::fstream newFile;

  newFile.open (path, std::fstream::in | std::fstream::out | std::fstream::app);

  exportable >> newFile; //Guardo json

  newFile.close();
}

std::map<std::string,int> JsonSerializer::importarEnte(std::string path)
{
  std::fstream fs;
  fs.open (path, std::fstream::in | std::fstream::out | std::fstream::app);

  json j;

  fs >> j;

  fs.close();

  std::map<std::string,int> caracteristicasEnte;

  for (json::iterator it = j.begin(); it != j.end(); ++it)
  {
    caracteristicasEnte.insert(std::pair<std::string,int>(it.key(), it.value()));
  }

  return caracteristicasEnte;
}
