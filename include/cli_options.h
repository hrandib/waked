#pragma once

#include "option_parser.h"
#include "wsp32.h"

namespace Wk {

using std::cerr;
using std::endl;
using std::exception;
using std::string;
using std::vector;

class CliOpts : Opts::Parser
{
private:
    static constexpr uint8_t ADDR_MAX = 127;
    vector<uint8_t> addressGroup_{};
    vector<uint8_t> payload_{};
    uint8_t command_{};
public:
    CliOpts(int argc, const char* argv[]) : Parser{argc, argv}
    {
        int result{};
        vector<string> keyValues;
        // Parse single or multi address
        tie(result, keyValues) = FindUnsized("-a");
        if(result >= 0 && !keyValues.empty()) {
            try {
                addressGroup_ = ConvertToNumbers(keyValues);
                if(any_of(addressGroup_.begin(), addressGroup_.end(), [](uint8_t addr) { return addr > ADDR_MAX; })) {
                    cerr << "Address should be in range 0..127" << endl;
                    exit(1);
                }
            }
            catch(exception& e) {
                cerr << "Address option (-a) param(s) are not valid. " << e.what() << endl;
                exit(1);
            }
        }
        else {
            cerr << "Address option (-a) is not set or address parameter not provided" << endl;
            exit(1);
        }
        // Parse Command
        tie(result, keyValues) = Find("-c", 1, 0);
        if(result >= 0 && !keyValues.empty()) {
            try {
                command_ = static_cast<uint8_t>(stoi(keyValues[0]));
            }
            catch(std::exception& e) {
                cerr << "Command is not valid. " << e.what() << endl;
                exit(1);
            }
            if(command_ > ADDR_MAX) {
                cerr << "Command should be in range 0..127" << endl;
                exit(1);
            }
        }
        else {
            cerr << "Command option (-c) is not set or command parameter not provided" << endl;
            exit(1);
        }
        tie(result, keyValues) = Find("-s", 1, 1);
        int strPayloadPos{};
        if(result >= 0) {
            strPayloadPos = result;
            if(!keyValues.empty()) {
                auto& payloadStr = keyValues[0];
                payload_.insert(payload_.end(), payloadStr.cbegin(), payloadStr.cend());
            }
            else {
                cerr << "Payload string (-s) should not be empty" << endl;
                exit(1);
            }
        }
        tie(result, keyValues) = FindUnsized("-d");
        int dataPayloadPos{};
        // If -d not found
        if(result < 0) {
            // Payload (unnamed args at the tail)
            keyValues = GetTail();
            dataPayloadPos = strPayloadPos + 1;
        }
        else if(keyValues.empty()) {
            cerr << "Payload (-d) should not be empty" << endl;
            exit(1);
        }
        else {
            dataPayloadPos = result;
        }
        if(!keyValues.empty()) {
            try {
                auto payloadData = ConvertToNumbers(keyValues);
                const auto& placeTo = strPayloadPos < dataPayloadPos ? payload_.end() : payload_.begin();
                payload_.insert(placeTo, payloadData.cbegin(), payloadData.cend());
            }
            catch(exception& e) {
                cerr << "Payload must contain only digits with one byte size. " << e.what() << endl;
            }
        }
// Print result
#ifndef NDEBUG
        cout << "Address(es): ";
        for(auto addr : addressGroup_) {
            cout << (size_t)addr << ' ';
        }
        cout << endl;
        cout << "Command: " << (size_t)command_ << endl;
        cout << "Payload: ";
        if(payload_.size()) {
            for(auto elem : payload_) {
                cout << (size_t)elem << ' ';
            }
        }
        else
            cout << "No";
        cout << endl;
#endif
    }
    void GetParams(std::vector<uint8_t>& addressGroup, Packet_t& params)
    {
        addressGroup = addressGroup_;
        params.addr = addressGroup[0];
        params.cmd = command_;
        params.n = (uint8_t)payload_.size();
        copy(payload_.begin(), payload_.end(), params.payload.begin());
    }
};
} // Wk
