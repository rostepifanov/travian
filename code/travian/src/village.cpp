#include "defs.structs.h"
#include "defs.game.h"
#include "defs.h"
#include "server.h"
#include "village.h"


Village::Village(Server* const server, const std::string& address) : server(server), address(address) {
    __init_resources();
    __init_domain();
}

void Village::__init_resources(void) {
    update_resources();
}

void Village::update_resources(void) {
    server->update_page(domain);

    std::string *page = server->get_raw();

    const std::string templates = "[\\s.\\n={:]*"
                                  "\"l1\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l2\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l3\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l4\":\\s*([0-9]*)\\.{0,1}[0-9]*,?"
                                  "\"?l?5?\"?:?\\s*([0-9]*)\\.{0,1}[0-9]*";

    std::string res_map = page->substr(page->find("resources"), 600);

    std::smatch matcher;

    for (std::string type : defs::Resources::types)
    {
        std::regex regex(type + templates);
        std::regex_search(res_map, matcher, regex);
        resources.data[type][defs::WOOD] = std::stoul(matcher[1]);
        resources.data[type][defs::CLAY] = std::stoul(matcher[2]);
        resources.data[type][defs::IRON] = std::stoul(matcher[3]);
        resources.data[type][defs::WHEAT] = std::stoul(matcher[4]);

        if (defs::is_integer(matcher[5]))
          resources.data[type][defs::CONSUMPTION] = std::stoul(matcher[5]);
    }

    resources.data["storage"][defs::CONSUMPTION] = resources.data["production"][defs::CONSUMPTION];
    resources.data["maxStorage"][defs::CONSUMPTION] = resources.data["production"][defs::CONSUMPTION];
}

defs::Resources Village::get_resoursces(void) const
{
    return resources;
}

void Village::__init_domain(void) {

}
