
#include <string>
#include <utility>

#include "zm/client/image_buffer.h"
#include "zm/client/client.h"


namespace zm {

ImageBuffer::ImageBuffer(Client& client) : client_(client) {
}

Glib::RefPtr<Gdk::Pixbuf> ImageBuffer::getImage(const std::string& key) {
  if (!images_.count(key)) {
    std::string resource_name = "/zm/images/" + key;

    auto image = Glib::wrap(gdk_pixbuf_new_from_resource(
                                 resource_name.c_str(), 0));

    int width = client_.scaleNum(image->get_width());
    int height = client_.scaleNum(image->get_height());

    image = image->scale_simple(width, height, Gdk::INTERP_BILINEAR);

    images_.insert({key, image});
  }

  return images_.at(key);
}

} // zm
