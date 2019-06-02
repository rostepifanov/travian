#include <cstdlib>

#include "app.h"
#include <curl/curl.h>

int main(void)
{
    player p(defs::keys(std::getenv("TRAVIAN_SERVER"), std::getenv("TRAVIAN_LOGIN2"), std::getenv("TRAVIAN_PASSWORD")));

    app::get_instance().run(p);
}
