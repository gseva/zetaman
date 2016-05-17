
#include <string>

#include "zm/client/map.h"

#include <gdkmm/general.h> // set_source_pixbuf()

static const char* image_names[] = {
  "grass.png",
  "ladder_mid.png",
  "grass_half.png",
  "grass_mid.png"
};

namespace zm {

Map::Map() {
  loadImages();
  // images_ v = {"grass.png", "ladder_mid.png"};
  tiles_ = {
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
}

void Map::loadImages() {
  std::string prefix = "/assets/images/";
  for (int i = 0; i < N_IMAGES; ++i) {
    std::string resource_name = prefix + image_names[i];
    images_[i] = Gdk::Pixbuf::create_from_resource(resource_name)
                        ->scale_simple(64, 64, Gdk::INTERP_BILINEAR);
  }
}

void Map::draw(const Cairo::RefPtr<Cairo::Context>& context) {
  context->save();

  // om.tercio // 2

  int largo = 20;

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 12; ++j) {
      int tilePos = largo * (12 - j) + i;
      int imageNum = tiles_[tilePos];
      if (imageNum == 0) continue;
      auto image = images_[imageNum - 1];
      Gdk::Cairo::set_source_pixbuf(context, image,
       image->get_width() * i, 768 - (image->get_height() * j));
      context->paint();
    }
  }

  context->restore();
}

} // zm
