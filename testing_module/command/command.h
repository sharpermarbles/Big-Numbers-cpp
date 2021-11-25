

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


