
#include <string>
#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gdkmm/cursor.h> // Necesario para que no tire el famoso error
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"
#include "zm/client/client.h"


namespace zm {
namespace drawing {


Drawable::Drawable(Client& c, bool needsScaling) : c_(c), flipped_(false),
    needsScaling_(needsScaling) {
}

void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();
  auto image = buff.getImage(getImageName(), flipped_, needsScaling_);

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

Drawable::~Drawable() {}

void Drawable::setFlipped(bool f) {
  flipped_ = f;
}

Player::Player(Client& c) : Drawable(c, false), tics(0) {
}

Player::~Player() {}

void Player::setState(proto::Player p) {
  if (p_.ps != p.ps) {
    tics = 0;
  }
  tics++;

  p_ = p;

  setFlipped(p_.o == proto::left);

  std::string image = "idle_1";
  switch (p_.ps) {
    case proto::PlayerState::moving:
      image = "running_" + std::to_string(tics);
      if (tics >=  11) tics = 0;
      break;
    case proto::PlayerState::idle:
      break;
    case proto::PlayerState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 2) tics = 1;
      break;
    case proto::PlayerState::jumping:
      if (p.ps == proto::PlayerState::jumpingShooting) tics = 2;
      image = "jumping_" + std::to_string(tics);
      if (tics >= 3) tics = 2;
      break;
    case proto::PlayerState::jumpingShooting:
      image = "jumping_shooting_1";
      if (tics >= 3) tics = 2;
      break;
    case proto::PlayerState::climbing:
      image = "climbing_1";
      if (tics >= 10) image = "climbing_2";
      if (tics >= 20) tics = 0;
      break;
  }

  // if (p_.o == proto::left) {
  //   image += "_left";
  // }

  imageName_ = "player/" + image + ".png";
}

std::string& Player::getImageName() {
  return imageName_;
}

proto::Position& Player::getPosition() {
  return p_.pos;
}


Enemy::Enemy(Client& c, proto::Enemy e) : Drawable(c, true), e_(e) {
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

Enemy::~Enemy() {}

std::string& Enemy::getImageName() {
  return imageName_;
}

proto::Position& Enemy::getPosition() {
  return e_.pos;
}

Proyectile::Proyectile(Client& c, proto::Proyectile p) : Drawable(c, true),
  p_(p) {
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

Proyectile::~Proyectile() {}

std::string& Proyectile::getImageName() {
  return imageName_;
}

proto::Position& Proyectile::getPosition() {
  return p_.pos;
}


PowerUp::PowerUp(Client& c, proto::PowerUp p) : Drawable(c, true), p_(p),
  imageName_("powerups/life.png") {
}

PowerUp::~PowerUp() {}

std::string& PowerUp::getImageName() {
  return imageName_;
}

proto::Position& PowerUp::getPosition() {
  return p_.pos;
}



} // draw
} // zm
