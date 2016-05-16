#ifndef __CLIENT_CANVAS_H__
#define __CLIENT_CANVAS_H__

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include "zm/game_protocol.h"


namespace zm {

class Client;

class Canvas : public Gtk::DrawingArea {

Client* c_;
GameState gs_;
Glib::RefPtr<Gdk::Pixbuf> m_image;

public:
  Canvas(Client* c);
  virtual ~Canvas();

  void updateGameState(GameState gs);

protected:
  void redraw();

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_timeout();
};

} // zm

#endif
