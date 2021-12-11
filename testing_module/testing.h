



#ifndef _testing_h_
#define _testing_h_

#include <string>
#include <iostream>
#include "bgnm.hpp"


class Testing
{

public:

    Testing();
	~Testing();
    
    static void random_tester(int number = 20, bool show_all = true);
    static void test(const std::string & operation, std::string constants, int number = 20, bool show_all = true );
    static void demo ();
    static void exceptions ();

};

#endif


