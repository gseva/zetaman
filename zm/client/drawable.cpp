
#include <string>
#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"


namespace zm {
namespace drawing {

void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();

  Glib::RefPtr<Gdk::Pixbuf> image = buff.getImage(getImageName());

  int width = image->get_width();
  int height = image->get_height();

  zm::proto::Position pos = getPosition();
  std::cout << "dibujando " << pos.x << "x" << pos.y << std::endl;

  Gdk::Cairo::set_source_pixbuf(context, image, pos.x + width / 2,
                                pos.y - height / 2);
  context->paint();

  context->restore();
}


Player::Player(zm::proto::Player p) : p_(p), imageName_("player/megaman.png") {
}

std::string& Player::getImageName() {
  return imageName_;
}

zm::proto::Position& Player::getPosition() {
  return p_.pos;
}


Enemy::Enemy(zm::proto::Enemy e) : e_(e), imageName_("enemies/bumpy/1.png") {
}

std::string& Enemy::getImageName() {
  return imageName_;
}

zm::proto::Position& Enemy::getPosition() {
  return e_.pos;
}


Proyectile::Proyectile(zm::proto::Proyectile p) : p_(p),
    imageName_("proyectiles/normal.png") {
}

std::string& Proyectile::getImageName() {
  return imageName_;
}

zm::proto::Position& Proyectile::getPosition() {
  return p_.pos;
}


PowerUp::PowerUp(zm::proto::PowerUp p) : p_(p),
  imageName_("powerups/life.png") {
}

std::string& PowerUp::getImageName() {
  return imageName_;
}

zm::proto::Position& PowerUp::getPosition() {
  return p_.pos;
}



} // draw
} // zm
