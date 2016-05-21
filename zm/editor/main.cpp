#include <iostream>
#include "editor.h"

int main(int argc, char **argv)
{
  Editor editor(argc, argv);

  editor.runEditor();

  return 0;
}
