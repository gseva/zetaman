
#ifndef __ZM_SERVER_CONFIG__
#define __ZM_SERVER_CONFIG__

#include <string>

namespace zm {

namespace config
{
    extern int gamePeriod;
    extern int bossLife;
    extern int playerLife;
    extern int lifes;

    extern std::string port;
    extern std::string host;

    void loadConfigFile(const std::string& file);
} //config

} // zm


#endif
