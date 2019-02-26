#include "app.h"
#include <curl/curl.h>
#include "html.h"

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(void)
{
//    player p(defs::keys("https://ts4.travian.ru/", "repifanov","12345678"));

//    app::get_instance().run(p);


    std::ifstream theFile ("tr.html");
    std::string buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
//	buffer.push_back('\0');

    html page(buffer);
}
