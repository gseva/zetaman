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
float scaleX, scaleY;

public:
  explicit Drawable(Client& c);
  virtual ~Drawable();

  void draw(const Cairo::RefPtr<Cairo::Context>& context, ImageBuffer& buff);

  virtual std::string& getImageName() = 0;
  virtual proto::Position& getPosition() = 0;

  void setFlipped(bool f);
  void setScale(float x, float y);
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
proto::Position pos_;
proto::Enemy e_;
std::string imageName_;

public:
  int tics;

  explicit Enemy(Client& c);
  virtual ~Enemy();

  virtual void setState(proto::Enemy e);

  virtual std::string& getImageName() override;
  void setImageName(const std::string& imageName);

  virtual proto::Position& getPosition() override;
  void setPosition(proto::Position pos);
};


class Met : public Enemy {
proto::Enemy e_;

public:
  explicit Met(Client& c);
  virtual ~Met();
  virtual void setState(proto::Enemy e);
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
