#ifndef __ZM_CLIENT_CLIENT__
#define __ZM_CLIENT_CLIENT__

#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>


class Client {

public:
  void run(Glib::RefPtr<Gtk::Application> app);

};

class Window : public Gtk::Window {

Client* c_;
public:
  Window(Client* c);

private:
  bool keyReleased(GdkEventKey* event);

};

class Area : public Gtk::DrawingArea
{
public:
  Area();
  virtual ~Area();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};


#endif
