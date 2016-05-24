
#include <string>

#include "zm/client/map.h"

#include <gdkmm/general.h> // set_source_pixbuf()

static const char* image_names[] = {
  "grass.png",
  "ladder_mid.png",
  "grass_half.png",
  "grass_mid.png"
};
static const int tiles_[240] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};


namespace zm {

Map::Map() {
  loadImages();
  // images_ v = {"grass.png", "ladder_mid.png"};
}

void Map::loadImages() {
  std::string prefix = "/zm/images/tiles/";
  for (int i = 0; i < N_IMAGES; ++i) {
    std::string resource_name = prefix + image_names[i];

    // create_from_resource es de gtk 3.12, y en ubuntu 14.04 la version de gtk
    // es 3.10
    // images_[i] = Gdk::Pixbuf::create_from_resource(resource_name)
    //                     ->scale_simple(64, 64, Gdk::INTERP_BILINEAR);

    images_[i] = Glib::wrap(gdk_pixbuf_new_from_resource_at_scale(
                              resource_name.c_str(), 64, 64, TRUE, 0));
  }
}

void Map::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  context->save();

  // om.tercio // 2

  int largo = 20;

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 12; ++j) {
      int tilePos = largo * j + i;
      int imageNum = tiles_[tilePos];
      if (imageNum == 0) continue;
      auto image = images_[imageNum - 1];
      Gdk::Cairo::set_source_pixbuf(context, image,
       image->get_width() * i, image->get_height() * j);
      context->paint();
    }
  }

  context->restore();
}

} // zm
