#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

#include "zm/game_protocol.h"
#include "zm/server_proxy.h"


class Client {

public:
  ServerProxy serverProxy;

  Client();
  void run(Glib::RefPtr<Gtk::Application> app);

  void draw(GameState state);

};

class Window : public Gtk::Window {

Client* c_;
public:
  Window(Client* c);

private:
  bool keyReleased(GdkEventKey* event);

};

class Area : public Gtk::DrawingArea {

Client* c_;

public:
  Area(Client* c);
  virtual ~Area();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_timeout();
};


#endif
