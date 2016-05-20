#include <gtkmm.h>
#include <iostream>
#include "editor.h"

#define ALTO 12
#define ANCHO 9

int main(int argc, char **argv)
{
  Editor editor;

  auto app = Gtk::Application::create(argc, argv, "");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("editor.glade");
  Gtk::Button * pBtnCrearTerreno;
  Gtk::Button * pBtnCrearEnemigo;
  Gtk::Button * pBtnCrearJugador;
  Gtk::Window * pwindow;
  Gtk::Grid* pGrid;

  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearEnemigo", pBtnCrearEnemigo);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("grid1", pGrid);

  pwindow->set_default_size(1024, 768);

  Gtk::EventBox eventBoxMatrix[ALTO][ANCHO];
  Gtk::Image imageMatrix[ALTO][ANCHO];

  if(pBtnCrearJugador)
  {
    pBtnCrearJugador->signal_clicked().connect( sigc::mem_fun(editor,&Editor::on_buttonCrearJugador_clicked) );
  }

  if(pBtnCrearTerreno)
  {
    pBtnCrearTerreno->signal_clicked().connect( sigc::mem_fun(editor,&Editor::on_buttonCrearTerreno_clicked) );
  }

  if(pBtnCrearEnemigo)
  {
    pBtnCrearEnemigo->signal_clicked().connect( sigc::mem_fun(editor,&Editor::on_buttonCrearEnemigo_clicked) );
  }

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
          imageMatrix[i][j].set("vacio.png");
      }
  }

  /*Seteo el evento en los event box*/
  for(int i=0; i<ALTO; i++)
  {
    for(int j=0; j<ANCHO; j++)
    {
      eventBoxMatrix[i][j].set_events(Gdk::BUTTON_PRESS_MASK);
      eventBoxMatrix[i][j].signal_button_press_event().connect (sigc::bind<Gtk::Image*>(sigc::mem_fun(editor,&Editor::on_eventbox_button_press), &imageMatrix[i][j]));
    }
  }
 
  pGrid->show_all_children();

  app->run(*pwindow);

  return 0;
}