#pragma once

#include "option_parser.h"
#include "wsp32.h"

namespace Wk {

    using std::exception;
    using std::string;
    using std::vector;
    using std::cout;
    using std::cerr;
    using std::endl;

    class CliOpts : Opts::Parser
    {
    private:
        vector<uint8_t> addressGroup_{};
        vector<uint8_t> payload_{};
        uint8_t command_{};
    public:
        CliOpts(int argc, const char* argv[]) : Parser{argc, argv}
        {
            int result;
            vector<string> keyValues;
            // Parse single or multi address
            tie(result, keyValues) = FindUnsized("-a");
            if(result >= 0 && keyValues.size() > 0) {
                try {
                    addressGroup_ = ConvertToNumbers(keyValues);
                    if(any_of(addressGroup_.begin(), addressGroup_.end(), [](uint8_t addr) {
                        return addr > 127;
                    })) {
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
            if(result >= 0 && keyValues.size() > 0) {
                try {
                    command_ = static_cast<uint8_t>(stoi(keyValues[0]));
                }
                catch(std::exception& e) {
                    cerr << "Command is not valid. " << e.what() << endl;
                    exit(1);
                }
                if(command_ > 127) {
                    cerr << "Command should be in range 0..127" << endl;
                    exit(1);
                }
            }
            else {
                cerr << "Command option (-c) is not set or command parameter not provided" << endl;
                exit(1);
            }
            tie(result, keyValues) = FindUnsized("-d");
            // If -d not found
            if(result < 0) {
                // Payload (unnamed args at the tail)
                keyValues = GetTail();
            }
            else if(!keyValues.size()) {
                cerr << "Payload (-d) should not be empty" << endl;
                exit(1);
            }
            if(keyValues.size()) {
                try {
                    payload_ = ConvertToNumbers(keyValues);
                }
                catch(exception& e) {
                    cerr << "Payload must contain only digits with one byte size. " << e.what() << endl;
                }
            }
            //Print result
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
            else cout << "No";
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
}//Wk
