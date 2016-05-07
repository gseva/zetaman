#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>


class Client {

public:
  void run(Glib::RefPtr<Gtk::Application> app);

};

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};


#endif
