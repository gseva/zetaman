
#include "zm/server/config.h"
#include "zm/server/server.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    zm::config::loadConfigFile("build/config.json");

    if (argc > 2) {
      Server server(argv[2]);
      server.run();
    } else {
      Server server("9090");
      server.run();
    }

    return 0;
}
