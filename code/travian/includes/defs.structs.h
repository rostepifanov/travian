#ifndef DEFS_STRUCTS_H
#define DEFS_STRUCTS_H

#include <array>
#include <algorithm>

namespace defs
{

    struct keys
    {
        std::string server;
        std::string login;
        std::string password;

        keys(const std::string& server, const std::string& login, const std::string& password) :
            server(server), login(login), password(password) { }
    };

    template <typename type, unsigned size>
    class vector : public std::array<type, size>
    {
    public:
        vector<type, size> ( ) { std::fill(this->begin(), this->end(), 0); }

        const defs::vector<type, size>& operator = (const defs::vector<type, size>& right)
        {
            std::copy(right.begin(), right.end(), this->begin());
            return *this;
        }

        bool operator <= (const defs::vector<type, size>& right) const
        {
            return std::equal(this->begin(), this->end(), right.begin(),
                       [](const type& left, const type& right) { return left <= right; });
        }

        bool operator == (const defs::vector<type, size>& right) const
        {
            return std::equal(this->begin(), this->end(), right.begin(),
                       [](const type& left, const type& right) { return left == right; });
        }
    };

    template <unsigned size>
    using uvector = vector<unsigned, size>;

    template <unsigned size>
    using dvector = vector<double, size>;


}

#endif // DEFS_STRUCTS_H
