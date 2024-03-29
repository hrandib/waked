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
#ifndef CONFIG_INI_H
#define CONFIG_INI_H

#include <memory>
#include <string_view>

constexpr std::string_view DEFAULT_CONFIG_PATH = ".config/waked/conf.ini";
static constexpr uint32_t DEFAULT_BAUDRATE = 9600;

class ConfigIni
{
private:
    using sv = std::string_view;
    class IniChar;
    std::unique_ptr<IniChar> ini_;
    const auto& getSerial();
public:
    explicit ConfigIni(sv configPath);
    std::string getTtyPath();
    uint32_t getBaudRate();
    ~ConfigIni();
};

#endif // CONFIG_INI_H
