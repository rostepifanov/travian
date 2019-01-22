#include "player.h"
#include <curl/curl.h>

int main(void)
{
    player p(player::keys("https://ts19.travian.ru/", "repifanov","12345678"));

    p.run();
}
