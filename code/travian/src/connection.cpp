#include "connection.h"

connection::connection()
{
    if((handle = curl_easy_init()) == nullptr)
        throw std::exception();

    set_cookie();

    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, callback);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback);

    curl_easy_setopt(handle, CURLOPT_HEADER, false);
    curl_easy_setopt(handle, CURLOPT_NOBODY, false);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, user_agent.c_str());
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 60);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
}

connection::~connection()
{
    curl_easy_cleanup(handle);
}

void connection::set_cookie()
{
    curl_easy_setopt(handle, CURLOPT_COOKIEJAR, cookie.c_str());
    curl_easy_setopt(handle, CURLOPT_COOKIEFILE, cookie.c_str());
}

std::string connection::get_data(const std::string& url, const std::string& method, const std::string& arg)
{
    std::string request = url;

    if(method == "POST")
    {
        curl_easy_setopt(handle, CURLOPT_POST, 1);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, arg.c_str());
    }
    else if (method == "GET")
    {
        curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
        request += !arg.empty() ? "?"+arg : "";
    }
    else return std::string();

    curl_easy_setopt(handle, CURLOPT_REFERER, request.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, request.c_str());

    std::string head;
    std::string body;
    curl_easy_setopt(handle, CURLOPT_HEADERDATA, &head);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &body);
    curl_easy_perform(handle);

    ++request_count;

    return body;
}

size_t connection::callback(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    int result = size * nmemb;
    if(buffer)
        buffer->append(data, size * nmemb);
    return result;
}
