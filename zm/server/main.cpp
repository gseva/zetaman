#include "zm/server/server.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    Server server;
    srand(time(NULL));
    server.run();

    return 0;
}
