#ifndef SERVER_H
#define SERVER_H


#include "connection.h"
#include "defs.structs.h"
#include "defs.h"

class LoginException : public std::exception
{
    std::string msg;
public:
    LoginException(const defs::Keys& info) : msg("Incorrect login-password " + info.login + " " + info.password) { }

    const char * what() const throw() { return msg.c_str(); }
};


class  Server {
    public:
        Server(const defs::Keys& info);

        void update_page(const std::string& suffix);
        std::string* get_raw(void);
        CDocument* get_parsed(void);

    private:
        const std::string login = "login.php";
        const std::string checkup = "dorf1.php";

        Connection con;
        std::string url;

        CDocument html;
        std::string page;

        void __login(const defs::Keys& info);
        void __login_checkup(const defs::Keys& info);
};

#endif // SERVER_H
