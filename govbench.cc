// I/O
#include <iostream>
#include <fstream>
#include <string>

// misc.
#include <vector>
#include <limits>

// stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// cores, sleep
#include <thread>

// chrono
#include <chrono>

// localization
#include "msg_es.h"

// Performance tests
#include "perftests.h"

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::vector;

/**
 * Clase GovernorBenchmark
 * 
 * Contiene los métodos necesarios para alteración
 * del governor y ejecución de una batería de pruebas
 * proporcionada por objetos de PerformanceTest
 *
 */
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
    unsigned int num_cores;
    bool intel_pstate_enabled;
    
    /**
     * check_intel_pstate
     *
     * Busca el directorio de sistema de Intel P-state para 
     * comprobar si está en uso
     */
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
    /**
     * GovernorBenchmark
     *
     * Constructor. Comprueba si Intel P-state está activado
     */
    GovernorBenchmark() 
        :num_cores(std::thread::hardware_concurrency()) {
        check_intel_pstate();
    }
    
    /**
     * set_governor
     * @param name Nombre del governor
     *
     * Modifica el governor en uso por la CPU
     */
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
    
    /**
     * benchmark
     *
     * Método que realiza las ejecuciones de las
     * tareas de PerformanceTest para conformar el
     * benchmark
     */
    int benchmark() {
    /*
        Ejecutamos tests para cada governor:
            * Cálculos largos
            * Cálculos medianos separados por tiempo corto de 'sleep'
            * Cálculos pequeños separados por tiempo largo de 'sleep'
    */
        for (auto& current_gov : AVAIL_GOVERNORS) {
            cerr << Message::USING_GOV(current_gov) << endl;
            
            if (!set_governor(current_gov)) {
                cout << Message::ERROR_SET_GOVERNOR << endl;
                return -1;
            }
            
            int maxlimit = 1000000000;
            
            for (int div = 1; div <= 100; div *= 10) {
                int limit = maxlimit / div;
            
                vector<PerformanceTest*> pruebas;
    /*
                Añadimos un test de enteros y uno de coma flotante
    */
                pruebas.push_back(new IntPerformanceTest(limit));
                pruebas.push_back(new FloatPerformanceTest(limit));
            
                for (auto& test : pruebas) {
                    long long int totalns = 0;
                    
                    for (int nump = 0; nump < div; nump++) {
                        auto start = std::chrono::high_resolution_clock::now();
                        test->run();
                        auto stop = std::chrono::high_resolution_clock::now();
                        
                        totalns += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
                    
                        std::this_thread::sleep_for(std::chrono::milliseconds(div*20));
                    }
                
                    cout << totalns << "," << std::flush;
                }
            }
            
            cout << endl << std::flush;
        }
        
        return 0;
    }
    
    bool using_intel_pstate() {
        return intel_pstate_enabled;
    }
};

/**
 * confirm
 * @param initial Valor por defecto (sí/no)
 *
 * Función que permite pedir al usuario que elija
 * entre sí/no, con una opción activada por defecto
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

/**
 * run_benchmark
 *
 * Se encarga de comprobar lo necesario e interactuar
 * con el usuario para ejecutar el benchmark
 */
int run_benchmark() {
    GovernorBenchmark bench;
    
    if (bench.using_intel_pstate()) {
        cerr << Message::PSTATE_ENABLED;
        
        if (!confirm(false)) {
            cerr << endl << Message::DISABLE_PSTATE << endl;
            return -1;
        }
    }
    
    bench.benchmark();
    
    return 0;
}

/**
 * main
 *
 * Únicamente comprueba que el usuario actual
 * sea root. Ejecuta la función run_benchmark.
 */
int main(int argc, char* argv[]) {
#ifndef DEVELOPMENT
    if (getuid() != 0) {
        cerr << Message::NO_ROOT << endl;
        return -1;
    }
#endif
    
    return run_benchmark();
}

/**
 * Otras posibles tareas:
 *  - Generar varios PerformanceTests en distintas hebras
 *  - Medidas de batería: /sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/BAT1
**/