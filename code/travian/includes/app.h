#ifndef APP_H
#define APP_H

#include "player.h"

class app
{
    player* p;

    app(void) { }
    app(const app&) = delete;
    app& operator=(const app&) = delete;

    typedef void (app::*execute)(const cmd_line&);

    struct cmd_info
    {
        std::string cmd;
        execute func;
    };

    static std::vector<cmd_info> cmds;

    void execute_test(const cmd_line& cmd);
    void execute_info_resource(const cmd_line& cmd);
    void execute_exit(const cmd_line& cmd);

public:
    static app& get_instance(void);
    void run(player& p);
};

#endif // APP_H
