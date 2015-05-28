// I/O
#include <iostream>
#include <fstream>
#include <string>

#include <limits>

// stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// cores
#include <thread>

// localization
#include "msg_es.h"

// Performance tests
#include "perftests.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;

class GovernorBenchmark {
private:
/*  
    Parameters for the benchmark
*/
    const string INTELPSTATE_PATH   = "/sys/devices/system/cpu/intel_pstate";
    const string GOVERNOR_PATH[2]   = {"/sys/devices/system/cpu/cpu", "/cpufreq/scaling_governor"};
    const string AVAIL_GOVERNORS[4] = {"performance", "ondemand", "conservative", "powersave"};

/*
    Other private members
*/
    bool intel_pstate_enabled;
    unsigned int num_cores;
    
    void check_intel_pstate() {
    /*
        Intel P-state is being used if the directory pointed
        by INTELPSTATE_PATH exists
    */
        struct stat st;
        stat(INTELPSTATE_PATH.c_str(), &st);
        
        intel_pstate_enabled = S_ISDIR(st.st_mode);
    }

public:
    GovernorBenchmark() 
        :num_cores(std::thread::hardware_concurrency()) {
        check_intel_pstate();
    }
    
    bool set_governor(string name) {
        bool success = true;
        
        for (unsigned core = 0; core < num_cores && success; ++core) {
            std::fstream gov_file(GOVERNOR_PATH[0] + (char)('0' + core) + GOVERNOR_PATH[1],
                std::fstream::in | std::fstream::out);
            
            success = (gov_file << name) ? true : false;
            gov_file.close();
        }
        
        return success;
    }
    
    int benchmark() {
        for (auto& current_gov : AVAIL_GOVERNORS) {
            cerr << Message::USING_GOV(current_gov) << endl;
            
            if (!set_governor(current_gov)) {
                cout << Message::ERROR_SET_GOVERNOR << endl;
                return -1;
            }
            
            int limit = 1000000000;
            
            IntPerformanceTest pruebasint(limit);
            //pruebasint.run();
            
            FloatPerformanceTest pruebasfloat(limit);
            pruebasfloat.run();
        }
        
        return 0;
    }
    
    bool using_intel_pstate() {
        return intel_pstate_enabled;
    }
    
    unsigned int cores() {
        return num_cores;
    } 
};

/*
Helper function for user interaction
*/
bool confirm(bool initial) {
    char confirmation;
    bool correct_input = false;
    
    while (!correct_input) {
        cerr << (initial ? Message::CONFIRM_Y : Message::CONFIRM_N);

        if (cin.get(confirmation) && confirmation != '\n') {
            confirmation = tolower(confirmation);
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            confirmation = initial ? Message::CHAR_Y : Message::CHAR_N;
        }

        correct_input = confirmation == Message::CHAR_Y || confirmation == Message::CHAR_N;
    }
    
    return confirmation == Message::CHAR_Y;
}

int run_benchmark() {
    GovernorBenchmark bench;
    
    if (bench.using_intel_pstate()) {
        cerr << Message::PSTATE_ENABLED;
        
        if (!confirm(false)) {
            cerr << endl << Message::DISABLE_PSTATE << endl;
            return -1;
        }
    }
    else {
    }
    
    bench.benchmark();
    
    return 0;
}

int main(int argc, char* argv[]) {
#ifndef DEVELOPMENT
    if (getuid() != 0) {
        cerr << Message::NO_ROOT << endl;
        return -1;
    }
#endif
    
    return run_benchmark();
}