#include "cli.h"
#include "config_env.h"
#include "serialport.h"
#include <iostream>

using namespace Wk;

int main(int argc, const char* argv[])
{
    ConfigEnv config;
    std::cout << config.getTtyPath() << " " << config.getBaudRate() << std::endl;
    SerialPort serialPort{config.getTtyPath(), config.getBaudRate()};
    Wake wake{serialPort};
    if(argc > 1) {
        CliOpts options{argc, argv};
        Cli cli{wake, options};
        if(cli.OpenConnection()) {
            cli.Process();
        }
    }
    return EXIT_SUCCESS;
}
