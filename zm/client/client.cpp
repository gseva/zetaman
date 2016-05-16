
#include <iostream>
#include <giomm/resource.h>
#include <glibmm/main.h>
#include <gdkmm/general.h>

#include "zm/client/client.h"

Client::Client() {
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  Window window(this);
  window.set_default_size(1024, 768);

  Area area(this);
  window.add(area);
  area.show();

  serverProxy.updateHandler.signal_game_update().connect(
      sigc::mem_fun(area, &Area::updateGameState) );

  app->run(window);
}

void Client::draw(GameState state) {
}


Window::Window(Client* c) : c_(c) {
  signal_key_release_event().connect(
    sigc::mem_fun(*this, &Window::keyReleased));
}

bool Window::keyReleased(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_space) {
    c_->serverProxy.jump();
  }
  return false;
}

Area::Area(Client* c) : c_(c) {
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &Area::on_timeout), 100);
  m_image = Gdk::Pixbuf::create_from_resource("/assets/images/grass.png");
}

Area::~Area() {
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  int radius = 35;

  Gdk::Cairo::set_source_pixbuf(cr, m_image,
   (width - m_image->get_width())/2, (height - m_image->get_height())/2);
  cr->paint();
  cr->set_line_width(10.0);

  // Dibujo de un circulo
  cr->save();
  cr->arc(gs_.x, gs_.y, radius, 0.0, 2.0 * M_PI); // Un circulo
  cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // Parcialmente transparente
  cr->fill_preserve();
  cr->restore();  // Vuelvo a un negro opaco
  cr->stroke();

  return true;
}

void Area::updateGameState(GameState gs) {
  gs_ = gs;
  redraw();
}

void Area::redraw() {
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win)
  {
      Gdk::Rectangle r(0, 0, get_allocation().get_width(),
              get_allocation().get_height());
      win->invalidate_rect(r, false);
  }
}

bool Area::on_timeout() {
  c_->serverProxy.updateState(c_->serverProxy.getState());
  return true;
}

