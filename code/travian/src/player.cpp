#include "player.h"
#include "defs.h"
#include <fstream>
#include <regex>
#include <chrono>
#include <queue>
#include <thread>

std::vector<player::cmd_info> player::cmds =
{
    {"resource", &player::execute_info_resource},
    {"exit", &player::execute_exit}
};

const std::vector<std::string> player::building::types = {
    {"Лесопилка"},
    {"Глиняный карьер"},
    {"Железный рудник"},
    {"Ферма"},
    {"Главное здание"},
    {"Тайник"},
    {"Мукомольная мельница"},
    {"Лесопильный завод"},
    {"Кирпичный завод"},
    {"Чугунолитейный завод"},
    {"Пекарня"},
    {"Склад"},
    {"Амбар"},
    {"Посольство"},
    {"Рынок"},
    {"Резиденция"},
    {"Дворец"},
    {"Каменотес"},
    {"Сокровищница"},
    {"Водопой"},
    {"Ратуша"},
    {"Торговая палата"},
    {"Большой амбар"},
    {"Большой склад"},
    {"Пункт сбора"},
    {"Стена"},
    {"Таверна"},
    {"Академия"},
    {"Кузница"},
    {"Конюшня"},
    {"Мастерская"},
    {"Арена"},
    {"Не построено"}
};

player::player(const keys& info)
{
    server = info.server;

    page = con.get_data(server+login, "GET", "");

    ///TODO set up regex search

//    std::smatch matcher;
//    std::regex rlogin ("<input type=\"text\" name=\"([0-9a-z]*)\"");
//    std::regex rpassword ("<input type=\"password\" name=\"([0-9a-z]*)\"");
//    std::regex rhidden ("<input type=\"hidden\" name=\"([0-9a-z]*)\"");

//    std::regex_search(page, matcher, rlogin);
//    std::string login = matcher[1].str();

//    std::regex_search(page, matcher, rpassword);
//    std::string password = matcher[1].str();

//    std::regex_search(page, matcher, rhidden);
//    std::string hidden = matcher[1].str();

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

    get_build_button_id();
    get_domain_info();
    get_village_info();

//    build(domains[0]);

    ///TODO check for login fail
}

void player::get_resourses(void)
{
    page = con.get_data(server + domain, "GET", "");

    const std::regex production("production[\\s.\\n={:]*"
                                "\"l1\":\\s*([0-9]*),"
                                "\"l2\":\\s*([0-9]*),"
                                "\"l3\":\\s*([0-9]*),"
                                "\"l4\":\\s*([0-9]*),"
                                "\"l5\":\\s*([0-9]*)");

    const std::regex storage("storage[\\s.\\n={:]*"
                             "\"l1\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                             "\"l2\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                             "\"l3\":\\s*([0-9]*)\\.{0,1}[0-9]*,"
                             "\"l4\":\\s*([0-9]*)");

    const std::regex max_storage("maxStorage[\\s.\\n={:]*"
                                 "\"l1\":\\s*([0-9]*),"
                                 "\"l2\":\\s*([0-9]*),"
                                 "\"l3\":\\s*([0-9]*),"
                                 "\"l4\":\\s*([0-9]*)");

    std::string res_map = page.substr(page.find("resources"), 600);

    std::smatch matcher;
    std::regex_search(res_map, matcher, production);
    res.production[WOOD] = std::stoul(matcher[1]);
    res.production[CLAY] = std::stoul(matcher[2]);
    res.production[IRON] = std::stoul(matcher[3]);
    res.production[WHEAT] = std::stoul(matcher[4]);
    res.production[CONSUMPTION] = std::stoul(matcher[5]);

    std::regex_search(res_map, matcher, storage);
    res.storage[WOOD] = std::stoul(matcher[1]);
    res.storage[CLAY] = std::stoul(matcher[2]);
    res.storage[IRON] = std::stoul(matcher[3]);
    res.storage[WHEAT] = std::stoul(matcher[4]);

    std::regex_search(res_map, matcher, max_storage);
    res.max_storage[WOOD] = std::stoul(matcher[1]);
    res.max_storage[CLAY] = std::stoul(matcher[2]);
    res.max_storage[IRON] = std::stoul(matcher[3]);
    res.max_storage[WHEAT] = std::stoul(matcher[4]);

}

