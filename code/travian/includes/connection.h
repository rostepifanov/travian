#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <curl/curl.h>

class Connection {
    const std::string cookie = "cookie";
    const std::string user_agent = "Mozilla/5.0 "
                                   "(X11; Linux x86_64) "
                                   "AppleWebKit/537.36 "
                                   "(KHTML, like Gecko) "
                                   "Chrome/62.0.3202.94 "
                                   "Safari/537.36";

    CURL* handle;

    size_t request_count = 0;

    static size_t callback(char *data, size_t size, size_t nmemb, std::string *buffer);


    /**
     * \brief установки cookie для соединения
     * \param имя cookie
     */
    void set_cookie();

public:
    Connection();

    /**
     * \brief отправка запроса по url и получение html страницы
     * \param имя url
     * \param тип запроса
     * \param аргументы запроса
     */
    std::string get_data(const std::string& url, const std::string& method, const std::string& args);

    ~Connection();
};


#endif // CONNECTION_H
