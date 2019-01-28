#include "player.h"
#include "defs.h"
#include <fstream>
#include <regex>
#include <chrono>
#include <queue>
#include <thread>
#include "defs.game.h"

player::player(const defs::keys& info)
{
    server = info.server;

    page = con.get_data(server+login, "GET", "");

    ///TODO set up regex search

    long int t = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - 30;

    std::string arg =
            "name=" + info.login +
            "&password=" + info.password +
            "&s1=%D0%92%D0%BE%D0%B9%D1%82%D0%B8"
            "&w=1920%3A1080"    // Size of the screen (imagine 1440x900)
            "&login=" + std::to_string(t) +
            "&lowRes=0";

    page = con.get_data(server + domain, "POST", arg);
    page = con.get_data(server + domain, "GET", "");

    ///TODO check for login fail
}

void player::get_resourses(void)
{
    page = con.get_data(server + domain, "GET", "");

    const std::string templates = "[\\s.\\n={:]*"
                                  "\"l1\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l2\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l3\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                                  "\"l4\":\\s*([0-9]*)\\.{0,1}[0-9]*,?"
                                  "\"?l?5?\"?:?\\s*([0-9]*)\\.{0,1}[0-9]*";

    std::string res_map = page.substr(page.find("resources"), 600);

    std::smatch matcher;

    for (std::string type : defs::resources::types)
    {
        std::regex regex(type + templates);
        std::regex_search(res_map, matcher, regex);
        res.resource[type][defs::WOOD] = std::stoul(matcher[1]);
        res.resource[type][defs::CLAY] = std::stoul(matcher[2]);
        res.resource[type][defs::IRON] = std::stoul(matcher[3]);
        res.resource[type][defs::WHEAT] = std::stoul(matcher[4]);

        if (defs::is_integer(matcher[5]))
          res.resource[type][defs::CONSUMPTION] = std::stoul(matcher[5]);
    }

    res.resource["production"][defs::CONSUMPTION] = res.resource["storage"][defs::CONSUMPTION];
    res.resource["maxStorage"][defs::CONSUMPTION] = res.resource["storage"][defs::CONSUMPTION];

}

bool player::get_valid_build_button(void)
{
    if (get_construct_status())
      return false;

    page = con.get_data(server + build_id + std::to_string(1), "GET", "");

    const std::string tag = "dorf1.php?a=1&amp;c=";
    std::string::size_type tag_position = page.find(tag);

    button.set(page.substr(tag_position + tag.size(), 6));

    return true;
}

bool player::get_construct_status(void)
{
    page = con.get_data(server + domain, "GET", "");

    const std::string duration = "buildDuration";

    return page.find(duration) != std::string::npos;
}

//<h1 class="titleInHeader">Железный рудник <span class="level">Уровень 0</span></h1>
defs::building player::get_building_description(size_t id)
{
    page = con.get_data(server + build_id + std::to_string(id), "GET", "");

    const std::string h1_opened = "<h1 class=\"titleInHeader\">";
    const std::string h1_closed = "</h1>";
    const std::string span_opened = " <span\n\t\tclass=\"level\">";
    const std::string span_closed = "</span>";
    const std::string level = "Уровень ";

    std::string::size_type h1_begin = page.find(h1_opened);
    std::string::size_type h1_end = page.find(h1_closed, h1_begin);

    std::string info (page.substr(h1_begin, h1_end - h1_begin));

    std::string::size_type span_begin = info.find(span_opened);
    std::string::size_type span_end = info.find(span_closed);

    std::string name;
    std::string level_id;

    defs::building build;

    if (span_begin != std::string::npos)
    {
        name = info.substr(h1_opened.size(), span_begin - h1_opened.size());
        level_id = info.substr(span_begin + span_opened.size() + level.size(), span_end - (span_begin + span_opened.size() + level.size()));
        build.level = std::stoul(level_id);
    }
    else
    {
        name = info.substr(h1_opened.size());
    }

//    defs::clear(name, ' ');

    build.id = id;

    for (int i = 0; i < defs::building::types.size(); ++i)
      if (name == defs::building::types[i])
      {
        build.type = defs::BUILD_TYPE(i);
        break;
      }

    return std::move(build);
}

void player::get_domain_info(void)
{
    for (size_t i = 0; i < domain_range; ++i)
    {
        domains[i] = get_building_description(i + 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void player::get_village_info(void)
{
    for (size_t i = domain_range; i < village_range; ++i)
    {
        domains[i] = get_building_description(i + 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void player::print_domain_info(void)
{
    for(size_t id = 0; id < domain_range; ++id)
        std::cout << domains[id] << std::endl;
}

void player::print_village_info(void)
{
    for(size_t id = domain_range; id < village_range; ++id)
        std::cout << domains[id] << std::endl;
}

std::array<unsigned, 5> player::get_build_cost(const defs::building& build)
{
    std::array<unsigned, 5> cost;

    page = con.get_data(server + build_id + std::to_string(build.id), "GET", "");

    const std::string name = "contractCosts";

    std::string div = page.substr(page.find(name) - 50, 900);

    const std::string resource = "resources r";

    const std::string span = "</span>";

    for(int type = 1; type < 6; ++type)
    {
        std::string::size_type pos = div.find(resource + std::to_string(type));
        pos += div.substr(pos, std::string::npos).find(span);
        std::string::size_type pos1 = div.substr(0, pos).find_last_of(">");
        cost[defs::BUILD_TYPE(type)] = std::stoul(div.substr(pos1 + 1, pos - pos1));
    }

    return std::move(cost);
}

void player::upgrade(defs::building& build)
{
    std::string field = build.id < domain_range ? domain : village;

    if (!button.valid())
      get_valid_build_button();

    page = con.get_data(server + field, "GET", "a=" + std::to_string(build.id) + "&c=" + button());
}

void player::build(defs::BUILD_TYPE type)
{
    if (check_building(type))
      return;

    if (!button.valid())
      get_valid_build_button();

    for (size_t id = domain_range; id < village_range; ++id)
      if (domains[id].type == defs::UNBUILD)
      {
          page = con.get_data(server + village, "GET", "a=" + std::to_string(id) + "&c=" + button());
      }
}

bool player::check_building(defs::BUILD_TYPE type)
{
   for (size_t id = domain_range; id < village_range; ++id)
     if (domains[id].type == type)
       return true;

   return false;
}

void player::run_domain_upgrade_strategy(void)
{
    get_resourses();

    ///check storage existance
    if (!check_building(defs::STORAGE))
    {
        build(defs::STORAGE);
    }

    ///check barn existance
    if (!check_building(defs::BARN))
    {
        build(defs::BARN);
    }


}
