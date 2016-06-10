#include <iostream>
#include "editor.h"

int main(int argc, char **argv)
{
  auto appl = Gtk::Application::create(argc, argv, "");

  Editor editor(appl, 4);

  editor.runEditor();

  return 0;
}
