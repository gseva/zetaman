
#include "zm/client/canvas.h"
#include "zm/client/drawable.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {


Canvas::Canvas(ServerProxy& sp) : map_(sp.getImages(), sp.getImageNames()) {
}

Canvas::~Canvas() {
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  // Gtk::Allocation allocation = get_allocation();
  // const int width = allocation.get_width();
  // const int height = allocation.get_height();

  // int radius = 35;

  map_.draw(cr);

  proto::Player p;
  p.pos.x = game_.x;
  p.pos.y = game_.y;
  game_.players.push_back(p);

  proto::Enemy e;
  e.pos.x = game_.x + 35;
  e.pos.y = game_.y + 35;
  game_.enemies.push_back(e);

  proto::Proyectile pr;
  pr.pos.x = game_.x - 35;
  pr.pos.y = game_.y - 35;
  game_.proyectiles.push_back(pr);

  proto::PowerUp pu;
  pu.pos.x = game_.x - 55;
  pu.pos.y = game_.y - 55;
  game_.powerUps.push_back(pu);


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

  // cr->set_line_width(10.0);

  // Dibujo de un circulo
  // cr->save();
  // cr->arc(game_.x, game_.y, radius, 0.0, 2.0 * M_PI); // Un circulo
  // cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // Parcialmente transparente
  // cr->fill_preserve();
  // cr->restore();  // Vuelvo a un negro opaco
  // cr->stroke();

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
