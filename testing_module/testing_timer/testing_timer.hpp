// The Timer class in this file generally follows the concept put forth by "The Cherno" YouTube channel in the video titled "Timing in C++" (https://www.youtube.com/watch?v=oEx5vGNFrLk). The implementation has been developed a little differently here.

// Timer class for testing purposes
// Just create a block of code and create an object of type Timer at the begining of the block. When the block completes (if auto_report is true), the destructor stops the timer and prints the elapsed time. Otherwise use the stop() and report() functions at the end of the code block to get the time ellapsed.

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
private:
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::string function;
    unsigned iterations;
    bool auto_report;
    std::string time_txt;
    
    void Done()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();
        auto howlong = end - start;
        auto ms = howlong * 0.001;
        time_txt = "" + std::to_string(ms) + " milliseconds";
        if(iterations == 1 && auto_report)
        {
            std::cout << function << " took " << time_txt << " to complete.\n";
        }
        else if (auto_report)
        {
            std::cout << function << " took " << time_txt << " to complete " << iterations << " iterations.\n";
        }
    }
    
public:
    Timer(const std::string_view func, int iter, bool aut = false)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = func;
        iterations = iter;
        auto_report = aut;
    }
    Timer(const std::string_view func, bool aut = false)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = func;
        iterations = 1;
        auto_report = aut;
    }
    Timer(int iter, bool aut = false)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = FUNCTION;
        iterations = iter;
        auto_report = aut;
    }
    Timer( bool aut = false)
    {
        start_time = std::chrono::high_resolution_clock::now();
        function = FUNCTION;
        iterations = 1;
        auto_report = aut;
    }
    ~Timer()
    {
        Done();
    }
    void stop()
    {
        Done();
    }
    std::string report()
    {
        Done();
        return time_txt;
    }
};

#endif /* testing_timer_hpp */

