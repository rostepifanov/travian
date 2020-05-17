#ifndef DEF_GAME_H
#define DEF_GAME_H

#include "defs.h"
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

    struct Building {
        Building() { }

        static const std::vector<std::string>types;

//        friend std::ostream& operator << (std::ostream& st, const defs::building& build)
//        {
//            st << "Номер: " << build.id << ", тип: " << defs::building::types[build.type] << ", уровень: " << build.level;
//            return st;
//        }

//        friend bool order (const building& left, const building& right);

        enum BUILD_TYPE type = UNBUILD;
        unsigned level = 0;
        unsigned id = 0;
//    };

//    struct simple_order
//    {
//        bool operator () (const building& left, const building& right)
//        {
//            return left.level >= right.level;
//        }
//    };

//    struct barn_storage_priority_order
//    {
//        bool operator () (const building& left, const building& right)
//        {
//            return right.type == defs::BUILD_TYPE::BARN || right.type == defs::BUILD_TYPE::STORAGE || left.level > right.level;
//        }
    };

    struct Resources {
        std::map<std::string, defs::ivector<5>> data;

        static const std::vector<std::string> types;

        Resources () {
            for (std::string type : types) {
              data[type];
            }
        }

        friend std::ostream& operator << (std::ostream& st, const defs::Resources& obj) {
            st << "Дерево: " << obj.data.at("storage")[defs::WOOD]
               << ", глина: " << obj.data.at("storage")[defs::CLAY]
               << ", железо: " << obj.data.at("storage")[defs::IRON]
               << ", зерно: " << obj.data.at("storage")[defs::WHEAT];
            return st;
        }
    };

}


#endif // DEF_GAME_H
