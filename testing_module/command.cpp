
#include "command.h"
#include <iostream>
#include <ctime> // for seeding rand() function


const std::string Command::commands[] = {
    // built in commands
    "help", "exit",
    
    //custom commands - must be entered here exactly as user should enter command
    "random"
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


// rand_int() will return numbers from flr to ceil inclusive
static uint rand_int(int flr, int ceil)
{
    int mod = ceil - flr + 1;
    if (mod < 0) throw "rand_int function given bad parameters.\n";
    return (rand() % mod) + flr;
}

static Bgnm* rand_bgnm(bool force_possitive = false)
{
    #define MAX_STR_SIZE         20
    #define PROBABILITY_INT       2
    #define PROBABILITY_POSSITIVE 2
    #define MAX_NUM_ZEROS        15
    
    // how big a number should we make?
    int size = rand_int(1,MAX_STR_SIZE);
    
    Bgnm * random = new Bgnm;
    
    // should we create a float or int?
    int f_or_i = rand_int(0,PROBABILITY_INT);
    if(f_or_i == 0)
    {
        //create a float bgnm
        std::string fl = "";
        for(int i = 0; i < size; i++)
        {
            fl.push_back('0' + rand_int(0, 9));
        }
        int dec_location = rand_int(0,(int)fl.size()-1);
        fl[dec_location] = '.';
        
        // add zeros for fun
        if(rand_int(0,3) < 3)
        {
            // make sure decimal isn't at very end, also only add zeros to small floats (where decimal is in first few places)
            if(dec_location + 1 < (int)fl.size() && dec_location < 3)
            {
                for(int i = 0; i < rand_int(1, MAX_NUM_ZEROS);i++) fl.insert(dec_location + 1,"0");
            }
        }
        
        *random = fl;
    }
    else
    {
        //create a int bgnm
        std::string int_num = "";
        for(int i = 0; i < size; i++)
        {
            int_num.push_back('0' + rand_int(0, 9));
        }
        *random = int_num;
    }
    
    // should we make it possitive or negative?
    int p_or_n = rand_int(0,PROBABILITY_POSSITIVE);
    if(p_or_n == 0)
    {
        // make number negative
        if(!force_possitive) *random = *random * -1;
    }
    
    return random;
}

static void test_add();

static void perform_random_test()
{
    // how many switch cases are there?
    int types_of_tests = 1;
    // pick one at random
    int rand = rand_int(0, types_of_tests - 1);
    switch (rand) {
        case 0:
            test_add(); break;
        case 1:
            std::cout << "running test case  1\n";
            break;
        case 2:
            std::cout << "running test case   2\n";
            break;
        default:
            //std::cout << "error running test case\n";
            break;
    }
}

void Command::random_tester()
{
    
    int isnt_num, test_count = 0;
    do {
        std::cout << "    Number of tests to perform: ";
        std::cin >> test_count;
        isnt_num = std::cin.fail();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (isnt_num == true);
    
    for (int i = 0; i < test_count; i++)
    {
        try {
            perform_random_test();
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    
}

static void test_add()
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    Bgnm c = *a + *b;
    double af, bf, cf;
    af = a->to_double();
    bf = b->to_double();
    cf = af + bf;
    if(c.to_double() != cf)
    {
        std::cout << "\n---- WE HAVE US A BIG PROBLEM HERE! ----\n";
        std::cout << "   bgnm: " << *a << " + " << *b << " = " << c << std::endl;
        std::cout << "   cont: " << af << " + " << bf << " = " << cf << std::endl;
        std::cout << "\n";
    }
    else
    {
        std::cout << " addition test successful\n";
    }
}
