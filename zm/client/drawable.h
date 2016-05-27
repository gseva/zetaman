#ifndef __ZM_CLIENT_DRAWABLE_H__
#define __ZM_CLIENT_DRAWABLE_H__

#include <string>

#include <cairomm/context.h>

#include "zm/game_protocol.h"
#include "zm/client/image_buffer.h"


namespace zm {
namespace drawing {


class Drawable {
public:
  void draw(const Cairo::RefPtr<Cairo::Context>& context, ImageBuffer& buff);

  virtual std::string& getImageName() = 0;
  virtual zm::proto::Position& getPosition() = 0;
};


class Player : public Drawable {
zm::proto::Player p_;
std::string imageName_;

public:
  explicit Player(zm::proto::Player p);

  virtual std::string& getImageName() override;
  virtual zm::proto::Position& getPosition() override;
};


class Enemy : public Drawable {
zm::proto::Enemy e_;
std::string imageName_;

public:
  explicit Enemy(zm::proto::Enemy e);

  virtual std::string& getImageName() override;
  virtual zm::proto::Position& getPosition() override;
};


class Proyectile : public Drawable {
zm::proto::Proyectile p_;
std::string imageName_;

public:
  explicit Proyectile(zm::proto::Proyectile p);

  virtual std::string& getImageName() override;
  virtual zm::proto::Position& getPosition() override;
};


class PowerUp : public Drawable {
zm::proto::PowerUp p_;
std::string imageName_;

public:
  explicit PowerUp(zm::proto::PowerUp p);

  virtual std::string& getImageName() override;
  virtual zm::proto::Position& getPosition() override;
};



} // draw
} // zm

#endif
