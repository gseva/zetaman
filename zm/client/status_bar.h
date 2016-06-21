
#ifndef __ZM_CLIENT_STATUS_BAR__
#define __ZM_CLIENT_STATUS_BAR__

#include <cairomm/context.h>

#include "zm/game_protocol.h"
#include "zm/client/image_buffer.h"

namespace zm {

class Client;

class StatusBar {
Client& client_;
public:
  explicit StatusBar(Client& c);
  void draw(const Cairo::RefPtr<Cairo::Context>& context,
            ImageBuffer& buff, const proto::Player& player);
private:
  void setImage(const Cairo::RefPtr<Cairo::Context>& context,
                Glib::RefPtr<Gdk::Pixbuf> image, int width, int heigth);
  void drawFilledCircle(const Cairo::RefPtr<Cairo::Context>& context,
                int width, int height, float percentage, int radius);
};

} // zm

#endif
