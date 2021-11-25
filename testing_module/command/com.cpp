//
//  coms.cpp
//  command
//
//  Created by Ben Harvey on 4/11/21.
//

// //////////////////////////////////////////////////////////////////
// INSTRUCTIONS AND CUSTOMIZATION BEGIN BELOW ON LINE 125 !!!!!!!!!!!
// //////////////////////////////////////////////////////////////////


#include "com.h"

#define   _CREATE_COMMAND_   add_com(vec, new Com(
#define   _ADD_OPTION_       vec->back()->add_opt( new Opt(
#define   ____               ));

std::string Com::welcome_msg = "";
std::string Com::exit_msg = "";

Com::Com(const func_type _func, const std::string &_name, const std::string &_notes) : func(_func), name(_name), notes(_notes) {}

Com::~Com()
{
    for(int i = 0; i < options.size(); i++) delete options[i];
}

void Com::add_com(std::vector<Com*> * const vec, Com* const com)
{
    vec->push_back(com);
}

void Com::add_opt(Opt* opt)
{
    if(options.size() > 0)
    {
        // check to see if the first one is a simple function, if so, throw an error because this new opt can't be added
        if(options[0]->simple) throw "Can't add another option to command. Command has a non-named, 'simple' option already. If a command has a 'simple' option, it cannot have any others. See com.cpp.";
        // check to see if this opt is a simple opt, if so, throw an error, because if a command has a simple option, it can't have any other options
        if(opt->simple) throw "Can't add this non-named, 'simple' option to this command because command already has other options. If a command has a 'simple' option, it cannot have any others. See com.cpp.";
        // check to see if may_have_argument is false, but argument_required is true (can't require an argument if you're saying there can't be one)
        if(opt->may_have_argument == false && opt->argument_required == true) throw "If Opt member 'may_have_argument' is provided as 'false', then Opt member 'argument_required' cannot be 'true'. See com.ccp.";
    }
    options.push_back(opt);
    std::sort(options.begin(),options.end(),[](Opt* a, Opt* b){return *a < *b;});
}

void Com::run(Parsed const * const parsed) const
{
    (*func)(parsed);
}

Opt* Com::get_opt_ptr(int i)
{
    if(i > -1 && i < options.size())
    {
        return (options[i]);
    }
    else
    {
        return NULL;
    }
}

bool Com::operator<(const Com& com)
{
    return name < com.name;
}

Opt* Com::check_nickname(const char c) const
{
    for(int i = 0; i < options.size(); i++)
    {
        if(options[i]->nickname == c) return options[i];
    }
    return NULL;
}

Opt* Com::check_fullname(const std::string & s) const
{
    for(int i = 0; i < options.size(); i++)
    {
        if(options[i]->fullname == s) return options[i];
    }
    return NULL;
}

void Com::print()
{
    std::cout << "  =>  " << this->name << std::endl;
    std::cout << "      " << this->notes << std::endl << std::endl;
    for(int n = 0; n < (int)this->options.size(); n++)
    {
        Opt* optr = this->get_opt_ptr(n);
        if(optr != NULL)
        {
            if(optr->simple == false) // only print nickname/fullname if it's not a simple option
            {
                std::cout << "          ";
                if(optr->nickname != '!') std::cout << "-" << optr->nickname;
                if(optr->nickname != '!' && optr->fullname !="")
                {
                    std::cout << ", --" << optr->fullname;
                }
                else if (optr->fullname !="")
                {
                    std::cout << "--" << optr->fullname;
                }
                std::cout << std::endl;
            }
            if(optr->notes != "")
            {
                std::cout << "          " << optr->notes << std::endl << std::endl;
            }
        }
    }
}






// STEP 1: INCLUDE REQUIRED PROJECT HEADERS HERE
#include "testing.h"

// STEP 2: PROVIDE CUSTOM FUNCTION PROTOTYPES HERE (MUST TAKE A const POINTER
//         TO A const Parsed OBJECT AS AN ARGUMENT - CAN BE NULL IF DESIRED)
static void func1( Parsed const * const );
static void func2( Parsed const * const );
static void func3( Parsed const * const );
static void func4( Parsed const * const );
static void func5( Parsed const * const );

static void random( Parsed const * const );

