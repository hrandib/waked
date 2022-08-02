#include "config.h"
#include <iostream>

#include "cli.h"
#include "serialport.h"

using namespace Wk;

int main(int argc, const char* argv[])
{
    Config config;
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
