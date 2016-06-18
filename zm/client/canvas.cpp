
#include "zm/client/canvas.h"
#include "zm/client/drawable.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {


Canvas::Canvas(Client& c) : c_(c), map_(c), buff_(c) {
  std::cout << "Creo canvas" << std::endl;
  sigc::connection conn = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &Canvas::on_timeout), 1000/30);
}

Canvas::~Canvas() {
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  // Gtk::Allocation allocation = get_allocation();
  // const int width = allocation.get_width();
  // const int height = allocation.get_height();

  // int radius = 35;

  map_.draw(cr, game_.camPos.x, game_.camPos.y);


  for (auto&& player : game_.players) {
    drawing::Player drawable(c_, player);
    drawable.draw(cr, buff_);
  }
  for (auto&& enemy : game_.enemies) {
    drawing::Enemy drawable(c_, enemy);
    drawable.draw(cr, buff_);
  }
  for (auto&& proy : game_.proyectiles) {
    drawing::Proyectile drawable(c_, proy);
    drawable.draw(cr, buff_);
  }
  for (auto&& powerUp : game_.powerUps) {
    drawing::PowerUp drawable(c_, powerUp);
    drawable.draw(cr, buff_);
  }

  return true;
}

void Canvas::updateGameState(proto::Game game) {
  Lock l(m_);
  game_ = game;
  // redraw();
}

bool Canvas::on_expose_event(GdkEventExpose* event) {
  std::cout << "Expose!" << std::endl;
  return on_expose_event(event);
}

bool Canvas::on_timeout() {
  redraw();
  return true;
}

void Canvas::redraw() {
  queue_draw();
}

} // zm
