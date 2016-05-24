
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


Enemy::Enemy(zm::Enemy e) : e_(e), imageName_("enemies/bumpy/1.png") {
}

std::string& Enemy::getImageName() {
  return imageName_;
}

zm::Position& Enemy::getPosition() {
  return e_.pos;
}


Proyectile::Proyectile(zm::Proyectile p) : p_(p),
    imageName_("proyectiles/normal.png") {
}

std::string& Proyectile::getImageName() {
  return imageName_;
}

zm::Position& Proyectile::getPosition() {
  return p_.pos;
}


PowerUp::PowerUp(zm::PowerUp p) : p_(p), imageName_("powerups/life.png") {
}

std::string& PowerUp::getImageName() {
  return imageName_;
}

zm::Position& PowerUp::getPosition() {
  return p_.pos;
}



} // draw
} // zm
