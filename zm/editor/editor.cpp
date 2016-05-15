#include "editor.h"
#include <iostream>

void Editor::on_buttonCrearJugador_clicked()
{
  std::cout << "Crear jugador seleccionado" << std::endl;
  bool crearJugador = true;
  bool crearTerreno = false;
  bool crearEnemigo = false;
}

void Editor::on_buttonCrearTerreno_clicked()
{
  std::cout << "Crear terreno seleccionado" << std::endl;
  bool crearJugador = false;
  bool crearTerreno = true;
  bool crearEnemigo = false;
}

void Editor::on_buttonCrearEnemigo_clicked()
{
  std::cout << "Crear enemigo seleccionado" << std::endl;
  bool crearJugador = false;
  bool crearTerreno = false;
  bool crearEnemigo = true;
}