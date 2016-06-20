
#ifndef __ZM_SERVER_CONFIG__
#define __ZM_SERVER_CONFIG__

#include <string>

namespace zm {

namespace config
{
    extern int gamePeriod;

    void loadConfigFile(const std::string& file);
} //config

} // zm


#endif
