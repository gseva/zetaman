#ifndef __ZM_CLIENT_CANVAS_H__
#define __ZM_CLIENT_CANVAS_H__

#include <map>

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

#include "zm/game_protocol.h"
#include "zm/thread.h"
#include "zm/client/drawable.h"
#include "zm/client/image_buffer.h"
#include "zm/client/map.h"
#include "zm/client/server_proxy.h"


namespace zm {

class Canvas : public Gtk::DrawingArea {
Client& c_;
proto::Game game_;
Map map_;
ImageBuffer buff_;
Mutex m_;
std::map<int, drawing::Player*> players_;
std::map<int, drawing::Enemy*> enemies_;

public:
  explicit Canvas(Client& c);
  virtual ~Canvas();

  void updateGameState(proto::Game gs);

protected:
  void redraw();

  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

  bool on_expose_event(GdkEventExpose* event);

  bool on_timeout();

private:
  drawing::Enemy* newEnemy_(const proto::Enemy& e);
};

} // zm

#endif
