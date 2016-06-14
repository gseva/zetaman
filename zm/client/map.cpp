
#include <string>
#include <vector>
#include <iostream>

#include <gdkmm/general.h> // set_source_pixbuf()

#include "zm/client/map.h"


namespace zm {

Map::Map(JsonMap& jsonMap) : jsonMap_(jsonMap), hasBackground(false) {
  loadImages();
}

void Map::loadImages() {
  std::string prefix = "/zm/images/tiles/";
  for (unsigned int i = 0; i < jsonMap_.imageNames.size(); ++i) {
    std::string resource_name = prefix + jsonMap_.imageNames[i];

    // create_from_resource es de gtk 3.12, y en ubuntu 14.04 la version de gtk
    // es 3.10
    // images_[i] = Gdk::Pixbuf::create_from_resource(resource_name)
    //                     ->scale_simple(64, 64, Gdk::INTERP_BILINEAR);

    images_[i] = Glib::wrap(gdk_pixbuf_new_from_resource_at_scale(
                              resource_name.c_str(), 64, 64, TRUE, 0));
  }

  if (jsonMap_.backgroundImage != "") {
    std::string backgroundName;
    backgroundName = "/zm/images/backgrounds/" + jsonMap_.backgroundImage;
    background_ = Glib::wrap(gdk_pixbuf_new_from_resource_at_scale(
                               backgroundName.c_str(), 1024, 768, TRUE, 0));
    hasBackground = true;
  }
}

void Map::draw(const Cairo::RefPtr<Cairo::Context>& context, int x, int y) {
  context->save();

  if (hasBackground) {
    Gdk::Cairo::set_source_pixbuf(context, background_, 0);
    context->paint();
  }
  // om.tercio // 2

  int largo = 64;

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 12; ++j) {
      int tilePos = largo * (j + y) + (i + x);
      int imageNum = jsonMap_.imageNumbers[tilePos];
      if (imageNum == 0) continue;
      auto image = images_[imageNum - 1];
      int width = image->get_width();
      int height = image->get_height();
      Gdk::Cairo::set_source_pixbuf(context, image, width * i, height * j);
      context->paint();
    }
  }

  context->restore();
}

} // zm
