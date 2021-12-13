



#ifndef _testing_h_
#define _testing_h_

#include <string>
#include <iostream>
#include "bgnm.hpp"
#include "testing_timer.hpp"

class Testing
{
    static double error_threshhold;

public:

    Testing();
	~Testing();
    
    static void      random_tester(int number = 20, bool show_all = true, bool timer = false);
    static void      test(const std::string & operation, std::string constants, int number = 20, bool show_all = true, bool timer = false );
    static void      demo ();
    static void      exceptions ();
    
    static void      set_error_threshhold(const double threshhold);
    static double    get_error_threshhold();
    
    static void      set_bgnm_internal_precision_limit(unsigned precision);
    static unsigned  get_bgnm_internal_precision_limit();
    static      void set_bgnm_max_root_guess_count(unsigned count);
    static  unsigned get_bgnm_max_root_guess_count();
    static      void set_bgnm_root_index_max_precision(unsigned count);
    static  unsigned get_bgnm_root_index_max_precision();

};

#endif


