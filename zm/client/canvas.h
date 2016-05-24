#ifndef __ZM_CLIENT_CANVAS_H__
#define __ZM_CLIENT_CANVAS_H__

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include "zm/game_protocol.h"
#include "zm/server_proxy.h"
#include "zm/client/map.h"
#include "zm/client/image_buffer.h"


namespace zm {

class Canvas : public Gtk::DrawingArea {
proto::Game game_;
Map map_;
ImageBuffer buff_;

public:
  explicit Canvas(ServerProxy& sp);
  virtual ~Canvas();

  void updateGameState(proto::Game gs);

protected:
  void redraw();

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

} // zm

#endif
