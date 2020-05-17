#ifndef APP_H
#define APP_H

#include "player.h"

class App {
    Player* p;

    App(void) { }
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    typedef void (App::*execute)(const CmdLine&);

    struct CmdInfo {
        std::string cmd;
        execute func;
    };

    static std::vector<CmdInfo> cmds;

    void execute_test(const CmdLine& cmd);
    void execute_info_resource(const CmdLine& cmd);
    void execute_exit(const CmdLine& cmd);

public:
    static App& get_instance(void);
    void run(Player& p);
};

#endif // APP_H
