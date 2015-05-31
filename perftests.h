/**
 * PerformanceTest
 *
 * Clase abstracta definiendo la interfaz de
 * una batería de pruebas
 */
class PerformanceTest {
protected:
    const int LIMIT;
public:
    PerformanceTest(int lim) :LIMIT(lim) {}
    virtual int run() = 0;
};

/**
 * FloatPerformanceTest
 *
 * Clase que permite probar el rendimiento mediante
 * tareas de cálculo en coma flotante
 */
class FloatPerformanceTest : public PerformanceTest {
    /**
     * tau
     * @param its Número de iteraciones
     *
     * Calcula una aproximación al número Tau (~ 6.28)
     */
    double tau(int its) {
    /*
        Uses the Leibniz alternating series to calculate
        an approximation of tau (τ = 2π)
    */
        double approx = 0;
        
        for (int i = 0; i < its; i += 2) {
            approx += 8.0/(2.0 * i + 1.0);
            approx -= 8.0/(2.0 * (i + 1) + 1.0);
        }
        
        return approx;
    }
public:
    FloatPerformanceTest(int lim) :PerformanceTest(lim) {}
    
    int run() {
        double mytau = tau(LIMIT);
        return 0;
    }
};

class IntPerformanceTest : public PerformanceTest {
private:
    /**
     * fib
     * @param index Índice del elemento en la secuencia 
     * a calcular
     *
     * Calcula el elemento indicado de la sucesión de Fibonacci
     */
    unsigned long int fib(int index) {
    /*
        Iteratively calculates elements from the
        Fibonacci sequence
    */
        unsigned long int prev = 1;
        unsigned long int curr = 1;
        
        for (int i = 0; i < index; i++) {
            curr = curr + prev;
            prev = curr - prev; // Previous curr value
        }
        
        return curr;
    }
public:
    IntPerformanceTest(int lim) :PerformanceTest(lim) {}
    
    int run() {
        unsigned long long int* myfib = new unsigned long long int;
        *myfib = fib(LIMIT);
        return 0;
    }
};
