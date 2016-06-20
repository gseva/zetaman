#include <gtkmm/application.h>

#include "zm/client/client.h"

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv);
    app->set_flags(Gio::APPLICATION_NON_UNIQUE);

    zm::Client client;

    client.run(app);
    return 0;
}
