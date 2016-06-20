
#include <string>
#include <glibmm/main.h>

#include "zm/client/client.h"


namespace zm {

Client::Client(const char* port) : port_(port), width(800), height(600),
    serverProxy(*this, port_) {
  ppm = scaleNum(64);
}

int Client::scaleNum(int n) {
  return width * n / 1024;
}

int Client::scaleWidth(float w) {
  return w * ppm;
}

int Client::scaleHeight(float h) {
  return h * -ppm + height;
}

void Client::run(Glib::RefPtr<Gtk::Application> app) {
  window_ = new Window(this);
  // window_->set_default_size(1024, 768);
  app->run(*window_);
}

void Client::startConnection() {
  serverProxy.connect();
}

void Client::startGame() {
  window_->showCanvas();
  serverProxy.startPlaying();
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

void Client::showWinDialog() {
  window_->showWaitingScreen();
  // window_->showWinDialog();
}


} // zm
