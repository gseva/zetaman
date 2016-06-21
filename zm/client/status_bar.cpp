
#include <gdkmm/cursor.h> // Necesario para que no tire el famoso error
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/status_bar.h"
#include "zm/client/client.h"

namespace zm {

StatusBar::StatusBar(Client& c) : client_(c) {
}

void StatusBar::draw(const Cairo::RefPtr<Cairo::Context>& context,
                     ImageBuffer& buff, const proto::Player& player) {
  context->save();

  int step = client_.width / 12;
  int maxHeight = client_.statusBarHeight;
  context->set_line_width(maxHeight * 0.04);
  context->save();

  context->set_source_rgba(0.7, 0.7, 0.7, 0.9);
  context->rectangle(0, 0, client_.width, maxHeight);
  context->fill();
  context->restore();

  int weaponOffset = static_cast<int>(player.weapon);
  context->set_source_rgba(1.0, 1.0, 1.0, 0.8);
  int circleRadius = client_.scaleNum(30);
  context->arc(step * (weaponOffset + 1), maxHeight / 2,
               circleRadius, 0, 2 * M_PI);
  context->fill();
  context->restore();

  auto image = buff.getImage("proyectiles/normal.png", false, 2, 2);
  setImage(context, image, step * 1, maxHeight / 2);
  context->paint();

  image = buff.getImage("proyectiles/bomb_1.png", false, 1.5, 1.5);
  setImage(context, image, step * 2, maxHeight / 2);
  context->paint();

  image = buff.getImage("proyectiles/magnet_1.png", false, 1.2, 1.2);
  setImage(context, image, step * 3, maxHeight / 2 + 5);
  context->paint();

  image = buff.getImage("proyectiles/spark_1.png", false, 1.5, 1.5);
  setImage(context, image, step * 4, maxHeight / 2);
  context->paint();

  image = buff.getImage("proyectiles/ring_1.png", false, 1.4, 1.4);
  setImage(context, image, step * 5, maxHeight / 2);
  context->paint();

  image = buff.getImage("proyectiles/fire_1.png");
  setImage(context, image, step * 6, maxHeight / 2 + 10);
  context->paint();

  context->restore();
}

void StatusBar::setImage(const Cairo::RefPtr<Cairo::Context>& context,
                         Glib::RefPtr<Gdk::Pixbuf> image,
                         int width, int heigth) {
  int imageWidth = image->get_width();
  int imageHeight = image->get_width();
  width = width - imageWidth / 2;
  heigth = heigth - imageHeight / 2;
  Gdk::Cairo::set_source_pixbuf(context, image, width, heigth);

}

} // zm
