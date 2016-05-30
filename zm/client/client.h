#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <sigc++/sigc++.h>

#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/stack.h>
#include <gtkmm/window.h>

#include "zm/game_protocol.h"
#include "zm/server_proxy.h"
#include "zm/client/canvas.h"

namespace zm {

class Client;

class LobbyFrame : public Gtk::Frame {
Client& c_;
public:
  explicit LobbyFrame(Client& c);
  ~LobbyFrame();

protected:
  void on_button_clicked();

  Gtk::Button m_button;
};


class GameFrame : public Gtk::Frame {
Client& c_;
Canvas area_;
public:
  explicit GameFrame(Client& c);
  ~GameFrame();

  void start();
};


class GameWindow : public Gtk::Window {
public:
  explicit GameWindow(Client& c);
  ~GameWindow();

  ServerProxy getProxy();

private:
  Gtk::Stack screens_;
  Client& c_;

  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey* event) override;
};


class Client {

// LobbyFrame lobby_;
// GameFrame game_;
Glib::RefPtr<Gtk::Application> app_;
GameWindow window_;
public:
  ServerProxy serverProxy;

  Client();
  void run(Glib::RefPtr<Gtk::Application> app);

  void startLobby();
  void startGame();
};

} // zm


#endif
