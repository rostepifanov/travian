#ifndef DEF_GAME_H
#define DEF_GAME_H

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "defs.structs.h"

namespace defs
{
    enum RESOURSE
    {
        WOOD = 0,
        CLAY = 1,
        IRON = 2,
        WHEAT = 3,
        CONSUMPTION = 4
    };

    enum BUILD_TYPE
    {
        LUMBER = 0,
        CLAY_QUARRY,
        IRON_MINE,
        FARM,
        MAIN,
        HIDE,
        MILL,
        SAWMILL,
        BRICKYARD,
        IRON_FOUNDRY,
        BAKERY,
        STORAGE,
        BARN,
        EMBASSY,
        MARKET,
        RESIDENCE,
        PALACE,
        MASON,
        TREASURE,
        WATER,
        TOWN_HALL,
        BOARD_TRADE,
        BIG_BRAN,
        BIG_STORAGE,
        ASSEMBLY_POINT,
        WALL,
        TAVERN,
        ACADEMY,
        SMITHY,
        STABLE,
        WORKSHOP,
        ARENA,
        UNBUILD
    };

    struct building
    {
        building() { }

        static const std::vector<std::string>types;

        friend std::ostream& operator << (std::ostream& st, const defs::building& build)
        {
            st << "Номер: " << build.id << ", тип: " << defs::building::types[build.type] << ", уровень: " << build.level;
            return st;
        }

        enum BUILD_TYPE type = UNBUILD;
        unsigned level = 0;
        unsigned id = 0;
    };

    struct resources
    {
        std::map<std::string, defs::uvector<5>> resource;

        static const std::vector<std::string> types;

        resources ()
        {
            for (std::string type : types)
              resource[type];
        }

        friend std::ostream& operator << (std::ostream& st, const defs::resources& res)
        {
            st << "Дерево: " << res.resource.at("storage")[defs::WOOD]
               << ", глина: " << res.resource.at("storage")[defs::CLAY]
               << ", железо: " << res.resource.at("storage")[defs::IRON]
               << ", зерно: " << res.resource.at("storage")[defs::WHEAT];
            return st;
        }
    };

}


#endif // DEF_GAME_H