// STEP 3: ADD COMMANDS AND THEIR OPTIONS TO THE Com::load() FUNCTION BELOW.
//         USE _CREATE_COMMAND_ AND _ADD_OPTION_ WITH PROPER ARGUMENTS
//         AS IN EXAMPLES. CLOSE EACH STATEMENT WITH "____". IF A COMMAND ONLY TAKES
//         A SIMPLE TEXT STRING (NO OPTION NICKNAME OR FULLNAME), THEN SHOW NICKNAME
//         AND FULLNAME AS '!' and "" RESPECTIVELY. IF DESIRED, REVISE WELCOME AND EXIT
//         FOR YOUR PROGRAM.
void Com::load( std::vector<Com*> * const vec)
{
    // revise welcome and exit message as desired
    welcome_msg = "Test module for command library.";
    exit_msg    = "Exiting program.";

    // FIRST CUSTOM FUNCTION
    _CREATE_COMMAND_     // macro to begin definition of custom command
        func1,           // function pointer
        "func1",         // name of command/function as string (must be identical to name of actual function)
                         // description of command (will be printed out by help command and --help option)
        "Function with no options or arguments."
        ____             // macro to conclude definition of custom command
    
    // SECOND CUSTOM FUNCTION
    _CREATE_COMMAND_
        func2,
        "func2",
        "func2 excepts a string argument, but it isn't required."
        ____
    
        _ADD_OPTION_      // macro to begin a custom option
             '!', "",     // nickname as char (if none, then '!'), then fullname as string (if none then "")
                          // description of command (will be printed out by help command and --help option).
             "This is considered a \"simple\" option because there is only one option - a basic string.",
             true,        // "true" means this option may have a string argument
             false        // "false" means this option does not require a string argument
             ____         // macro to close this option
    
    // THIRD CUSTOM FUNCTION
    _CREATE_COMMAND_
        func3,
        "func3",
        "func3 requires a string argument."
        ____
    
        _ADD_OPTION_
             '!', "",
             "This is considered a \"simple\" option because there is only one option - a basic string.", true, true
             ____

    
    // FOURTH CUSTOM FUNCTION
    _CREATE_COMMAND_
        func4,
        "func4",
        "func4 has several options with nicknames and/or fullnames, some of which take or require arguments."
        ____
        _ADD_OPTION_ // option 1
             'a', "",
             "Option a is a boolean option."
             ____
        _ADD_OPTION_ // option 2
             'b', "",
             "Option b may take an argument.", true, false
             ____
        _ADD_OPTION_ // option 3
             'c', "",
             "Option c requires an argument.", true, true
             ____
        _ADD_OPTION_ // option 4
             'e', "example_option",
             "Option can be called by either flag/nickname '-e' or fullname '--example_option'. This option is boolean."
             ____
        _ADD_OPTION_ // option 5
             '!', "fullname_option",
             "Option may only be called using the fullname '--fullname_option'. This option is boolean."
             ____
        _ADD_OPTION_ // option 6
             '!', "zanzibar",
             "Option may only be called using the fullname '--zanzibar'. Option may accept an argument.", true, false
             ____
    
    // FIFTH CUSTOM FUNCTION
    _CREATE_COMMAND_
        func5,
        "func5",
        "func5 is contained in com.cpp and does not call any functions from custom headers."
        ____
        _ADD_OPTION_ // option 1
             'x', "x_option",
             "Option x is a boolean option that may accept an argument.", true, false
             ____
    
    // SIXTH CUSTOM FUNCTION
    _CREATE_COMMAND_
        random,
        "random",
        "random will perform a default of 50 random operations with bgnm objects and check for errors or discrepencies."
        ____
        _ADD_OPTION_ // option 1
             'n', "number",
             "Number of operations to perform if not the default number.", true, true
             ____
}

// STEP 4: ADD CUSTOM FUNCTION DEFINITIONS HERE
//     NOTE THAT THERE ARE SEVERAL AVAILABLE parsed->check() OVERLOADS DEPENDING
//     ON HOW EACH OPTION IS SET UP, ESPECIALLY WHETHER OR NOT ARGUMENTS ARE
//     REQUIRED OR GIVEN. REFER TO parsed.h FOR MORE CLARIFICATION. NOTE THAT IF AN
//     OPTION MAY BE CALLED BY EITHER NICKNAME OR FULLNAME (FOR EXAMPLE -e OR --example_option )
//     YOU MAY USE EITHER THE METHOD parsed->check(const char& nickname) OR THE
//     METHOD parsed->check(const std::string& fullname). IT WILL NOT MATTER WHICH ONE YOU USE.

void func1( Parsed const * const parsed )
{
    // example using static methods from class SampleClass
    //SampleClass::function_1();
}

void func2( Parsed const * const parsed )
{
    // example using static methods from class SampleClass
    //SampleClass::function_2(parsed->check()); // (parsed->check() returns a string)
}

void func3( Parsed const * const parsed )
{
    // example using static methods from class SampleClass
    //SampleClass::function_3(parsed->check()); // (parsed->check() returns a string)
}

void func4( Parsed const * const parsed )
{
    // example using runtime instance of SampleClass
    //SampleClass * sc = new SampleClass;
    
    bool a,b,c,e,f,z;
    std::string b_txt, c_txt, z_txt;
    
    a = parsed->check('a');
    b = parsed->check('b',b_txt); // parsed argument will be passed to string b_txt
    c = parsed->check('c',c_txt); // parsed argument will be passed to string c_txt
    e = parsed->check('e'); // or you could use parsed->check("example_option") and get the same result
    f = parsed->check("fullname_option");
    z = parsed->check("zanzibar",z_txt); // parsed argument will be passed to string z_txt
    //sc->function_4(a,b,c,e,f,z,b_txt, c_txt, z_txt);
    
    //delete sc;
}

void func5( Parsed const * const parsed )
{
    // example function that does not call on functions defined in custom headers
    std::string arg;
    bool x = parsed->check('x',arg);
    std::cout       << "    Executed func5 \n";
    if(x) std::cout << "        Option -x / --x_option entered. Argument (if any) : " << arg << "\n";
}

void random (Parsed const * const parsed )
{
    bool n;
    int n_int;
    std::string number;
    n = parsed->check('n',number);
    if(n && (n_int = std::stoi(number)))
    {
        Testing::random_tester(n_int);
    }
    else
    {
        Testing::random_tester();
    }
}
