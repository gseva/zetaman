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
  virtual zm::Position& getPosition() = 0;
};


class Player : public Drawable {
zm::Player p_;
std::string imageName_;

public:
  explicit Player(zm::Player p);

  virtual std::string& getImageName() override;
  virtual zm::Position& getPosition() override;
};


class Enemy : public Drawable {
zm::Enemy e_;
std::string imageName_;

public:
  explicit Enemy(zm::Enemy e);

  virtual std::string& getImageName() override;
  virtual zm::Position& getPosition() override;
};


class Proyectile : public Drawable {
zm::Proyectile p_;
std::string imageName_;

public:
  explicit Proyectile(zm::Proyectile p);

  virtual std::string& getImageName() override;
  virtual zm::Position& getPosition() override;
};


class PowerUp : public Drawable {
zm::PowerUp p_;
std::string imageName_;

public:
  explicit PowerUp(zm::PowerUp p);

  virtual std::string& getImageName() override;
  virtual zm::Position& getPosition() override;
};



} // draw
} // zm

#endif
