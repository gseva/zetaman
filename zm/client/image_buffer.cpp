
#include <string>
#include <utility>

#include "zm/client/image_buffer.h"


namespace zm {

Glib::RefPtr<Gdk::Pixbuf> ImageBuffer::getImage(const std::string& key) {
  if (!images_.count(key)) {
    std::string resource_name = "/zm/images/" + key;

    auto image = Glib::wrap(gdk_pixbuf_new_from_resource(
                                 resource_name.c_str(), 0));
    images_.insert(std::make_pair(key, image));
  }

  return images_.at(key);
}

} // zm
