/*
 * Copyright (c) 2020 Dmytro Shestakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "config.h"
#include "inipp.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <string>
#include <syslog.h>
#include <unistd.h>

using namespace std::literals::string_literals;
using std::string;
using std::to_string;
namespace fs = std::filesystem;

class Config::IniChar : public inipp::Ini<char>
{
    using Ini::Ini;
};

static fs::path getHome()
{
    const char* homedir;
    if((homedir = getenv("HOME")) == nullptr) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    return homedir;
}

Config::Config(std::string_view configPath)
{
    if(configPath.size()) {
        ini_ = std::make_unique<IniChar>();
        std::ifstream is(getHome() / configPath);
        if(!is) {
            std::cerr << "Config not found" << std::endl;
        }
        ini_->parse(is);
    }
    else {
        std::cerr << "Config path is empty" << std::endl;
        // syslog(LOG_ERR, "Config path is empty");
    }
}

const auto& Config::getSerial()
{
    return ini_->sections["Serial"];
}

std::string Config::getTtyPath()
{
    return getSerial().at("tty");
}

uint32_t Config::getBaudRate()
{
    uint32_t baud = 9600;
    if(!inipp::extract(getSerial().at("baud"), baud)) {
        std::cerr << "No valid value for baud, defaults to 9600\r\n";
    }
    return baud;
}

Config::~Config() = default;
