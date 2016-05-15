#include <gtkmm.h>
#include <iostream>

int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "");
  Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("probando.glade");
  Gtk::Button * psomeButton;
  Gtk::Window * pwindow;
  Gtk::Label * plabel;
  Gtk::Image * pimage;
  builder->get_widget("button1", psomeButton);
  builder->get_widget("applicationwindow1", pwindow);
  builder->get_widget("label1", plabel);
  builder->get_widget("image1", pimage);

  app->run(*pwindow);

  return 0;
}