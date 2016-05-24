#ifndef __ZM_CLIENT_MAP_H__
#define __ZM_CLIENT_MAP_H__

#include <vector>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/context.h>

#define N_IMAGES 4

namespace zm {

class Map {
std::vector<int> tiles_;
std::vector<std::string> imageNames_;
Glib::RefPtr<Gdk::Pixbuf> images_[N_IMAGES];

void loadImages();

public:
  Map(std::vector<int> tiles, std::vector<std::string> imageNames);

  void draw(const Cairo::RefPtr<Cairo::Context>& context);
};

} // zm

#endif
