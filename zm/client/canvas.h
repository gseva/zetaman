#ifndef __CLIENT_CANVAS_H__
#define __CLIENT_CANVAS_H__

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include "zm/game_protocol.h"
#include "zm/server_proxy.h"
#include "zm/client/map.h"


namespace zm {

class Client;

class Canvas : public Gtk::DrawingArea {
Game game_;
Map map_;

public:
  explicit Canvas(ServerProxy& sp);
  virtual ~Canvas();

  void updateGameState(Game gs);

protected:
  void redraw();

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

} // zm

#endif
