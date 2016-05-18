#ifndef __CLIENT_MAP_H
#define __CLIENT_MAP_H

#include <vector>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/context.h>

#define N_IMAGES 4

namespace zm {

class Map {
Glib::RefPtr<Gdk::Pixbuf> images_[N_IMAGES];

void loadImages();

public:
  Map();

  void draw(const Cairo::RefPtr<Cairo::Context>& context);
};

} // zm

#endif
