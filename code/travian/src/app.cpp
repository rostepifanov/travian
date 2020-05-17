#include "app.h"

std::vector<App::CmdInfo> App::cmds =
{
//    {"test", &app::execute_test},
//    {"resource", &app::execute_info_resource},
    {"exit", &App::execute_exit}
};

App& App::get_instance(void)
{
    static App instance;
    return instance;
}

//void app::execute_info_resource(const cmd_line& cmd)
//{
//    p->update_resourses();
//    std::cout << p->get_resoursces() << std::endl;
//}

//void app::execute_test(const cmd_line& cmd)
//{
////    p->get_domain_info();
////    p->print_domain_info();
////    std::cout << p->res << std::endl;
//    p ->run_domain_upgrade_strategy();
////    std::cout << p->get_construct_status();
//}

void App::execute_exit(const CmdLine& cmd)
{
    std::exit(0);
}

void App::run(Player& p)
{
    this->p = &p;

    std::string line;


    while(std::getline(std::cin, line))
    {
        CmdLine cmd(line);

        for(CmdInfo info : cmds)
            if(info.cmd == cmd.get_cmd())
            {
                try
                {
                    (*this.*(info.func))(cmd);
                    break;
                }
                catch (std::exception& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }
    }
}
