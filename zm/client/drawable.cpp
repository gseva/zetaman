
#include <string>
#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"

// pixels per meter
#define PPM 64

// Estos offsets hacen falta para que todo se dibuje mas lindo, no se porque
#define X_OFFSET 0
#define Y_OFFSET 0

namespace zm {
namespace drawing {

proto::Position posToScale(const proto::Position& from) {
  proto::Position result;
  result.x = from.x * PPM + X_OFFSET;
  result.y = from.y * -PPM + 768 + Y_OFFSET;
  return result;
}


void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();

  Glib::RefPtr<Gdk::Pixbuf> image = buff.getImage(getImageName());

  // int width = image->get_width();
  // int height = image->get_height();

  proto::Position pos = posToScale(getPosition());

  // Gdk::Cairo::set_source_pixbuf(context, image, pos.x + width / 2,
  //                               pos.y - height / 2);
  Gdk::Cairo::set_source_pixbuf(context, image, pos.x, pos.y);
  context->paint();

  // context->set_source_rgb(0.8, 0.0, 0.0);
  // context->move_to(pos.x, pos.y);
  // context->line_to(pos.x + width / 2, pos.y - height / 2);
  // context->line_to(pos.x + width / 2, pos.y + height / 2);
  // context->line_to(pos.x - width / 2, pos.y + height / 2);
  // context->line_to(pos.x - width / 2, pos.y - height / 2);
  // context->line_to(pos.x + width / 2, pos.y - height / 2);
  // context->stroke();

  context->restore();
}


Player::Player(proto::Player p) : p_(p), imageName_("player/megaman.png") {
}

std::string& Player::getImageName() {
  return imageName_;
}

proto::Position& Player::getPosition() {
  return p_.pos;
}


Enemy::Enemy(proto::Enemy e) : e_(e) {
  if ( e.enemyType == proto::EnemyType::Met ){
    if ( e.enemyState == proto::EnemyState::guarded )
      imageName_ = "enemies/met/guarded.png";
    else
      imageName_ = "enemies/met/unguarded.png";
  } else if ( e.enemyType == proto::EnemyType::Bumby ) {
    imageName_ = "enemies/bumpy/1.png";
  }
}

std::string& Enemy::getImageName() {
  return imageName_;
}

proto::Position& Enemy::getPosition() {
  return e_.pos;
}


Proyectile::Proyectile(proto::Proyectile p) : p_(p),
    imageName_("proyectiles/normal.png") {
}

std::string& Proyectile::getImageName() {
  return imageName_;
}

proto::Position& Proyectile::getPosition() {
  return p_.pos;
}


PowerUp::PowerUp(proto::PowerUp p) : p_(p),
  imageName_("powerups/life.png") {
}

std::string& PowerUp::getImageName() {
  return imageName_;
}

proto::Position& PowerUp::getPosition() {
  return p_.pos;
}



} // draw
} // zm
