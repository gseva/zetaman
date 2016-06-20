
#include <string>
#include <iostream>

#include <gdkmm/pixbuf.h>
#include <gdkmm/cursor.h> // Necesario para que no tire el famoso error
#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/drawable.h"
#include "zm/client/client.h"


namespace zm {
namespace drawing {


Drawable::Drawable(Client& c) : c_(c), flipped_(false),
    scaleX(1), scaleY(1), tics(0) {
}

void Drawable::draw(const Cairo::RefPtr<Cairo::Context>& context,
                    ImageBuffer& buff) {
  context->save();
  // std::cout << getImageName() << std::endl;
  auto image = buff.getImage(getImageName(), flipped_, scaleX, scaleY);

  // int width = image->get_width();
  // int height = image->get_height();
  proto::Position pos = getPosition();

  int x = c_.scaleWidth(pos.x);
  int y = c_.scaleHeight(pos.y);

  // Gdk::Cairo::set_source_pixbuf(context, image, pos.x + width / 2,
  //                               pos.y - height / 2);
  Gdk::Cairo::set_source_pixbuf(context, image, x, y);
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

void Drawable::setScale(float x, float y) {
  scaleX = x;
  scaleY = y;
}

void Drawable::setPosition(proto::Position pos) {
  pos_ = pos;
}

proto::Position& Drawable::getPosition() {
  return pos_;
}

void Drawable::setImageName(const std::string& imageName) {
  imageName_ = imageName;
}

std::string& Drawable::getImageName() {
  return imageName_;
}


Player::Player(Client& c) : Drawable(c) {
  setScale(1.5, 1.5); tics = 0;
}

Player::~Player() {}

void Player::setState(proto::Player p) {
  if (p_.ps != p.ps) {
    tics = 0;
  }
  tics++;

  p_ = p;
  setPosition(p_.pos);

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

  setImageName("player/" + image + ".png");
}


Enemy::Enemy(Client& c) : Drawable(c) {
}

Enemy::~Enemy() {}

void Enemy::setState(proto::Enemy e) {
  setPosition(e.pos);
  setImageName("enemies/bumby/idle_1.png");
}


Met::Met(Client& c) : Enemy(c) {
  setScale(2.1, 2.1); tics = 0;
}

Met::~Met() {}

void Met::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::left);

  std::string image = "idle_1";
  switch (e.enemyState) {
    case proto::EnemyState::moving:
      image = "moving_" + std::to_string(tics);
      if (tics >=  10) tics = 0;
      break;
    case proto::EnemyState::idle: break;
    case proto::EnemyState::shooting: return;
    case proto::EnemyState::jumping: image = "jumping_1"; break;
    case proto::EnemyState::guarded:
      image = "guarded_" + std::to_string(tics);
      if (tics >=  3) tics = 2;
      break;
  }
  setImageName("enemies/met/" + image + ".png");
}


Bumby::Bumby(Client& c) : Enemy(c) {
  setScale(1.8, 1.8); tics = 0;
}

Bumby::~Bumby() {}

void Bumby::setState(proto::Enemy e) {
  e_ = e;
  setPosition(e_.pos);
  tics++;

  std::string image = "idle_" + std::to_string(tics);
  if (tics >= 12) tics = 0;

  setImageName("enemies/bumby/" + image + ".png");
}


Sniper::Sniper(Client& c) : Enemy(c) {
  setScale(1.3, 1.3); tics = 0;
}

Sniper::~Sniper() {}

void Sniper::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::right);

  std::string image = "unguarded";
  switch (e.enemyState) {
    case proto::EnemyState::moving: break;
    case proto::EnemyState::idle: break;
    case proto::EnemyState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 2) tics = 1;
      break;
    case proto::EnemyState::jumping:
      image = "jumping_" + std::to_string(tics);
      if (tics >= 2) tics = 1;
      break;
    case proto::EnemyState::guarded:
      image = "guarded_1";
      break;
  }
  setImageName("enemies/sniper/" + image + ".png");
}


Bombman::Bombman(Client& c) : Enemy(c) {
  setScale(1.5, 1.5); tics = 0;
}

Bombman::~Bombman() {}

void Bombman::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::left);

  std::string image = "moving_1";
  switch (e.enemyState) {
    case proto::EnemyState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 3) tics = 1;
      break;
    case proto::EnemyState::idle: break;
    case proto::EnemyState::moving:
      image = "moving_" + std::to_string(tics);
      if (tics >= 9) tics = 0;
      break;
    case proto::EnemyState::jumping:
      image = "jumping_" + std::to_string(tics);
      if (tics >= 2) tics = 1;
      break;
    case proto::EnemyState::guarded:
      break;
  }
  setImageName("enemies/bombman/" + image + ".png");
}


