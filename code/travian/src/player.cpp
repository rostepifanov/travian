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

//void player::get_domain_info(void)
//{
//    domains.clear();

//    ///TODO пробежать прямо по зданиям от 1 до 18 это будет быстрее

//    std::string map (page.substr(page.find("map name"), page.find("/map") - page.find("map name")));

//    while(map.find(build_id) != std::string::npos)
//    {
//        std::string area = map.substr(map.find("<area"), map.find("/>") - map.find("<area"));
//        std::string::size_type id_m1 = area.find(build_id) + build_id.length();
//        std::string::size_type id_m2 = area.find("\"", id_m1);

//        building build;
//        build.id = std::stoul(area.substr(id_m1, id_m2 - id_m1));

//        if(area.find("Лесопилка") != std::string::npos) build.type = building::WOOD;
//        else if(area.find("Глиняный") != std::string::npos) build.type = building::CLAY;
//        else if(area.find("Железный") != std::string::npos) build.type = building::IRON;
//        else if(area.find("Ферма") != std::string::npos) build.type = building::WHEAT;

//        std::string::size_type level = area.find("Уровень ") + std::string("Уровень ").length();
//        build.level = std::stoul(area.substr(level, 1));
//        domains.push_back(build);
//        map = map.substr(map.find("/>") + 2, std::string::npos);
//    }
//}

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

//    get_domain_info();
//    get_village_info();

//    print_domain_info();
//    print_village_info();

//    read_config();

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
//                    std::cout << "[ERROR] Ошибка времени исполнения команды " << cmd.get_cmd() << std::endl;
//                    if(active_stream != &std::cin)
//                    {
//                        active_stream = &std::cin;
//                        std::cout << "[INFO] ввод переключен на консоль" << std::endl;
//                    }
                }
            }
    }
}


void player::run_domain_upgrade_strategy(void)
{
//    std::priority_queue<area_info> build_queue;
//    for(area_info area: areas)
//        build_queue.push(area);
//\

//    while(true)
//    {

//    }
}
