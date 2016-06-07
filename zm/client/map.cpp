
#include <string>
#include <iostream>

#include "zm/client/map.h"
#include <vector>
#include <gdkmm/general.h> // set_source_pixbuf()


namespace zm {

Map::Map(std::vector<int> tiles, std::vector<std::string> imageNames) :
        tiles_(tiles), imageNames_(imageNames) {
  loadImages();
}

void Map::loadImages() {
  std::string prefix = "/zm/images/tiles/";
  for (unsigned int i = 0; i < imageNames_.size(); ++i) {
    std::string resource_name = prefix + imageNames_[i];

    // create_from_resource es de gtk 3.12, y en ubuntu 14.04 la version de gtk
    // es 3.10
    // images_[i] = Gdk::Pixbuf::create_from_resource(resource_name)
    //                     ->scale_simple(64, 64, Gdk::INTERP_BILINEAR);

    images_[i] = Glib::wrap(gdk_pixbuf_new_from_resource_at_scale(
                              resource_name.c_str(), 64, 64, TRUE, 0));
  }
}

void Map::draw(const Cairo::RefPtr<Cairo::Context>& context, int x, int y) {
  context->save();

  // om.tercio // 2

  int largo = 64;

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 12; ++j) {
      int tilePos = largo * (j+y) + (i + x);
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
