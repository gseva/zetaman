
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {

Client::Client() {
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  Window window(this);
  window.set_default_size(1024, 768);

  Canvas area(this);
  window.add(area);
  area.show();

  serverProxy.updateHandler.signal_game_update().connect(
      sigc::mem_fun(area, &Canvas::updateGameState) );

  app->run(window);
}

void Client::draw(Game game) {
}


Window::Window(Client* c) : c_(c) {
  // signal_key_press_event().connect(
  //   sigc::mem_fun(*this, &Window::keyReleased));
  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
}


bool Window::on_key_press_event(GdkEventKey* event) {
  if (event->keyval == GDK_KEY_space) {
    c_->serverProxy.jump();
  } else if (event->keyval == GDK_KEY_Right) {
    c_->serverProxy.moveRight();
  } else if (event->keyval == GDK_KEY_Left) {
    c_->serverProxy.moveLeft();
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

} // zm
