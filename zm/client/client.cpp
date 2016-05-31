
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {

Client::Client() {
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  Window window(this);
  window.set_default_size(1024, 768);
  app->run(window);
}

void Client::startGame() {
  serverProxy.connect();
}



Window::Window(Client* c) : c_(c), startButton_("Empezar!") {
  add(box_);

  // signal_key_press_event().connect(
  //   sigc::mem_fun(*this, &Window::keyReleased));
  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);

  startButton_.signal_clicked().connect(
        sigc::mem_fun(this, &Window::on_startButton_clicked));

  box_.pack_start(startButton_);
  startButton_.show();
  box_.show();
}


bool Window::on_key_press_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_space) {
    c_->serverProxy.jump();
  } else if (event->keyval == GDK_KEY_Right) {
    c_->serverProxy.moveRight();
  } else if (event->keyval == GDK_KEY_Left) {
    c_->serverProxy.moveLeft();
  } else if (event->keyval == GDK_KEY_Up) {
    c_->serverProxy.up();
  } else if ((event->keyval == GDK_KEY_A) || (event->keyval == GDK_KEY_a)) {
    c_->serverProxy.shoot();
  }

  return Gtk::Window::on_key_press_event(event);
}

bool Window::on_key_release_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_Right) {
    c_->serverProxy.stopHorizontalMove();
  } else if (event->keyval == GDK_KEY_Left) {
    c_->serverProxy.stopHorizontalMove();
  }

  return Gtk::Window::on_key_release_event(event);
}

void Window::on_startButton_clicked() {
  box_.remove(startButton_);
  c_->startGame();
  canvas_ = Gtk::manage(new Canvas(c_->serverProxy));

  c_->serverProxy.updateHandler.signal_game_update().connect(
          sigc::mem_fun(*canvas_, &Canvas::updateGameState) );

  box_.pack_start(*canvas_);
  canvas_->show();
  c_->serverProxy.startLevel();
}

} // zm
