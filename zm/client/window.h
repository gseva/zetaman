
#ifndef __ZM_CLIENT_WINDOW_H__
#define __ZM_CLIENT_WINDOW_H__

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/window.h>

#include "zm/client/canvas.h"

namespace zm {

class Client;

class LevelSelectionPanel : public Gtk::Grid {
Gtk::Button levelButtons[5];
Client* c_;
public:
  LevelSelectionPanel(Client* c);

private:
  void on_button_clicked(int i);
};

class WaitingScreen : public Gtk::Box {
Gtk::Label label_;
public:
  WaitingScreen();
};

class Window : public Gtk::Window {
Widget* selectedWidget_;
Client* c_;
Canvas* canvas_;
WaitingScreen* waitingScreen_;
LevelSelectionPanel* panel_;
Gtk::Box box_;
Gtk::Button startButton_;

public:
  explicit Window(Client* c);

  void showLevelSelection();
  void showWaitingScreen();
  void showCanvas();
private:

  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey* event) override;

  void on_startButton_clicked();
};

} // zm

#endif
