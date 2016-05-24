#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <sigc++/sigc++.h>

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

#include "zm/game_protocol.h"
#include "zm/server_proxy.h"
#include "zm/client/canvas.h"

namespace zm {

class Client {
public:
  ServerProxy serverProxy;

  Client();
  void run(Glib::RefPtr<Gtk::Application> app);

  void draw(proto::Game game);
};

class Window : public Gtk::Window {
Client* c_;
public:
  explicit Window(Client* c);

private:
  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey* event) override;
};

} // zm


#endif
