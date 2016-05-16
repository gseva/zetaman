
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

} // zm
