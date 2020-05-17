#include <cstdlib>

#include "app.h"
#include <curl/curl.h>

int main(void)
{
    Player p(defs::Keys(std::getenv("TRAVIAN_SERVER"), std::getenv("TRAVIAN_LOGIN"), std::getenv("TRAVIAN_PASSWORD")));

    App::get_instance().run(p);
}
