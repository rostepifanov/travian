#include "app.h"
#include <curl/curl.h>

#include "rapidxml/rapidxml.hpp"

int main(void)
{
    rapidxml::xml_document<> doc;

    player p(defs::keys("https://ts19.travian.ru/", "repifanov","12345678"));

    app::get_instance().run(p);
}
