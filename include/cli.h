#pragma once

#include <vector>
#include "wsp32.h"
#include "cli_options.h"

namespace Wk {

    class Cli
    {
    private:
        constexpr static uint32_t TIMEOUT = 50;
        Packet_t params_;
        std::vector<uint8_t> addressGroup_;
        Wake wake_;
        void PrintRaw(const Packet_t& params)
        {
            using namespace std;
            cout << setw(4) << (uint32_t)params.addr
                << setw(4) << (uint32_t)params.cmd
                << setw(4) << (uint32_t)params.n;
            for(uint8_t i = 0; i != params.n; ++i)
            {
                cout << setw(4) << (uint32_t)params.payload[i];
            }
            cout << endl;
        }
        void PrintOperatingTime(uint8_t* data)
        {
            uint32_t uptime = ((*(uint32_t*)data & 0xFFFFFF) * 10); //In minutes
            uint32_t years = uptime / (365 * 24 * 60);
            uptime -= years * 365 * 24 * 60;
            uint32_t months = uptime / (30 * 24 * 60);
            uptime -= months * 30 * 24 * 60;
            uint32_t days = uptime / (24 * 60);
            uptime -= days * 24 * 60;
            uint32_t hours = uptime / 60;
            uint32_t minutes = uptime - hours * 60;
            cout << "Operating Time: "
                << years << " years " << months << " months " << days << " days "
                << hours << " hours " << minutes << " minutes" << endl;
        }
    public:
        Cli(CliOpts& options) : wake_{options.GetPort(), options.GetBaudRate()}
        {
            options.GetParams(addressGroup_, params_);
        }
        bool OpenConnection()
        {
            for(uint32_t n = 0; n < 10; ++n) {
                if(wake_.OpenConnection()) {
                    return true;
                }
                else {
                    Sleep(TIMEOUT);
                }
            }
            std::cerr << "Port is busy (or absent)";
            return false;
        }
        static bool IsMulticast(size_t addr)
        {
            return addr > 79 && addr < 96;
        }
        void Process()
        {
            using std::setw;
            for(auto addr : addressGroup_) {
                Packet_t paramsTemp = params_;
                paramsTemp.addr = addr;
                if(C_GETINFO == paramsTemp.cmd) {
                    cout << ">>> Device address: " << (size_t)addr << "\r\n";
                    wake_.GetInfo(paramsTemp);
                    continue;
                }
                else if(IsMulticast(paramsTemp.addr)) {
                    wake_.Request(paramsTemp, 10);
                    continue;
                }
                else if(!wake_.Request(paramsTemp)) {
                    std::cerr << "!!! Request failed, address: " << (size_t)paramsTemp.addr << endl;
                    continue;
                }
                if(paramsTemp.payload[0] && paramsTemp.cmd > C_ECHO) {
                    cout << "!!! Error: " << GetErrorString((Err)paramsTemp.payload[0]) << "\r\n";
                    return;
                }
                switch(paramsTemp.cmd) {
                case C_GETOPTIME:
                    PrintOperatingTime(&paramsTemp.payload[1]);
                    break;
                case C_SETGROUPADDRESS:
                    cout << "Group address: " << (size_t)paramsTemp.payload[1] << "\r\n";
                    break;
                default:
                    PrintRaw(paramsTemp);
                }
            }
        }
    };

}//WakeCli

