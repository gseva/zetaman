
#include <string>
#include <glibmm/main.h>

#include "zm/config.h"
#include "zm/client/client.h"


namespace zm {

Client::Client() : connected_(false), width(zm::config::screenWidth),
    height(width * 3 / 4), statusBarHeight(64), serverProxy(*this) {
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
  if (!connected_) {
    connected_ = true;
    serverProxy.connect();
  }
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

void Client::showWinAnimation() {
  window_->showWinAnimation();
}
void Client::showLoseAnimation() {
  window_->showLoseAnimation();
}

void Client::showWinDialog() {
  window_->showWaitingScreen();
  // window_->showWinDialog();
}



} // zm
