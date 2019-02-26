#include "app.h"
#include <curl/curl.h>

int main(void)
{
    player p(defs::keys("https://ts4.travian.ru/", "repifanov","12345678"));

    app::get_instance().run(p);
}
