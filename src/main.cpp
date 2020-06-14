#include "daemon.h"
#include "config.h"
#include <iostream>

#include "serialport.h"
#include "cli.h"

int main(int argc, const char* argv[]) {
//  daemonize("waked");
    Config config;
    std::cout << config.getTtyPath() << " " << config.getBaudRate() << std::endl;
    SerialPort port{config.getTtyPath(), config.getBaudRate()};
    Wk::Wake wake{port};
    if(wake.OpenConnection()) {

    }
    return EXIT_SUCCESS;
}
