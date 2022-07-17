/*
main.cpp Entry point for testing module of Big Numbers C++ Library
 
Created by Ben Harvey on 10/10/20.
Copyright © 2020 Ben Harvey.
 
This file is part of Big Numbers C++ Library

Big Numbers C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Big Numbers C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Big Numbers C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "command.h"

//big_nums library
#include "bgnm.hpp"

int main()
{

    srand ((unsigned int)time(NULL)); // seed random
    
    try
    {
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
