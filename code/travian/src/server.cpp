#include "server.h"
#include "defs.h"
#include "connection.h"

#include <chrono>

Server::Server(const defs::Keys& info) : url(info.server) {
    __login(info);
    __login_checkup(info);
}

void Server::__login(const defs::Keys& info) {
    this->update_page(login);

    auto current_time = std::chrono::system_clock::now().time_since_epoch();
    long int delta_t = std::chrono::duration_cast<std::chrono::seconds>(current_time).count() - 30;

    std::string arg =
            "name=" + info.login +
            "&password=" + info.password +
            "&s1=%D0%92%D0%BE%D0%B9%D1%82%D0%B8"
            "&w=1920%3A1080"    // Size of the screen (imagine 1440x900)
            "&login=" + std::to_string(delta_t) +
            "&lowRes=0";

    page = con.get_data(url + login, "POST", arg);
}

void Server::__login_checkup(const defs::Keys& info) {
    this->update_page(checkup);

    CSelection div = html.find("div[class='error']");

    if (div.nodeNum())
        throw LoginException(info);
}

void Server::update_page(const std::string& suffix) {
    page = con.get_data(url+suffix, "GET", "");
    html.parse(page);
}

std::string* Server::get_raw(void) {
    return &page;
}

CDocument* Server::get_parsed(void) {
    return &html;
}
