#ifndef __ZM_CLIENT_DRAWABLE_H__
#define __ZM_CLIENT_DRAWABLE_H__

#include <string>

#include <cairomm/context.h>

#include "zm/game_protocol.h"
#include "zm/client/image_buffer.h"


namespace zm {

class Client;

namespace drawing {


class Drawable {
Client& c_;
bool flipped_;
bool needsScaling_;
public:
  explicit Drawable(Client& c);
  Drawable(Client& c, bool needsScaling=true);
  virtual ~Drawable();

  void draw(const Cairo::RefPtr<Cairo::Context>& context, ImageBuffer& buff);

  virtual std::string& getImageName() = 0;
  virtual proto::Position& getPosition() = 0;

  void setFlipped(bool f);
};


class Player : public Drawable {
proto::Player p_;
std::string imageName_;
int tics;

public:
  explicit Player(Client& c);
  virtual ~Player();

  void setState(proto::Player p);

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class Enemy : public Drawable {
proto::Enemy e_;
std::string imageName_;

public:
  Enemy(Client& c, proto::Enemy e);
  virtual ~Enemy();

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class Proyectile : public Drawable {
proto::Proyectile p_;
std::string imageName_;

public:
  Proyectile(Client& c, proto::Proyectile p);
  virtual ~Proyectile();

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class PowerUp : public Drawable {
proto::PowerUp p_;
std::string imageName_;

public:
  PowerUp(Client& c, proto::PowerUp p);
  virtual ~PowerUp();

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};



} // draw
} // zm

#endif
