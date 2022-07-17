/*
command.hpp

Copyright © 2021 Ben Harvey.
Created by Ben Harvey on 4/11/21.
 
This file is part of Command Parse C++ Library

Command Parse C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Command Parse C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Command C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _command_h_
#define _command_h_

#include <string>
#include <iostream>
#include <vector>
#include "com.h"


class Command
{
    static std::string welcome_msg;
    static std::string exit_msg;
    
    static std::vector<Com*> coms;
	static bool keep_going;
    static void load(std::vector<Com*> * const);
    static bool parser( Com const * const com, Parsed* const parsed, std::string input_string, std::string & err_msg);
    
    static void help( Parsed const * const = NULL);
    static void exit( Parsed const * const = NULL);
    
    Command();
    ~Command();
    
    static void set_keep_going(bool);
    static void set_welcome_msg(std::string &);
    static void set_exit_msg(std::string &);
    static int  get_command_count();
    static Com* get_com_ptr(const int);

public:
    
	static void main_input_loop();

};

#endif


