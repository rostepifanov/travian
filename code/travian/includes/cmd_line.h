#ifndef CMD_LINE_H
#define CMD_LINE_H

#include <string>
#include <vector>
#include <algorithm>
#include <array>

class CmdLine {
    public:
        CmdLine() {}

        /*!
         * \brief разбирает аргументы команды
         * \note разбираются команды вида -name или -name=value, остальные игнорируются
         * \param line строка с командой
         */
        CmdLine(const std::string& line);

        /*!
         * \brief проверяет присутствует ли именованный параметр
         * \param name именнованный параметр
         */
        bool has(const std::string& name) const;

        /*!
         * \brief возвращает значение именнованного параметра
         * \note если значение параметра на специфицирорванно возращает пустую строку.
         *       если значений несколько возвращает первое найденное значение.
         * \param name именнованный параметр
         */
        std::string get(const std::string& name) const;

        /*!
         * \brief возвращает все значения именнованного параметра
         * \param name именнованный параметр
         */
        std::vector<std::string> get_multiple(const std::string& name) const;

        /*!
         * \brief возвращает имя команды (первый аргумент строки до пробела)
         */
        std::string get_cmd() const;

        /*!
         * \brief возвращает неименнованные параметры
         */
        std::vector<std::string> get_args_without_name() const;

    private:
        std::string cmd;
        typedef std::array<std::string, 2> named_arg;
        std::vector<named_arg> args;
        std::vector<std::string> args_without_name;
};

#endif // CMD_LINE_H
