
#include <string>
#include <utility>

#include "zm/client/image_buffer.h"
#include "zm/client/client.h"


namespace zm {

ImageBuffer::ImageBuffer(Client& client) : client_(client) {
}

void ImageBuffer::loadImage(const std::string& key, bool flipped,
                            float scaleY, float scaleX) {
  std::string resource_name = "/zm/images/" + key;

  std::string key_ = key;
  key_ += std::to_string(scaleX) + std::to_string(scaleX);
  auto image = Glib::wrap(gdk_pixbuf_new_from_resource(
                               resource_name.c_str(), 0));

  int width = client_.scaleNum(image->get_width() * scaleY);
  int height = client_.scaleNum(image->get_height() * scaleX);

  image = image->scale_simple(width, height, Gdk::INTERP_BILINEAR);

  if (flipped) {
    image = image->flip();
    key_ += "_flipped";
  }

  images_.insert({key_, image});
}

Glib::RefPtr<Gdk::Pixbuf> ImageBuffer::getImage(const std::string& key,
    bool flipped, float scaleY, float scaleX) {
  std::string key_ = key;
  key_ += std::to_string(scaleX) + std::to_string(scaleX);
  if (flipped) {
    key_ += "_flipped";
  }

  if (!images_.count(key_)) {
    loadImage(key, flipped, scaleY, scaleX);
  }

  return images_.at(key_);
}

} // zm
