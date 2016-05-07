
#include "client.h"


void Client::run(Glib::RefPtr<Gtk::Application> app) {

  Gtk::Window window;
  window.set_default_size(1024, 768);

  MyArea area;
  window.add(area);
  area.show();

  app->run(window);

}


MyArea::MyArea() {
}

MyArea::~MyArea() {
}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // coordinates for the center of the window
  int xc, yc;
  xc = width / 2;
  yc = height / 2;

  int radius = 35;

  cr->set_line_width(10.0);

  // Dibujo de un circulo
  cr->save();
  cr->arc(xc, height - radius - 5, radius, 0.0, 2.0 * M_PI); // Un circulo
  cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // Parcialmente transparente
  cr->fill_preserve();
  cr->restore();  // Vuelvo a un negro opaco
  cr->stroke();

  return true;
}
