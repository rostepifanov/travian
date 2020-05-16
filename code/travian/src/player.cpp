#include "player.h"
#include "defs.h"
#include <fstream>
#include <regex>
#include <chrono>
#include <queue>
#include <thread>
#include <iomanip>
#include "defs.game.h"

#include <iostream>
player::player(const defs::keys& info)
{
    server = info.server;

    page = con.get_data(server+login, "GET", "");

    /// TODO set up regex search

    auto current_time = std::chrono::system_clock::now().time_since_epoch();
    long int delta_t = std::chrono::duration_cast<std::chrono::seconds>(current_time).count() - 30;

    std::string arg =
            "name=" + info.login +
            "&password=" + info.password +
            "&s1=%D0%92%D0%BE%D0%B9%D1%82%D0%B8"
            "&w=1920%3A1080"    // Size of the screen (imagine 1440x900)
            "&login=" + std::to_string(delta_t) +
            "&lowRes=0";

    page = con.get_data(server + login, "POST", arg);
    page = con.get_data(server + domain, "GET", "");

    html.parse(page);

    CSelection div = html.find("div[class='error']");

    if (div.nodeNum())
        throw LoginException(info);
}

void player::update_resourses(void)
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
        resource.data[type][defs::WOOD] = std::stoul(matcher[1]);
        resource.data[type][defs::CLAY] = std::stoul(matcher[2]);
        resource.data[type][defs::IRON] = std::stoul(matcher[3]);
        resource.data[type][defs::WHEAT] = std::stoul(matcher[4]);

        if (defs::is_integer(matcher[5]))
          resource.data[type][defs::CONSUMPTION] = std::stoul(matcher[5]);
    }

    resource.data["storage"][defs::CONSUMPTION] = resource.data["production"][defs::CONSUMPTION];
    resource.data["maxStorage"][defs::CONSUMPTION] = resource.data["production"][defs::CONSUMPTION];
}

defs::resources player::get_resoursces(void) const
{
    return resource;
}

bool player::get_valid_build_button(size_t id)
{
    if (get_construct_status())
      return false;

    page = con.get_data(server + build_id + std::to_string(id), "GET", "");

    const std::string tag = "&amp;c=";
    std::string::size_type tag_position = page.find(tag);

    button.set(page.substr(tag_position + tag.size(), 6));

    return true;
}

size_t player::get_construct_status(void)
{
    page = con.get_data(server + domain, "GET", "");

    html.parse(page);

    CSelection div = html.find("div[class='buildDuration']");

    if (!div.nodeNum())
      return 0;

    CSelection span = div.find("span[class='timer']");

    std::string time = span.nodeAt(0).ownText();

    // add zero to begin
    if (time.size() == 7)
      time = "0" + time;

    std::tm parsed_time = { 0 };
    std::istringstream stream (time);
    stream >> std::get_time(&parsed_time, "%T");
    size_t duration = 3600 * parsed_time.tm_hour + 60 * parsed_time.tm_min + parsed_time.tm_sec;

    return duration;
}

