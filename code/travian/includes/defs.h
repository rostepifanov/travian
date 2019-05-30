#ifndef DEF_H
#define DEF_H

#include <string>
#include <algorithm>
#include <vector>
#include <sstream>

#include "gumbo/Document.h"
#include "gumbo/Node.h"
#include "gumbo/Object.h"
#include "gumbo/Parser.h"
#include "gumbo/QueryUtil.h"
#include "gumbo/Selection.h"
#include "gumbo/Selector.h"


namespace defs
{
    /*!
     * \brief удаление повторяющихся делиметеров
     * \param str строка для очистки
     * \param delim делимитр для удаления
     */
    inline void clear(std::string& str, char delim = ' ')
    {
        std::string::size_type drop = 0;
        while (str[drop] == delim)
          ++drop;

        str = str.substr(drop, std::string::npos);

        std::string::iterator new_end = std::unique(str.begin(), str.end(),
            [delim] (const char &x, const char &y)
            {
                return x == y && x == delim;
            });

        str.erase(new_end, str.end());

//        drop = str.size() - 1;
//        while (str[drop] == delim)
//          --drop;

//        str.erase(str.begin() + drop + 1, str.end());
    }

    /*!
     * \brief разбиение строки на подстроки
     * \param str строка для разбиния
     * \param delim разделительный символ
     * \return подстроки строки
     */
    inline std::vector<std::string> split(std::string str, char delim = ' ')
    {
        defs::clear(str, delim);

        std::vector<std::string> splits;
        std::stringstream stream(str);

        std::string::iterator new_end = std::unique(str.begin(), str.end(),
            [delim] (const char &x, const char &y)
            {
                return x == y && x == delim;
            });

        str.erase(new_end, str.end());

        while (str[str.length() - 1] == delim) str.pop_back();

        size_t i = 0;
        size_t pos = str.find(delim);

        while (pos != std::string::npos)
        {
            splits.push_back(str.substr(i, pos - i));

            i = pos + 1;
            pos = str.find(delim, i);
        }

        splits.push_back(str.substr(i, std::min(pos, str.length()) - i + 1));
        return std::move(splits);
    }

    /*!
     * \brief Проверяет является ли строка целым числом
     * \param str строка для проверки
     */
    inline bool is_integer(const std::string& str)
    {
        return (std::isdigit(str[0]) || str[0] == '-') && std::all_of(str.begin() + 1, str.end(), [](char c){return std::isdigit(c);});
    }

    /*!
     * \brief Проверяет является ли строка дробным числом
     * \param str строка для проверки
     */
    inline bool is_double(const std::string& str)
    {
        return (std::isdigit(str[0]) || str[0] == '-') && std::all_of(str.begin() + 1, str.end(), [](char c){return std::isdigit(c) || c == '.';});
    }
}



#endif // DEF_H
