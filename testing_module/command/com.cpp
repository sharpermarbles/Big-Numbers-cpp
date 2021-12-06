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
static void demo   ( Parsed const * const );
static void random ( Parsed const * const );
static void test   ( Parsed const * const );

// STEP 3: ADD COMMANDS AND THEIR OPTIONS TO THE Com::load() FUNCTION BELOW.
//         USE _CREATE_COMMAND_ AND _ADD_OPTION_ WITH PROPER ARGUMENTS
//         AS IN EXAMPLES. CLOSE EACH STATEMENT WITH "____". IF A COMMAND ONLY TAKES
//         A SIMPLE TEXT STRING (NO OPTION NICKNAME OR FULLNAME), THEN SHOW NICKNAME
//         AND FULLNAME AS '!' and "" RESPECTIVELY. IF DESIRED, REVISE WELCOME AND EXIT
//         FOR YOUR PROGRAM.
void Com::load( std::vector<Com*> * const vec)
{
    // revise welcome and exit message as desired
    welcome_msg = "Test module for Big Numbers library. Type \"help\" if needed.";
    exit_msg    = "Exiting program.";
    
    _CREATE_COMMAND_
        random,
        "random",
        "Performs a default of 50 random operations with bgnm objects and check for errors or discrepencies."
        ____
        _ADD_OPTION_ // option 1
             'n', "number",
             "Number of operations to perform if not the default number.", true, true
             ____
        _ADD_OPTION_ // option 1
             'e', "",
             "Only show test results that indicate errors or discrepencies."
             ____
    
    _CREATE_COMMAND_
        demo,
        "demo",
        "Demo runs a demonstration, testing all the overrides of the Bgnm constructor for various data types. The results will be printed out in comparison to the original data to confirm fidelity."
        ____
    
    _CREATE_COMMAND_
        test,
        "test",
        "Executes a specific operation (option -o) on Bgnm objects to check for errors or discrepencies."
        ____
        _ADD_OPTION_ // option 1
             'n', "number",
             "Number of operations to perform if not the default number.", true, true
             ____
        _ADD_OPTION_ // option 1
             'o', "operation",
             "Name of operation to be tested. Possibilities are:\n             add      (add)\n             sub      (subtract)\n             mult     (multiply)\n             div      (divide)\n             inc_pre  (increment prefix)\n             inc_post (increment postfix)\n             dec_pre  (decriment prefix)\n             dec_post (decriment postfix)\n", true, true
             ____
        _ADD_OPTION_ // option 1
             'e', "",
             "Only show test results that indicate discrepencies."
             ____
        _ADD_OPTION_ // option 1
             'c', "constants",
             "Provide the constants(s) that the operation (add, sub, mult, etc.) should evaluate. If providing two numbers (e.g. a and b), separate b from a with at least one space. If either a or b are negative, enclose entire string in quotes (e.g. \"-0.02352 -26234.734409\").", true, true
             ____
    
}

// STEP 4: ADD CUSTOM FUNCTION DEFINITIONS HERE
//     NOTE THAT THERE ARE SEVERAL AVAILABLE parsed->check() OVERLOADS DEPENDING
//     ON HOW EACH OPTION IS SET UP, ESPECIALLY WHETHER OR NOT ARGUMENTS ARE
//     REQUIRED OR GIVEN. REFER TO parsed.h FOR MORE CLARIFICATION. NOTE THAT IF AN
//     OPTION MAY BE CALLED BY EITHER NICKNAME OR FULLNAME (FOR EXAMPLE -e OR --example_option )
//     YOU MAY USE EITHER THE METHOD parsed->check(const char& nickname) OR THE
//     METHOD parsed->check(const std::string& fullname). IT WILL NOT MATTER WHICH ONE YOU USE.

void random (Parsed const * const parsed )
{
    bool n,e=true;
    int n_int;
    std::string number;
    n = parsed->check('n',number);
    e = !(parsed->check('e'));
    bool stoi_valid = true;
    try
    {
        n_int = std::stoi(number);
    }
    catch (...)
    {
        stoi_valid = false;
    }
    if(n && stoi_valid)
    {
        Testing::random_tester(n_int, e);
    }
    else
    {
        Testing::random_tester(20, e);
    }
}

void test (Parsed const * const parsed )
{
    bool n,e=true,c=false,o=false;
    int n_int;
    std::string number = "",operation = "",constants = "";
    n = parsed->check('n',number);
    o = parsed->check('o',operation);
    e = !(parsed->check('e'));
    c = parsed->check('c',constants);
    bool stoi_valid = true;
    try
    {
        n_int = std::stoi(number);
    }
    catch (...)
    {
        stoi_valid = false;
    }
    if(c)
    {
        if(n) std::cout << "     [ -n (--number) is not a valid option together with -c (--constants). -n will be ignored.\n";
        Testing::test(operation, constants, 1, e);
    }
    else if(n && stoi_valid)
    {
        Testing::test(operation, constants, n_int, e);
    }
    else
    {
        Testing::test(operation, constants, 20, e);
    }
}

void demo (Parsed const * const parsed )
{
    Testing::demo();
}