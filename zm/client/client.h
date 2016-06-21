#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <string>

#include <gtkmm/application.h>

#include "zm/game_protocol.h"
#include "zm/client/server_proxy.h"
#include "zm/client/canvas.h"
#include "zm/client/window.h"

namespace zm {

class Client {
Window* window_;
bool connected_;

public:
  int width, height, statusBarHeight, ppm;
  ServerProxy serverProxy;

  Client();

  int scaleNum(int n);
  int scaleWidth(float w);
  int scaleHeight(float h);

  void run(Glib::RefPtr<Gtk::Application> app);
  void startConnection();
  void startGame();

  void waitForPlayers();
  void selectLevel();

  void showConnectedPlayer(const std::string& playerName);
  void showWinDialog();
  void showWinAnimation();
  void showLoseAnimation();
};

} // zm


#endif
