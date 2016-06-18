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
public:
  explicit Drawable(Client& c);
  void draw(const Cairo::RefPtr<Cairo::Context>& context, ImageBuffer& buff);

  virtual std::string& getImageName() = 0;
  virtual proto::Position& getPosition() = 0;
};


class Player : public Drawable {
proto::Player p_;
std::string imageName_;

public:
  Player(Client& c, proto::Player p);

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class Enemy : public Drawable {
proto::Enemy e_;
std::string imageName_;

public:
  Enemy(Client& c, proto::Enemy e);

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class Proyectile : public Drawable {
proto::Proyectile p_;
std::string imageName_;

public:
  Proyectile(Client& c, proto::Proyectile p);

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};


class PowerUp : public Drawable {
proto::PowerUp p_;
std::string imageName_;

public:
  PowerUp(Client& c, proto::PowerUp p);

  virtual std::string& getImageName() override;
  virtual proto::Position& getPosition() override;
};



} // draw
} // zm

#endif