defs::building player::get_building_description(size_t id)
{
    page = con.get_data(server + build_id + std::to_string(id), "GET", "");
    html.parse(page);

    //поиск всех h1 c titleInHeader
    CSelection h1 = html.find("h1[class='titleInHeader']");

    ///TODO check if node more than one

    defs::building build;
    build.id = id;

    std::string name = h1.nodeAt(0).ownText();

    for (int i = 0; i < defs::building::types.size(); ++i)
      if (name.find(defs::building::types[i]) != std::string::npos)
      {
        build.type = defs::BUILD_TYPE(i);
        break;
      }

    CSelection span = h1.nodeAt(0).find("span");
    if (span.nodeNum())
    {
        const std::string level_text = "Уровень ";
        std::string level;
        level = span.nodeAt(0).text();
        level = level.substr(level_text.size());
        build.level = std::stoul(level);
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

void player::get_building_info(int i)
{
    domains[i] = get_building_description(i + 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

size_t player::get_empty_place_id()
{
    size_t place_id = 0;

    for (size_t id = domain_range; id < village_range; ++id)
      if (domains[id].type == defs::UNBUILD)
      {
          place_id = domains[id].id;
          break;
      }

    return place_id;
}

defs::ivector<5> player::get_building_build_cost(const defs::BUILD_TYPE type)
{
    /// TODO different category

    defs::ivector<5> cost;

    size_t place_id = get_empty_place_id();

    page = con.get_data(server + build_id + std::to_string(place_id), "GET", "");
    html.parse(page);

    CSelection div = html.find("div[class='buildingWrapper']");

    for (int i = 0; i < div.nodeNum(); ++i)
    {
        CSelection title = div.nodeAt(i).find("h2");
        if (title.nodeAt(0).ownText() == defs::building::types[type])
        {
            CSelection resources = div.nodeAt(i).find("span[class='value value']");
            for (int i = 0; i < resources.nodeNum(); ++i)
              cost[i] = std::stoul(resources.nodeAt(i).text());

            return cost;
        }
    }

    return cost;
}

defs::ivector<5> player::get_building_upgrade_cost(const defs::building& build)
{
    if (build.type == defs::UNBUILD)
        return defs::ivector<5>();


    page = con.get_data(server + build_id + std::to_string(build.id), "GET", "");
    html.parse(page);

    CSelection resources = html.find("div[class='contractWrapper']").find("span[class='value value']");

    defs::ivector<5> cost;

    for (int i = 0; i < resources.nodeNum(); ++i)
      cost[i] = std::stoul(resources.nodeAt(i).text());

    return cost;
}

void player::upgrade(defs::building& build)
{
    std::string field = build.id < domain_range ? domain : village;

    get_valid_build_button(build.id);

    page = con.get_data(server + field, "GET", "a=" + std::to_string(build.id) + "&c=" + button());
}


std::string player::get_building_construct_code(defs::BUILD_TYPE type, size_t place_id)
{
    /// TODO different category

    page = con.get_data(server + build_id +  std::to_string(place_id), "GET", "");

    html.parse(page);

    CSelection div = html.find("div[class='buildingWrapper']");

    for (int i = 0; i < div.nodeNum(); ++i)
    {
        CSelection title = div.nodeAt(i).find("h2");
        if (title.nodeAt(0).ownText() == defs::building::types[type])
        {
            std::string content = div.nodeAt(i).text();
            const std::string tag = village + "?a=";
            std::string::size_type tag_position = content.find(tag);

            std::string code = content.substr(tag_position + tag.size(), 2);
            return code;
        }
    }

    return "";
}

void player::build(defs::BUILD_TYPE type)
{
    if (check_building(type))
      return;

    size_t place_id = get_empty_place_id();

    get_valid_build_button(place_id);

    std::string build_code = get_building_construct_code(type, place_id);

    page = con.get_data(server + village, "GET", "a=" + build_code + "&id=" + std::to_string(place_id) + "&c=" + button());

}

bool player::check_building(defs::BUILD_TYPE type)
{
   for (size_t id = domain_range; id < village_range; ++id)
     if (domains[id].type == type)
       return true;

   return false;
}

size_t player::get_resource_gathering_time(const defs::ivector<5>& required)
{
    update_resourses();

    if (required < resource.data["storage"])
        return 0;

    defs::ivector<5> diff = required - resource.data["storage"];
    std::pair<unsigned, int> max = diff.max();

    double production_per_second = resource.data["production"][max.first] / 3600.;

    size_t time = max.second / production_per_second;
    return time;
}

void player::run_domain_upgrade_strategy(void)
{
    get_domain_info();
    get_village_info();

    update_resourses();

    ///check storage existance
    if (!check_building(defs::STORAGE))
    {
        defs::ivector<5> cost = get_building_build_cost(defs::STORAGE);
        size_t time;
        while(time = get_resource_gathering_time(cost))
        {
            std::cout << "Gathering " << time << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        std::cout << defs::building::types[defs::STORAGE] << std::endl;
        build(defs::STORAGE);

        while (time = get_construct_status())
        {
            std::cout << "Building " << time << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        get_village_info();
    }

    ///check barn existance
    if (!check_building(defs::BARN))
    {
        defs::ivector<5> cost = get_building_build_cost(defs::BARN);
        size_t time;
        while(time = get_resource_gathering_time(cost))
        {
            std::cout << "Gathering " << time << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        std::cout << defs::building::types[defs::BARN] << std::endl;
        build(defs::BARN);

        while (time = get_construct_status())
        {
            std::cout << "Building " << time << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        get_village_info();
    }

    std::priority_queue<defs::building, std::vector<defs::building>, defs::simple_order> poll;

    for (size_t i = 0; i < domain_range; ++i)
      poll.push(domains[i]);

    operation_continue = true;

    while (operation_continue)
    {
        defs::building upgradable = poll.top();
        poll.pop();

        defs::ivector<5> cost = get_building_upgrade_cost(upgradable);

        /// TODO upgrade barn and storage

        size_t time;
        while(time = get_resource_gathering_time(cost))
        {
            std::cout << "Gathering " << time << " to " << defs::building::types[upgradable.type] << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        upgrade(upgradable);

        while (time = get_construct_status())
        {
            std::cout << "Building " << time << " to " << defs::building::types[upgradable.type] << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(std::min(time, size_t(600))));
        }

        get_building_info(upgradable.id - 1);
        poll.push(domains[upgradable.id - 1]);
    }

}
