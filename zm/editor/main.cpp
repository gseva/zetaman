#include <iostream>
#include "editor.h"

int main(int argc, char **argv)
{
  auto appl = Gtk::Application::create(argc, argv, "Editor.Menu.app");

  EditorMenu editorMenu(appl);

  editorMenu.runEditorMenu();

  return 0;
}
