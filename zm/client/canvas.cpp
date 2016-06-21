
#include "zm/client/canvas.h"
#include "zm/client/client.h"
#include "zm/client/drawable.h"

#include <glibmm/main.h>
#include <glibmm/fileutils.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {


Canvas::Canvas(Client& c) : c_(c), gameSet_(false), map_(c), buff_(c),
    statusBar_(c) {
  std::cout << "Creo canvas" << std::endl;
  sigc::connection conn = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &Canvas::on_timeout), 1000/30);
  std::cout << "Crea2" << std::endl;
}

Canvas::~Canvas() {
  for (auto&& player : players_) {
    delete player.second;
  }
  for (auto&& enemy : enemies_) {
    delete enemy.second;
  }
  for (auto&& proyectile : proyectiles_) {
    delete proyectile.second;
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
  for (auto&& proyectile : game_.proyectiles) {
    drawing::Proyectile* drawable;
    if (!proyectiles_.count(proyectile.id)) {
      drawable = new drawing::Proyectile(c_);
      proyectiles_.insert({proyectile.id, drawable});
    } else {
      drawable = proyectiles_.at(proyectile.id);
    }
    drawable->setState(proyectile);
    drawable->draw(cr, buff_);
  }
  for (auto&& powerUp : game_.powerUps) {
    drawing::PowerUp drawable(c_);
    drawable.setState(powerUp);
    drawable.draw(cr, buff_);
  }

  for (auto&& player : game_.players) {
    if (player.id == game_.playerId) {
      statusBar_.draw(cr, buff_, player);
      break;
    }
  }

  if (winScreen_) {
    cr->set_source_rgba(.5, 1.0, .5, 1.0 - (tics / 100.));
    cr->rectangle(0, 0, c_.width, c_.height + c_.statusBarHeight);
    cr->fill();
    tics--;
    if (tics < 0) tics = 1;
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


void Canvas::setWinScreen() {
  winScreen_ = true;
  tics = 100;
}

void Canvas::setLoseScreen() {
  loseScreen_ = true;
  tics = 100;
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
