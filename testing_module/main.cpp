
//  main.cpp
//
//  Created by Ben Harvey on 1/6/20.
//  Copyright © 2020 Ben Harvey. All rights reserved.

//  main() function for testing and playing with bgnm library


#include <iostream>
#include "command.h"

//big_nums library
#include "bgnm.hpp"


int main()
{
    
    srand ((unsigned int)time(NULL));
    
    try
    {
        // basic program user input loop
        Command::main_input_loop();
    }
    catch (char const * c)
    {
        std::cout << "Exception thrown: " << c << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Exception thrown.\n";
    }
}
