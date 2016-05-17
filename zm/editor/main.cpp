#include <gtkmm.h>
#include <iostream>
#include "editor.h"

int main(int argc, char **argv)
{
  Editor editor;

  auto app = Gtk::Application::create(argc, argv, "");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("editor.glade");
  Gtk::Button * pBtnCrearTerreno;
  Gtk::Button * pBtnCrearEnemigo;
  Gtk::Button * pBtnCrearJugador;
  Gtk::Window * pwindow;
  Gtk::EventBox* pEventBox;
  Gtk::Image* pImage;
  Gtk::Image* pImage2;
  builder->get_widget("btnCrearTerreno", pBtnCrearTerreno);
  builder->get_widget("btnCrearEnemigo", pBtnCrearEnemigo);
  builder->get_widget("btnCrearJugador", pBtnCrearJugador);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("eventbox2", pEventBox);
  builder->get_widget("image1", pImage);
  builder->get_widget("image2",pImage2);

  pwindow->set_default_size(1024, 768);

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

  if(pEventBox)
  {
    pEventBox->set_events(Gdk::BUTTON_PRESS_MASK);
    pEventBox->signal_button_press_event().connect (sigc::mem_fun(editor,&Editor::on_eventbox_button_press));
  }

  pImage->set("vacio.jpg");

  pImage2->set("vacio.jpg");

  app->run(*pwindow);

  return 0;
}