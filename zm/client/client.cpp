
#include <string>
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {

Client::Client() : serverProxy(*this) {
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  window_ = new Window(this);
  window_->set_default_size(1024, 768);
  app->run(*window_);
}

void Client::startConnection() {
  serverProxy.connect();
}

void Client::startGame() {
  window_->showCanvas();
}

void Client::waitForPlayers() {
  window_->showWaitingScreen();
}

void Client::selectLevel() {
  window_->showLevelSelection();
}

void Client::showConnectedPlayer(const std::string& playerName) {
  std::cout << "Connected " << playerName << std::endl;
}


} // zm
