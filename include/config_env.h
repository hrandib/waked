#ifndef CONFIG_ENV_H
#define CONFIG_ENV_H

#include <cstdlib>
#include <string_view>

#include "config_ini.h"
#include <iostream>

class ConfigEnv
{
    using cs = const char* const;

    static inline cs ENV_CONFIG_VAR = "WAKE_CONFIG_PATH";
    static inline cs ENV_TTY_VAR = "WAKE_TTY";
    static inline cs ENV_BAUDRATE_VAR = "WAKE_BAUDRATE";

    uint32_t baud_{};
    std::string tty_;
public:
    ConfigEnv()
    {
        cs config = std::getenv(ENV_CONFIG_VAR);
        cs tty = std::getenv(ENV_TTY_VAR);
        cs baud = std::getenv(ENV_BAUDRATE_VAR);
        if(tty && baud) {
            tty_ = tty;
            baud_ = std::atoi(baud);
        }
        else if(config) {
            ConfigIni conf{config};
            tty_ = conf.getTtyPath();
            baud_ = conf.getBaudRate();
        }
        else {
            ConfigIni conf{DEFAULT_CONFIG_PATH};
            tty_ = conf.getTtyPath();
            baud_ = conf.getBaudRate();
        }
    }

    std::string getTtyPath() const
    {
        return tty_;
    }
    uint32_t getBaudRate() const
    {
        return baud_;
    }
};

#endif // CONFIG_ENV_H
