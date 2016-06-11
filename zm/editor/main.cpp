#include <iostream>
#include "editor.h"

int main(int argc, char **argv)
{
  auto appl = Gtk::Application::create(argc, argv, "");

  Editor editor(appl);

  editor.runEditor();

  return 0;
}
