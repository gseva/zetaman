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

proto::Position pos_;
std::string imageName_;

public:
  int tics;

  explicit Drawable(Client& c);
  virtual ~Drawable();

  void draw(const Cairo::RefPtr<Cairo::Context>& context, ImageBuffer& buff);

  void setFlipped(bool f);
  void setScale(float x, float y);

  virtual std::string& getImageName();
  void setImageName(const std::string& imageName);

  virtual proto::Position& getPosition();
  void setPosition(proto::Position pos);
};


class Player : public Drawable {
proto::Player p_;

public:
  explicit Player(Client& c);
  virtual ~Player();

  void setState(proto::Player p);
};


class Enemy : public Drawable {
proto::Enemy e_;

public:
  explicit Enemy(Client& c);
  virtual ~Enemy();

  virtual void setState(proto::Enemy e);
};


class Met : public Enemy {
proto::Enemy e_;

public:
  explicit Met(Client& c);
  virtual ~Met();
  virtual void setState(proto::Enemy e);
};

class Bumby : public Enemy {
proto::Enemy e_;

public:
  explicit Bumby(Client& c);
  virtual ~Bumby();
  virtual void setState(proto::Enemy e);
};

class Sniper : public Enemy {
proto::Enemy e_;

public:
  explicit Sniper(Client& c);
  virtual ~Sniper();
  virtual void setState(proto::Enemy e);
};

class Bombman : public Enemy {
proto::Enemy e_;

public:
  explicit Bombman(Client& c);
  virtual ~Bombman();
  virtual void setState(proto::Enemy e);
};

class Fireman : public Enemy {
proto::Enemy e_;

public:
  explicit Fireman(Client& c);
  virtual ~Fireman();
  virtual void setState(proto::Enemy e);
};

class Magnetman : public Enemy {
proto::Enemy e_;

public:
  explicit Magnetman(Client& c);
  virtual ~Magnetman();
  virtual void setState(proto::Enemy e);
};

class Ringman : public Enemy {
proto::Enemy e_;

public:
  explicit Ringman(Client& c);
  virtual ~Ringman();
  virtual void setState(proto::Enemy e);
};

class Sparkman : public Enemy {
proto::Enemy e_;

public:
  explicit Sparkman(Client& c);
  virtual ~Sparkman();
  virtual void setState(proto::Enemy e);
};


class Proyectile : public Drawable {
int tics;
proto::Proyectile p_;

public:
  explicit Proyectile(Client& c);
  virtual ~Proyectile();

  virtual void setState(proto::Proyectile p);
};


class PowerUp : public Drawable {
proto::PowerUp p_;

public:
  explicit PowerUp(Client& c);
  virtual ~PowerUp();

  virtual void setState(proto::PowerUp p);
};



} // draw
} // zm

#endif
