#include <gtkmm/application.h>

#include "zm/config.h"
#include "zm/client/client.h"

int main(int argc, char *argv[])
{
    zm::config::loadConfigFile("build/config.json");

    Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv);
    app->set_flags(Gio::APPLICATION_NON_UNIQUE);

    zm::Client client;
    client.run(app);

    return 0;
}
