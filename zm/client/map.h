#ifndef __ZM_CLIENT_MAP_H__
#define __ZM_CLIENT_MAP_H__

#include <vector>
#include <string>

#include <gdkmm/pixbuf.h>
#include <glibmm/refptr.h>
#include <cairomm/context.h>

#include "zm/json/jsonserializer.h"

#define N_IMAGES 4

namespace zm {

class Client;


class Map {
Client& client_;
JsonMap& jsonMap_;
std::vector<Glib::RefPtr<Gdk::Pixbuf>> images_;
Glib::RefPtr<Gdk::Pixbuf> background_;
bool hasBackground;
int offset_;
void loadImages();

public:
  explicit Map(Client& client);

  void draw(const Cairo::RefPtr<Cairo::Context>& context, int x, int y);
};

} // zm

#endif
