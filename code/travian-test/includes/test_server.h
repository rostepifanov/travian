#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include "server.h"

class TestServer : public Server {
    public:
        Server() { }
        void update_page(const std::string& suffix) override;
};

#endif // TEST_SERVER_H
