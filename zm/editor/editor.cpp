#include "editor.h"
#include <iostream>

#define IMAGEN_JUGADOR "/zm/editor/images/player.png"
#define IMAGEN_TERRENO "/zm/editor/images/grass_mid.png"
#define IMAGEN_ENEMIGO "/zm/editor/images/enemy.png"

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

bool Editor::on_eventbox_button_press(GdkEventButton* eventButton,
                                      Gtk::Image* imagen)
{
  imagen->set_from_resource(imagenSeleccionada);
  std::cout << "Clickeo en division" << std::endl;
  return true;
}

Editor::Editor(int argc, char **argv)
{
  app = Gtk::Application::create(argc, argv, "");
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_resource("/zm/editor/editor.glade");

  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearEnemigo", pBtnCrearEnemigo);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("grid1", pGrid);

  pwindow->set_default_size(1024, 768);

  connectButtonsWithSignals();

  createEmptyGrid();
}

void Editor::connectButtonsWithSignals()
{
  if (pBtnCrearJugador)
  {
    pBtnCrearJugador->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearJugador_clicked));
  }

  if (pBtnCrearTerreno)
  {
    pBtnCrearTerreno->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearTerreno_clicked));
  }

  if (pBtnCrearEnemigo)
  {
    pBtnCrearEnemigo->signal_clicked().connect(
        sigc::mem_fun(this,&Editor::on_buttonCrearEnemigo_clicked));
  }
}

void Editor::createEmptyGrid()
{
  /*Agrego los event box a la grid*/
  for (int i = 0; i < ALTO; i++)
  {
      for (int j = 0; j < ANCHO; j++)
      {
          pGrid->attach(eventBoxMatrix[i][j], i, j, 1, 1);
      }
  }

  /*Agrego las imagenes a los event box*/
  for (int i = 0; i < ALTO; i++)
  {
      for (int j = 0; j < ANCHO; j++)
      {
          eventBoxMatrix[i][j].add(imageMatrix[i][j]);
      }
  }

  /*Seteo las imagenes*/
  for (int i = 0; i < ALTO; i++)
  {
      for (int j = 0; j < ANCHO; j++)
      {
          imageMatrix[i][j].set_from_resource("/zm/editor/images/void.png");
      }
  }

  /*Seteo el evento en los event box*/
  for (int i=0; i<ALTO; i++)
  {
    for (int j=0; j<ANCHO; j++)
    {
      eventBoxMatrix[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
      eventBoxMatrix[i][j].signal_button_press_event().connect(
        sigc::bind<Gtk::Image*>(sigc::mem_fun(
            this,&Editor::on_eventbox_button_press), &imageMatrix[i][j]));
    }
  }
}

void Editor::runEditor()
{
  pGrid->show_all_children();

  app->run(*pwindow);
}
