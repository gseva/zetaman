
#include "zm/config.h"
#include "zm/server/server.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    zm::config::loadConfigFile("build/config.json");

    Server server;
    server.run();

    return 0;
}
