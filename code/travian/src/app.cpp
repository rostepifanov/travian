#include "app.h"

std::vector<app::cmd_info> app::cmds =
{
    {"test", &app::execute_test},
    {"resource", &app::execute_info_resource},
    {"exit", &app::execute_exit}
};

app& app::get_instance(void)
{
    static app instance;
    return instance;
}

void app::execute_info_resource(const cmd_line& cmd)
{
    p->update_resourses();
    std::cout << p->get_resoursces() << std::endl;
}

void app::execute_test(const cmd_line& cmd)
{
//    p->get_domain_info();
//    p->print_domain_info();
//    std::cout << p->res << std::endl;
    p ->run_domain_upgrade_strategy();
//    std::cout << p->get_construct_status();
}

void app::execute_exit(const cmd_line& cmd)
{
    std::exit(0);
}

void app::run(player& p)
{
    this->p = &p;

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
                    std::cout << e.what() << std::endl;
                }
            }
    }
}
