
#include "zm/client/canvas.h"
#include "zm/client/drawable.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {


Canvas::Canvas(Client& c) : c_(c), gameSet_(false), map_(c), buff_(c) {
  std::cout << "Creo canvas" << std::endl;
  sigc::connection conn = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &Canvas::on_timeout), 1000/30);
}

Canvas::~Canvas() {
  for (auto&& player : players_) {
    delete player.second;
  }
  for (auto&& enemy : enemies_) {
    delete enemy.second;
  }
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  // Gtk::Allocation allocation = get_allocation();
  // const int width = allocation.get_width();
  // const int height = allocation.get_height();

  // int radius = 35;
  if (!gameSet_) return true;

  map_.draw(cr, game_.camPos.x, game_.camPos.y);

  for (auto&& player : game_.players) {
    drawing::Player* drawable;
    if (!players_.count(player.id)) {
      drawable = new drawing::Player(c_);
      players_.insert({player.id, drawable});
    } else {
      drawable = players_.at(player.id);
    }
    drawable->setState(player);
    drawable->draw(cr, buff_);
  }
  for (auto&& enemy : game_.enemies) {
    drawing::Enemy* drawable;
    if (!enemies_.count(enemy.id)) {
      drawable = newEnemy_(enemy);
    } else {
      drawable = enemies_.at(enemy.id);
    }
    drawable->setState(enemy);
    drawable->draw(cr, buff_);
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

drawing::Enemy* Canvas::newEnemy_(const proto::Enemy& e) {
  drawing::Enemy* enemy;
  switch (e.enemyType) {
    case proto::Met:
      enemy = new drawing::Met(c_); break;
    case proto::Bumby:
      enemy = new drawing::Bumby(c_); break;
    case proto::Sniper:
    case proto::JumpingSniper:
      enemy = new drawing::Sniper(c_); break;
    case proto::Bombman:
      enemy = new drawing::Bombman(c_); break;
    case proto::Fireman:
      enemy = new drawing::Fireman(c_); break;
    case proto::Magnetman:
      enemy = new drawing::Magnetman(c_); break;
    case proto::Ringman:
      enemy = new drawing::Ringman(c_); break;
    case proto::Sparkman:
      enemy = new drawing::Sparkman(c_); break;
    default:
      enemy = new drawing::Enemy(c_); break;
  }
  enemies_.insert({e.id, enemy});
  return enemy;
}

void Canvas::updateGameState(proto::Game game) {
  // Lock l(m_);
  game_ = game;
  gameSet_ = true;
  // redraw();
}

bool Canvas::on_timeout() {
  redraw();
  return true;
}

void Canvas::redraw() {
  queue_draw();
}

} // zm
