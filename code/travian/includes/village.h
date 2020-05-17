#ifndef VILLAGE_H
#define VILLAGE_H

#include <string>
#include <vector>

#include "defs.structs.h"
#include "defs.game.h"
#include "defs.h"
#include "server.h"


class Village {
    public:
        Village(Server* const server, const std::string& address);

        const std::string& get_id() { return address; }

        void update_resources(void);
        defs::Resources get_resoursces(void) const;
    private:
        /// common properties
        const std::string domain = "dorf1.php";
        const std::string village = "dorf2.php";
        const std::string build_id = "build.php?id=";

        /// custom properties
        Server* const server;
        std::string address;

        defs::Resources resources;

        std::array<defs::Building, 40> domains;
        const size_t domain_range = 18;
        const size_t village_range = 40;

        /// init functions
        void __init_resources(void);
        void __init_domain(void);

};

#endif // VILLAGE_H
