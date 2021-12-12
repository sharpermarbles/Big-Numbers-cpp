// Timer class for testing purposes
// Just create a block of code and create an object of type Timer at the begining of the block. When the block completes, the destructor stops the timer and prints the elapsed time.

// Optional to provide to the Timer constructor
//  - Name or Description of block of code or function being timed
//  - Int number of loop iterations being timed (if applicable)

// Timer t ("my_function()",10000);


#ifndef testing_timer_hpp
#define testing_timer_hpp

#include <chrono>
#include <string>

#define FUNCTION "The designated block of code"

class Timer
{
public:
    Timer(const std::string_view func, int iter)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = func;
        iterations = iter;
    }
    Timer(const std::string_view func)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = func;
        iterations = 1;
    }
    Timer(int iter)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = FUNCTION;
        iterations = iter;
    }
    Timer()
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = FUNCTION;
        iterations = 1;
    }
    ~Timer()
    {
        Stop();
    }
private:
    void Stop()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();
        auto howlong = end - start;
        long double ms = howlong * 0.001;
        if(iterations == 1)
        {
            std::cout << function << " took " << ms << " milliseconds to complete.\n";
        }
        else
        {
            std::cout << function << " took " << ms << " milliseconds to complete " << iterations << " iterations.\n";
        }
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::string function;
    int iterations;
};

#endif /* testing_timer_hpp */
