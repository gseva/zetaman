
#include <string>

#include <gdkmm/pixbuf.h>
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"


namespace zm {
namespace drawing {

void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();

  Glib::RefPtr<Gdk::Pixbuf> image = buff.getImage(getImageName());

  zm::Position pos = getPosition();

  Gdk::Cairo::set_source_pixbuf(context, image, pos.x, pos.y);
  context->paint();

  context->restore();
}


Player::Player(zm::Player p) : p_(p), imageName_("player/megaman.png") {
}

std::string& Player::getImageName() {
  return imageName_;
}

zm::Position& Player::getPosition() {
  return p_.pos;
}


} // draw
} // zm
