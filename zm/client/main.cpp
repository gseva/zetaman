#include <gtkmm/application.h>

#include "zm/client/client.h"

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create();
    app->set_flags(Gio::APPLICATION_NON_UNIQUE);

    std::cout << argc << " " << argv[1] << std::endl;
    if (argc > 1) {
      zm::Client client(argv[1]);
      client.run(app);
    } else {
      zm::Client client("9090");
      client.run(app);
    }

    return 0;
}
