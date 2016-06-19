
#include <string>
#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gdkmm/cursor.h> // Necesario para que no tire el famoso error
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"
#include "zm/client/client.h"


namespace zm {
namespace drawing {


Drawable::Drawable(Client& c) : c_(c) {
}

void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();

  Glib::RefPtr<Gdk::Pixbuf> image = buff.getImage(getImageName());

  // int width = image->get_width();
  // int height = image->get_height();
  proto::Position pos = getPosition();
  pos.x = c_.scaleWidth(pos.x);
  pos.y = c_.scaleHeight(pos.y);

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


Player::Player(Client& c, proto::Player p) : Drawable(c), p_(p),
    imageName_("player/megaman.png") {
}

std::string& Player::getImageName() {
  return imageName_;
}

proto::Position& Player::getPosition() {
  return p_.pos;
}


Enemy::Enemy(Client& c, proto::Enemy e) : Drawable(c), e_(e) {
  if ( e.enemyType == proto::EnemyType::Met ){
    if ( e.enemyState == proto::EnemyState::guarded )
      imageName_ = "enemies/met/guarded.png";
    else
      imageName_ = "enemies/met/unguarded.png";
  } else if ( e.enemyType == proto::EnemyType::Bumby ) {
    imageName_ = "enemies/bumpy/1.png";
  } else if ( e.enemyType == proto::EnemyType::Sniper ){
    switch ( e.enemyState ) {
      case proto::EnemyState::guarded:
        imageName_ = "enemies/sniper/guarded.png";
        break;
      case proto::EnemyState::jumping:
        imageName_ = "enemies/sniper/jumping.png";
        break;
      case proto::EnemyState::unguarded:
        imageName_ = "enemies/sniper/unguarded.png";
        break;
      default: break;
    }
  } else if ( e.enemyType == proto::EnemyType::Fireman ) {
    imageName_ = "enemies/fireman/charmeleon.png";
  } else if ( e.enemyType == proto::EnemyType::Magnetman ) {
    imageName_ = "enemies/magnetman/magneton.png";
  } else if ( e.enemyType == proto::EnemyType::Sparkman ) {
    imageName_ = "enemies/sparkman/voltorb.png";
  } else if ( e.enemyType == proto::EnemyType::Ringman ) {
    imageName_ = "enemies/ringman/mewtwo.png";
  } else if ( e.enemyType == proto::EnemyType::Bombman ) {
    imageName_ = "enemies/bombman/golem.png";
  }
}

std::string& Enemy::getImageName() {
  return imageName_;
}

proto::Position& Enemy::getPosition() {
  return e_.pos;
}

Proyectile::Proyectile(Client& c, proto::Proyectile p) : Drawable(c), p_(p) {
  if ( p.type == proto::ProyectileType::Normal )
    imageName_ = "proyectiles/normal.png";
  else if ( p.type == proto::ProyectileType::Bomb )
    imageName_ = "proyectiles/bomb.png";
  else if ( p.type == proto::ProyectileType::Spark )
    imageName_ = "proyectiles/spark.png";
  else if ( p.type == proto::ProyectileType::Magnet )
    imageName_ = "proyectiles/magnet.png";
  else if ( p.type == proto::ProyectileType::Ring )
    imageName_ = "proyectiles/ring.png";
  else if ( p.type == proto::ProyectileType::Fire )
    imageName_ = "proyectiles/fire.png";
  }

std::string& Proyectile::getImageName() {
  return imageName_;
}

proto::Position& Proyectile::getPosition() {
  return p_.pos;
}


PowerUp::PowerUp(Client& c, proto::PowerUp p) : Drawable(c), p_(p),
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
