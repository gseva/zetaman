
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {

Client::Client() {
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  window_ = new Window(this);
  window_->set_default_size(1024, 768);
  app->run(*window_);
}

void Client::startConnection() {
  serverProxy.connect();
}

void Client::getMap() {
  serverProxy.getMap();
  startGame();
}

void Client::startGame() {
  window_->showCanvas();
}


} // zm
