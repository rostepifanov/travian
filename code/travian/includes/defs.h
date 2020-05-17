#ifndef DEF_H
#define DEF_H

#include <algorithm>
#include <array>
#include <vector>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <regex>

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
    inline void clear(std::string& str, char delim = ' ') {
        std::string::size_type drop = 0;
        while (str[drop] == delim) {
          ++drop;
        }

        drop = std::min(std::string::size_type(0), drop - 1);

        str = str.substr(drop, std::string::npos);

        std::string::iterator new_end = std::unique(str.begin(), str.end(),
            [delim] (const char &x, const char &y)
            {
                return x == y && x == delim;
            });

        str.erase(new_end, str.end());
    }

    /*!
     * \brief разбиение строки на подстроки
     * \param str строка для разбиния
     * \param delim разделительный символ
     * \return подстроки строки
     */
    inline std::vector<std::string> split(std::string str, char delim = ' ') {
        defs::clear(str, delim);

        std::vector<std::string> splits;

        std::string::size_type drop = 0;
        while (str[drop] == delim) {
          ++drop;
        }

        str = str.substr(drop, std::string::npos);

        while (str[str.length() - 1] == delim) str.pop_back();

        size_t i = 0;
        size_t pos = str.find(delim);

        while (pos != std::string::npos) {
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
    inline bool is_integer(const std::string& str) {
        for (int i = 0; i < str.size(); ++i) {
            char cur = str[i];
            if ( ! (std::isdigit(cur) || (cur == '-' && i == 0) ) )
              return false;
        }

        return true && ! str.empty();
    }
}

#endif // DEF_H
