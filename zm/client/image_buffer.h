#ifndef __ZM_CLIENT_IMAGE_BUFFER_H__
#define __ZM_CLIENT_IMAGE_BUFFER_H__

#include <map>
#include <string>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>


namespace zm {

class Client;


class ImageBuffer {
Client& client_;
std::map<std::string, Glib::RefPtr<Gdk::Pixbuf>> images_;

public:
  explicit ImageBuffer(Client& client);

  void loadImage(const std::string& key, bool flipped,
                 float scaleY, float scaleX);
  Glib::RefPtr<Gdk::Pixbuf> getImage(const std::string& key, bool flipped,
      float scaleY, float scaleX);
}; // zm


}

#endif
