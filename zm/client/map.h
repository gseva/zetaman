#ifndef __ZM_CLIENT_MAP_H__
#define __ZM_CLIENT_MAP_H__

#include <vector>
#include <string>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/context.h>

#include "zm/json/jsonserializer.h"

#define N_IMAGES 4

namespace zm {

class Map {
JsonMap& jsonMap_;
Glib::RefPtr<Gdk::Pixbuf> images_[N_IMAGES];
Glib::RefPtr<Gdk::Pixbuf> background_;
bool hasBackground;
void loadImages();

public:
  Map(JsonMap& jsonMap_);

  void draw(const Cairo::RefPtr<Cairo::Context>& context, int x, int y);
};

} // zm

#endif
