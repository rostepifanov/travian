#include "app.h"

std::vector<app::cmd_info> app::cmds =
{
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
    p->get_resourses();
    std::cout << p->res << std::endl;
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
                    std::cout << "Oops!" << std::endl;
                }
            }
    }
}
