
#include "zm/client/canvas.h"
#include "zm/client/client.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <cairomm/context.h>

namespace zm {


Canvas::Canvas(Client* c) : c_(c) {
  Glib::signal_timeout().connect(
    sigc::mem_fun(*this, &Canvas::on_timeout), 40);
}

Canvas::~Canvas() {
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  // Gtk::Allocation allocation = get_allocation();
  // const int width = allocation.get_width();
  // const int height = allocation.get_height();

  int radius = 35;

  map_.draw(cr);

  cr->set_line_width(10.0);

  // Dibujo de un circulo
  cr->save();
  cr->arc(game_.x, game_.y, radius, 0.0, 2.0 * M_PI); // Un circulo
  cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // Parcialmente transparente
  cr->fill_preserve();
  cr->restore();  // Vuelvo a un negro opaco
  cr->stroke();

  return true;
}

void Canvas::updateGameState(Game game) {
  game_ = game;
  redraw();
}

void Canvas::redraw() {
  Glib::RefPtr<Gdk::Window> win = get_window();
  if (win)
  {
      Gdk::Rectangle r(0, 0, get_allocation().get_width(),
              get_allocation().get_height());
      win->invalidate_rect(r, false);
  }
}

bool Canvas::on_timeout() {
  c_->serverProxy.updateState(c_->serverProxy.getState());
  return true;
}

} // zm
