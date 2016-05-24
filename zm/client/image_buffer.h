#ifndef __ZM_CLIENT_IMAGE_BUFFER_H__
#define __ZM_CLIENT_IMAGE_BUFFER_H__

#include <map>
#include <string>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>


namespace zm {

class ImageBuffer {
std::map<std::string, Glib::RefPtr<Gdk::Pixbuf>> images_;

public:
  Glib::RefPtr<Gdk::Pixbuf> getImage(const std::string& key);
}; // zm

}

#endif
