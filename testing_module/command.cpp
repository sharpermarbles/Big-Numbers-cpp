
#include "command.h"
#include <ctime> // for seeding rand() function

static uint rand_int(int flr, int ceil)
{
    int mod = ceil - flr;
    if (mod < 0) throw "rand_int function given bad parameters.\n";
    return (rand() % mod) + flr;
}

const std::string Command::commands[] = {
    // built in commands
    "help", "exit",
    
    //custom commands - must be entered here exactly as user should enter command
	"func1", "func2", "func3", "random"
};

// pointers to custom classes should be passed to the Command constructor
Command::Command()
{
    
    //make sure rand() function is seeded
    std::srand((uint)std::time(0));
    
    
    // set member pointer equal to pointer to custom class
    //sc = _sc;

    
    std::string s = "";
    s += "    help          - List all possible user commands.\n";
    s += "    exit          - Terminate program.\n";
    
    // edit list below to include description/help for each custom function
    s += "    func1         - calls Command::do_stuff_1() which calls SampleClass member function_1()\n";
    s += "    func2         - calls Command::do_stuff_2() which calls SampleClass member function_2(std::string)\n";
    s += "    func3         - calls Command::do_stuff_3() which calls SampleClass member function_3()\n";
    s += "    random        - performs random tests on Bgnm class objects and methods.\n";
    help_text = s;
}

Command::~Command()
{
}

void Command::eval(int c)
{
    switch (c)
    {
            // built-in functions
        case 0:
            help(); //provides list of possible user commands
            break;
        case 1:
            exit_program(); //terminate program
            break;
            
            // custom functions (case number MUST MATCH INDEX of command in commands[] array above
        case 2:
            do_stuff_1(); // calls Command member function do_stuff_1() which interacts with user and/or calls custom project function
            break;
        case 3:
            do_stuff_2(); // calls Command member function do_stuff_2() which interacts with user and/or calls custom project function
            break;
        case 4:
            do_stuff_3(); // calls Command member function do_stuff_3() which interacts with user and/or calls custom project function
            break;
        case 5:
            random_tester();
            break;
            
            // default case
        default:
            std::cout << "Not a command. Please try again." << std::endl;
            break;
    }
}

// function to get and process user input commands - loops forever until Command member keep_going set to false
void Command::get()
{
	int command_count = sizeof(commands) / sizeof(commands[0]);
	std::string s;
	int c = 100; // if no match found, 100 will send eval() function to default error message
	//std::cout << "Enter command (or type 'help'): ";
    std::cout << "Enter command: ";
	getline(std::cin, s);
	for (int i = 0; i < command_count; i++)
    {
		if (s == commands[i])
        {
			c = i;
			break;
		}
	}
    eval(c);
	std::cout << std::endl;
	if(keep_going) get();
}

// built-int functions
void Command::help()
{
    std::cout << help_text << std::endl;
}

void Command::exit_program()
{
    keep_going = false;
    std::cout << "Closing program..." << std::endl;
}

/////////////////////////
/////////////////////////
/// CUSTOM  COMMANDS BELOW ///
/////////////////////////
/////////////////////////

// example custom function which calls SampleClass member function
void Command::do_stuff_1()
{
    //sc->function_1();
}

// example custom function that requires command line input and calls SampleClass member function
void Command::do_stuff_2()
{
    std::string s;
    std::cout << "  User input: ";
    getline(std::cin, s);
    //sc->function_2(s);
}

// example custom function which calls SampleClass member function which throws an error
void Command::do_stuff_3()
{
    //sc->function_3();
}

void Command::random_tester()
{
    std::string s;
    std::cout << "  number of tests to perform: ";
    getline(std::cin, s);
    int rand;
    for (int i = 0; i < 10; i++)
    {
        rand = rand_int(0,10);
        std::cout << "rand is: " << rand << std::endl;
    }
    
}


