
#include <string>

#include "zm/client/map.h"

#include <gdkmm/general.h> // set_source_pixbuf()

static const char* image_names[] = {
  "grass.png",
  "grass_half.png",
  "grass_mid.png",
  "ladder_mid.png"
};

namespace zm {

Map::Map() {
  loadImages();
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

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 12; ++j) {
      if (j == 0) {
        auto image = images_[0];
        Gdk::Cairo::set_source_pixbuf(context, image,
         image->get_width() * i, 768 - image->get_height());
        context->paint();
      }
      if (i == 10 && j > 1) {
        auto image = images_[3];
        Gdk::Cairo::set_source_pixbuf(context, image,
         image->get_width() * i, (768 - image->get_height() * j));
        context->paint();
      }
    }
  }

  context->restore();
}

} // zm
