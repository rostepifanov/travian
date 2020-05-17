#include "cmd_line.h"
#include "defs.h"


CmdLine::CmdLine(const std::string& line)
{
    //Поиск комментария в строке
    std::string::size_type pos = line.find('#');

    std::vector<std::string> sublines(defs::split(line.substr(0, pos), ' '));

    cmd = sublines.front();
    sublines.erase(sublines.begin());

    for (const std::string& subline : sublines)
    {
        // Принимает аргументы только с префиксом '-'
        if (subline.front() != '-')
        {
            args_without_name.emplace_back(subline);
            continue;
        }

        std::string name;
        std::string value = "";

        // Проверка аргументов вида "-name=values"
        std::string::size_type pos = subline.find('=');
        if (pos != std::string::npos)
        {
            name = subline.substr(1, pos - 1);
            value = subline.substr(pos+1, std::string::npos);

            // Проверка значений вида -name=value_1,...,value_n
            if(value.find(',') != std::string::npos)
            {
                for(const std::string& subvalue : defs::split(value, ','))
                    args.push_back({name, subvalue});
                continue;
            }
        }
        else
            name = subline.substr(1, std::string::npos);

        args.emplace_back(std::array<std::string, 2>{std::move(name), std::move(value)});
    }
}

bool CmdLine::has(const std::string& name) const
{
    return std::any_of(args.begin(), args.end(), [&name](const std::array<std::string, 2>& arg){ return arg[0] == name;});
}

std::string CmdLine::get_cmd() const
{
    return cmd;
}

std::string CmdLine::get(const std::string& name) const
{
    std::vector<named_arg>::const_iterator it = std::find_if(args.begin(), args.end(), [&name](const std::array<std::string, 2>& arg){ return arg[0] == name;});
    return it != args.end() ? (*it)[1] : "";
}

std::vector<std::string> CmdLine::get_multiple(const std::string& name) const
{
    std::vector<std::string> values;
    std::vector<named_arg>::const_iterator it = args.begin();
    while ((it = std::find_if(it, args.end(), [&name](const std::array<std::string, 2>& arg){ return arg[0] == name;})) != args.end())
    {
        values.push_back((*it)[1]);
        ++it;
    }
    return values;
}

std::vector<std::string> CmdLine::get_args_without_name() const
{
    return args_without_name;
}
