#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <sigc++/sigc++.h>

#include <gtkmm/application.h>

#include "zm/game_protocol.h"
#include "zm/client/server_proxy.h"
#include "zm/client/canvas.h"
#include "zm/client/window.h"

namespace zm {

class Client {
Window* window_;
public:
  ServerProxy serverProxy;

  Client();

  void run(Glib::RefPtr<Gtk::Application> app);
  void startConnection();
  void getMap();
  void startGame();
};

} // zm


#endif
