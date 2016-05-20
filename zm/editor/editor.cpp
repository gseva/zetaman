#include "editor.h"
#include <iostream>

#define IMAGEN_JUGADOR "jugador.png"
#define IMAGEN_TERRENO "terreno.png"
#define IMAGEN_ENEMIGO "enemigo.png"

void Editor::on_buttonCrearJugador_clicked()
{
  std::cout << "Crear jugador seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_JUGADOR;
}

void Editor::on_buttonCrearTerreno_clicked()
{
  std::cout << "Crear terreno seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_TERRENO;
}

void Editor::on_buttonCrearEnemigo_clicked()
{
  std::cout << "Crear enemigo seleccionado" << std::endl;
  imagenSeleccionada = IMAGEN_ENEMIGO;
}

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton, Gtk::Image* imagen)
{
  imagen->set(imagenSeleccionada);
  std::cout << "Clickeo en division" << std::endl;
  return true;
}