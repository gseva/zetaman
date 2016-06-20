#include <gtkmm/application.h>

#include "zm/client/client.h"

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv);
    app->set_flags(Gio::APPLICATION_NON_UNIQUE);

    if (argc > 2) {
      zm::Client client(argv[2]);
      client.run(app);
    } else {
      zm::Client client("9090");
      client.run(app);
    }

    return 0;
}
