
#include <string>
#include <utility>

#include "zm/client/image_buffer.h"
#include "zm/client/client.h"


namespace zm {

ImageBuffer::ImageBuffer(Client& client) : client_(client) {
}

void ImageBuffer::loadImage(const std::string& key, bool flipped,
                            bool needsScaling) {
  std::string resource_name = "/zm/images/" + key;

  std::string key_ = key;
  auto image = Glib::wrap(gdk_pixbuf_new_from_resource(
                               resource_name.c_str(), 0));

  int width = client_.scaleNum(image->get_width());
  int height = client_.scaleNum(image->get_height());

  if (needsScaling) {
    image = image->scale_simple(width, height, Gdk::INTERP_BILINEAR);
  }

  if (flipped) {
    image = image->flip();
    key_ += "_flipped";
  }

  images_.insert({key_, image});
}

Glib::RefPtr<Gdk::Pixbuf> ImageBuffer::getImage(const std::string& key,
    bool flipped, bool needsScaling) {
  std::string key_ = key;
  if (flipped) {
    key_ += "_flipped";
  }

  if (!images_.count(key_)) {
    loadImage(key, flipped, needsScaling);
  }

  return images_.at(key_);
}

} // zm
