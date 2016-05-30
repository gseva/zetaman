
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {


GameFrame::GameFrame(Client& c) : c_(c), area_(c.serverProxy) {
  add(area_);
  c_.serverProxy.connect();
  c_.serverProxy.startLevel();

  c_.serverProxy.updateHandler.signal_game_update().connect(
      sigc::mem_fun(area_, &Canvas::updateGameState) );
}

GameFrame::~GameFrame() {}

void GameFrame::start() {
  area_.show();
}


LobbyFrame::LobbyFrame(Client& c) : c_(c), m_button("Start game") {
  set_border_width(10);

  m_button.signal_clicked().connect(sigc::mem_fun(*this,
              &LobbyFrame::on_button_clicked));

  add(m_button);

  m_button.show();
}

LobbyFrame::~LobbyFrame() {}

void LobbyFrame::on_button_clicked() {
  c_.startGame();
}


GameWindow::GameWindow(Client& c) : c_(c) {
  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

  GameFrame frame(c_);
  screens_.add(frame);
  screens_.set_visible_child(frame);
  add(screens_);
  screens_.show();

  set_position(Gtk::WIN_POS_CENTER);
  set_default_size(1024, 768);
  show();
  show_all_children();
}

GameWindow::~GameWindow() {};

bool GameWindow::on_key_press_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_space) {
    c_.serverProxy.jump();
  } else if (event->keyval == GDK_KEY_Right) {
    c_.serverProxy.moveRight();
  } else if (event->keyval == GDK_KEY_Left) {
    c_.serverProxy.moveLeft();
  }

  return Gtk::Window::on_key_press_event(event);
}

bool GameWindow::on_key_release_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_Right) {
    c_.serverProxy.stopHorizontalMove();
  } else if (event->keyval == GDK_KEY_Left) {
    c_.serverProxy.stopHorizontalMove();
  }

  return Gtk::Window::on_key_release_event(event);
}


Client::Client() : window_(*this) {
  // screens_.add(lobby_);
  // screens_.set_visible_child(lobby_);
  // screens_.show();
}

void Client::startLobby() {
  // lobby_.show();
}

void Client::startGame() {
  // GameFrame frame(*this);
  // frame.add(area_);
  // area_.show();
  // frame.start();
  // frame.show();
  // lobby_.hide();
  // screens_.add(frame);
  // screens_.set_visible_child(frame);
  // app_->release();
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  // Gtk::Window window_;
  app->run(window_);
}


} // zm
