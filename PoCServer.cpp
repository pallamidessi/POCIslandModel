#include <iostream>
#include <cstdlib>
#include <thread>

#include <boost/asio.hpp>

#include "islandServer.h"

int main(int argc, const char *argv[]) {

    std::thread islandThread([]() {
        try {
            boost::asio::io_service io_service;
            evo::island::island_udp_server server(io_service, 1337);
            io_service.run();
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    });
    islandThread.join();
    return 0;
}
