
//How to add a custom command

    // command.h
    // 1. add function declaration to command.h file
    // 2. create member(s) of Command class which will hold pointer(s) to custom project class(es)

    // command.cpp
    // 2. add name of command to commands[] array
    // 3. add case to eval() function (be sure case int matches index location in commands[] array)
    // 4. write function to be triggered by command
    // 5. add command and description to help_text (in the Command constructor)


#ifndef _command_h_
#define _command_h_

#include <string>
#include <iostream>

class Command
{
	static const std::string commands[];
	std::string help_text;
	int keep_going = true;
	void eval(int);
    
    // member variables to hold pointer to custom project class object
	//SampleClass * sc;
    
    // built in commands
    void help();
    void exit_program();

	// custom commands here
	void do_stuff_1();
    void do_stuff_2();
    void do_stuff_3();

public:
    
	//Command(SampleClass *);
    Command();
	~Command();
	void get();

};

#endif


