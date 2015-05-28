/*********************************************
    File containing typical mathematical 
    calculations used for performance tests.
 *********************************************/

class PerformanceTest {
protected:
    const int LIMIT;
public:
    PerformanceTest(int lim) :LIMIT(lim) {}
    virtual int run() = 0;
};
 
class FloatPerformanceTest : public PerformanceTest {
/*
    Uses the Nilakantha series to calculate
    an approximation of tau (τ = 2π)
*/
    double tau(int its) {
        double approx = 6;
        
        for (int i = 0; i < its; i++) {
            double m = 2.0 * (i + 1);
            approx += 8.0/(m * (m + 1) * (m + 2));
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

/*
    Iteratively calculates elements from the
    Fibonacci sequence
*/
    unsigned long long int fib(int index) {
        unsigned long long int prev = 1;
        unsigned long long int curr = 1;
        
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