void player::get_build_button_id(void)
{
    page = con.get_data(server + build_id + std::to_string(1), "GET", "");

    const std::string tag = "dorf1.php?a=1&amp;c=";
    std::string::size_type tag_position = page.find(tag);

    build_button_id = page.substr(tag_position + tag.size(), 6);
}

//<h1 class="titleInHeader">Железный рудник <span class="level">Уровень 0</span></h1>
player::building player::get_building_description(size_t id)
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

    building build;

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

    for (int i = 0; i < player::building::types.size(); ++i)
      if (name == player::building::types[i])
      {
        build.type = player::building::TYPE(i);
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

std::ostream& operator<< (std::ostream& st, const player::resourses& res)
{
    st << "Дерево: " << res.storage[player::RESOURSE::WOOD]
       << ", глина: " << res.storage[player::RESOURSE::CLAY]
       << ", железо: " << res.storage[player::RESOURSE::IRON]
       << ", зерно: " << res.storage[player::RESOURSE::WHEAT];
    return st;
}

std::ostream& operator << (std::ostream& st, const player::building& build)
{
    st << "Номер: " << build.id << ", тип: " << player::building::types[build.type] << ", уровень: " << build.level;
    return st;
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

std::array<unsigned, 5> player::get_build_cost(const building& build)
{
    std::array<unsigned, 5> cost;

    page = con.get_data(server + build_id + std::to_string(build.id), "GET", "");

    const std::string name = "contractCosts";

    std::string div = page.substr(page.find(name) - 50, 900);

    const std::array<std::string, 5> res_type = { "resources r1",  "resources r2",  "resources r3", "resources r4", "resources r5"};

    const std::string span = "</span>";

    for(int type = 0; type < 5; ++type)
    {
        std::string::size_type pos = div.find(res_type[type]);
        pos += div.substr(pos, std::string::npos).find(span);
        std::string::size_type pos1 = div.substr(0, pos).find_last_of(">");
        cost[building::TYPE(type)] = std::stoul(div.substr(pos1 + 1, pos - pos1));
    }

    return std::move(cost);
}

void player::execute_info_resource(const cmd_line& cmd)
{
    get_resourses();
    std::cout << res << std::endl;
}

void player::execute_exit(const cmd_line& cmd)
{
    std::exit(0);
}

void player::run(void)
{
//    print_domain_info();
//    print_village_info();

    std::string line;

    while(std::getline(std::cin, line))
    {
        cmd_line cmd(line);

        for(cmd_info info : cmds)
            if(info.cmd == cmd.get_cmd())
            {
                try
                {
                    (*this.*(info.func))(cmd);
                    break;
                }
                catch (std::exception& e)
                {
                }
            }
    }
}

void player::upgrade(building& build)
{
    std::string field = build.id < domain_range ? domain : village;

    page = con.get_data(server + field, "GET", "a=" + std::to_string(build.id) + "&c=" + build_button_id);
}

void player::build(player::building::TYPE type)
{
    if (check_building(type))
      return;

    for (size_t id = domain_range; id < village_range; ++id)
      if (domains[id].type == player::building::UNBUILD)
      {
          page = con.get_data(server + village, "GET", "a=" + std::to_string(id) + "&c=" + build_button_id);
      }
}

bool player::check_building(player::building::TYPE type)
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
    if (!check_building(player::building::STORAGE))
    {
        build(player::building::STORAGE);
    }

    ///check storage existance
    if (!check_building(player::building::STORAGE))
    {
        build(player::building::STORAGE);
    }



//    std::priority_queue<area_info> build_queue;
//    for(area_info area: areas)
//        build_queue.push(area);
//\

//    while(true)
//    {

//    }
}
