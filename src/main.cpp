#include "daemon.h"
#include "config.h"
#include <iostream>

int main(int argc, const char* argv[]) {
//  daemonize("waked");
    Config config;
    std::cout << config.getTtyPath() << " " << config.getBaudRate() << std::endl;
//    while(true) {
//        syslog(LOG_NOTICE, "First daemon started.");
//        sleep(30);
//        break;
//    }
    return EXIT_SUCCESS;
}