Fireman::Fireman(Client& c) : Enemy(c) {
  setScale(1.5, 1.5); tics = 0;
}

Fireman::~Fireman() {}

void Fireman::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::right);

  std::string image = "idle_1";
  switch (e.enemyState) {
    case proto::EnemyState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 4) tics = 0;
      break;
    case proto::EnemyState::moving:
    case proto::EnemyState::jumping:
      image = "moving_" + std::to_string(tics / 3 + 1);
      if (tics >= 11) tics = 0;
      break;
    case proto::EnemyState::idle:
    case proto::EnemyState::guarded:
      break;
  }
  setImageName("enemies/fireman/" + image + ".png");
}


Magnetman::Magnetman(Client& c) : Enemy(c) {
  setScale(1.5, 1.5); tics = 0;
}

Magnetman::~Magnetman() {}

void Magnetman::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::left);

  std::string image = "idle_1";
  switch (e.enemyState) {
    case proto::EnemyState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 4) tics = 0;
      break;
    case proto::EnemyState::moving:
      image = "moving_" + std::to_string(tics);
      if (tics >= 7) tics = 0;
      break;
    case proto::EnemyState::jumping:
      image = "jumping_" + std::to_string(tics);
      if (tics >= 3) tics = 2;
      break;
    case proto::EnemyState::idle:
    case proto::EnemyState::guarded:
      break;
  }
  setImageName("enemies/magnetman/" + image + ".png");
}


Ringman::Ringman(Client& c) : Enemy(c) {
  setScale(2, 2); tics = 0;
}

Ringman::~Ringman() {}

void Ringman::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::right);

  std::string image = "idle_1";
  switch (e.enemyState) {
    case proto::EnemyState::shooting:
      image = "shooting_" + std::to_string(tics);
      if (tics >= 3) tics = 1;
      break;
    case proto::EnemyState::moving:
      image = "moving_" + std::to_string(tics);
      if (tics >= 8) tics = 0;
      break;
    case proto::EnemyState::jumping:
      image = "jumping_" + std::to_string(tics);
      if (tics >= 4) tics = 3;
      break;
    case proto::EnemyState::idle:
    case proto::EnemyState::guarded:
      break;
  }
  setImageName("enemies/ringman/" + image + ".png");
}


Sparkman::Sparkman(Client& c) : Enemy(c) {
  setScale(2, 2); tics = 0;
}

Sparkman::~Sparkman() {}

void Sparkman::setState(proto::Enemy e) {
  if (e.enemyState != e_.enemyState) {
    tics = 0;
  }
  e_ = e;
  setPosition(e_.pos);
  tics++;

  setFlipped(e_.o == proto::right);

  std::string image = "idle_1";
  switch (e.enemyState) {
    case proto::EnemyState::moving:
      image = "moving_" + std::to_string(tics);
      if (tics >= 7) tics = 0;
      break;
    case proto::EnemyState::jumping:
      image = "jumping_1";
      break;
    case proto::EnemyState::shooting:
    case proto::EnemyState::idle:
    case proto::EnemyState::guarded:
      break;
  }

  setImageName("enemies/sparkman/" + image + ".png");
}


Proyectile::Proyectile(Client& c) : Drawable(c) {
  setScale(1, 1); tics = 0;
}

void Proyectile::setState(proto::Proyectile p) {
  setPosition(p.pos);
  tics++;
  std::string image = "normal";
  switch (p.type) {
    case proto::ProyectileType::Bomb:
      image = "bomb_" + std::to_string(tics);
      if (tics >= 8) tics = 0;
      break;
    case proto::ProyectileType::Fire:
      image = "fire_" + std::to_string(tics);
      if (tics >= 5) tics = 4;
      break;
    case proto::ProyectileType::Magnet:
      // setFlipped(p_.o == proto::left);
      image = "magnet_" + std::to_string(tics / 2 + 1);
      if (tics >= 3) tics = 0;
      break;
    case proto::ProyectileType::Ring:
      image = "ring_" + std::to_string(tics);
      if (tics >= 8) tics = 0;
      break;
    case proto::ProyectileType::Spark:
      image = "spark_" + std::to_string(tics);
      if (tics >= 8) tics = 4;
      break;
    case proto::ProyectileType::Normal: break;
  }

  if (image != "normal") {
    std::cout << image << std::endl;
  }
  setImageName("proyectiles/" + image + ".png");
}

Proyectile::~Proyectile() {}


PowerUp::PowerUp(Client& c) : Drawable(c) {
}

PowerUp::~PowerUp() {}

void PowerUp::setState(proto::PowerUp p) {
  setPosition(p.pos);
  setImageName("powerups/life.png");
}



} // draw
} // zm
