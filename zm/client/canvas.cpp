
#include "zm/client/canvas.h"
#include "zm/client/drawable.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {


Canvas::Canvas(ServerProxy& sp) : map_(sp.getImages(), sp.getImageNames()) {
  std::cout << "Creo canvas" << std::endl;
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
    drawing::Player dp(player);
    dp.draw(cr, buff_);
  }
  for (auto&& enemy : game_.enemies) {
    drawing::Enemy dp(enemy);
    dp.draw(cr, buff_);
  }
  for (auto&& proy : game_.proyectiles) {
    drawing::Proyectile dp(proy);
    dp.draw(cr, buff_);
  }
  for (auto&& powerUp : game_.powerUps) {
    drawing::PowerUp dp(powerUp);
    dp.draw(cr, buff_);
  }

  return true;
}

void Canvas::updateGameState(proto::Game game) {
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

} // zm
