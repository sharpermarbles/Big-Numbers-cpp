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
static void demo       ( Parsed const * const );
static void random     ( Parsed const * const );
static void exceptions ( Parsed const * const );
static void threshhold ( Parsed const * const );
//static void test   ( Parsed const * const );

static void add        ( Parsed const * const );
static void sub        ( Parsed const * const );
static void mult       ( Parsed const * const );
static void div        ( Parsed const * const );
static void inc_pre    ( Parsed const * const );
static void inc_post   ( Parsed const * const );
static void dec_pre    ( Parsed const * const );
static void dec_post   ( Parsed const * const );
static void mod        ( Parsed const * const );
static void pow        ( Parsed const * const );
static void great      ( Parsed const * const );
static void less       ( Parsed const * const );
static void great_eql  ( Parsed const * const );
static void less_eql   ( Parsed const * const );
static void equal      ( Parsed const * const );
static void not_equal  ( Parsed const * const );
static void shift_r    ( Parsed const * const );
static void shift_l    ( Parsed const * const );
static void add_assign ( Parsed const * const );
static void sub_assign ( Parsed const * const );
static void mult_assign( Parsed const * const );
static void div_assign ( Parsed const * const );
static void mod_assign ( Parsed const * const );
static void root       ( Parsed const * const );
static void sqrt       ( Parsed const * const );
static void cbrt       ( Parsed const * const );
static void abs        ( Parsed const * const );
static void floor      ( Parsed const * const );
static void ceil       ( Parsed const * const );


// STEP 3: ADD COMMANDS AND THEIR OPTIONS TO THE Com::load() FUNCTION BELOW.
//         USE _CREATE_COMMAND_ AND _ADD_OPTION_ WITH PROPER ARGUMENTS
//         AS IN EXAMPLES. CLOSE EACH STATEMENT WITH "____". IF A COMMAND ONLY TAKES
//         A SIMPLE TEXT STRING (NO OPTION NICKNAME OR FULLNAME), THEN SHOW NICKNAME
//         AND FULLNAME AS '!' and "" RESPECTIVELY. IF DESIRED, REVISE WELCOME AND EXIT
//         FOR YOUR PROGRAM.


#define   _ADD_TEST_FUNCTION_OPTIONS_A_B_   vec->back()->add_opt( new Opt( 'n', "number", "Number of operations to perform if not the default number.", true, true)); vec->back()->add_opt( new Opt( 'e', "", "Only show test results that indicate discrepencies." )); vec->back()->add_opt( new Opt('c', "constants", "Provide a and b separated by space. If a or b is negative, enclose string in quotes (e.g. \"0.02352 -26234.734409\").", true, true ));  vec->back()->add_opt( new Opt('t', "", "Time execution of all tests and report at the end."));

#define   _ADD_TEST_FUNCTION_OPTIONS_A_   vec->back()->add_opt( new Opt( 'n', "number", "Number of operations to perform if not the default number.", true, true)); vec->back()->add_opt( new Opt( 'e', "", "Only show test results that indicate discrepencies." )); vec->back()->add_opt( new Opt('c', "constants", "Provide constant a. If a is negative, enclose in quotes (e.g. \"-26234.00734409\").", true, true )); vec->back()->add_opt( new Opt('t', "", "Time execution of all tests and report at the end."));


void Com::load( std::vector<Com*> * const vec)
{
    // revise welcome and exit message as desired
    welcome_msg = "Test module for Big Numbers library. Type \"help\" if needed.";
    exit_msg    = "Exiting program.";
    
    _CREATE_COMMAND_
        threshhold,
        "threshhold",
        "Set or view error threshhold. When a test is performed with Bgnm objects, the result is compared against the same operation with type double. If there is a discrepency between the two results, the error threshhold sets whether the discrepency is reported. Remember that the discrepency is calculated as a fraction of the difference between the results divided by the Bgnm result. Therefore in most cases the discrepency will be something like 3.412e-16. Therefore the error threshhold should be set to something in the range of 1e-15 to 1e-18. However the only rule is that it be a possitive number."
        ____
        _ADD_OPTION_
             '!', "",
             "Enter new threshhold or leave empty to view current threshhold. Type \"threshhold 0\" to reset error threshhold to default value.", true, false
             ____
    
    _CREATE_COMMAND_
        random,
        "random",
        "Performs a default of 50 random operations with bgnm objects and check for errors or discrepencies."
        ____
        _ADD_OPTION_
             'n', "number",
             "Number of operations to perform if not the default number.", true, true
             ____
        _ADD_OPTION_
             'e', "",
             "Only show test results that indicate errors or discrepencies."
             ____
        _ADD_OPTION_
             't', "",
             "Time execution of all tests and report at the end."
             ____
    
    _CREATE_COMMAND_
        demo,
        "demo",
        "Demo runs a demonstration, testing all the overrides of the Bgnm constructor for various data types. The results will be printed out in comparison to the original data to confirm fidelity."
        ____

    _CREATE_COMMAND_
        add,
        "add",
        "(add) Executes a + b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        sub,
        "sub",
        "(subtract) Executes a - b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        mult,
        "mult",
        "(greater than) Executes a x b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        div,
        "div",
        "(divide) Executes a / b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        inc_pre,
        "inc_pre",
        "(increment prefix) Executes ++a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        inc_post,
        "inc_post",
        "(increment postfix) Executes a++."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        dec_pre,
        "dec_pre",
        "(decrement prefix) Executes --a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        dec_post,
        "dec_post",
        "(decrement postfix) Executes a--."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        mod,
        "mod",
        "(modulo) Executes a % b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        pow,
        "pow",
        "(power) Executes a ^ b. (NOTE: in Bgnm library, '^' operator is the power operator, not XOR operator)"
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        great,
        "great",
        "(greater than) Answers true if a > b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        less,
        "less",
        "(less than) Answers true if a < b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        great_eql,
        "great_eql",
        "(greater than or equal) Answers true if a >= b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        less_eql,
        "less_eql",
        "(less than or equal) Answers true if a <= b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        equal,
        "equal",
        "(equal) Answers true if a == b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        not_equal,
        "not_equal",
        "(not equal) Answers true if a != b."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        shift_r,
        "shift_r",
        "(shift right) Base-ten operation analogous to bitwise shift right. Executes a / (10^b)."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        shift_l,
        "shift_l",
        "(shift left) Base-ten operation analogous to bitwise shift left. Executes a x (10^b)."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        add_assign,
        "add_assign",
        "(add and assign) Executes a + b and assigns result to a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        sub_assign,
        "sub_assign",
        "(subtract and assign) Executes a - b and assigns result to a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        mult_assign,
        "mult_assign",
        "(multiply and assign) Executes a x b and assigns result to a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        div_assign,
        "div_assign",
        "(divide and assign) Executes a / b and assigns result to a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        mod_assign,
        "mod_assign",
        "(modulo and assign) Executes a % b and assigns result to a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        root,
        "root",
        "(find nth root) Calculates b(th) root of a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_B_
    
    _CREATE_COMMAND_
        sqrt,
        "sqrt",
        "(square root) Calculates square root of a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        cbrt,
        "cbrt",
        "(cube root) Calculates cube root of a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        abs,
        "abs",
        "(absolute) Finds absolute value of a."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        floor,
        "floor",
        "(round down) Round a (floating point) down to nearest integer."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        ceil,
        "ceil",
        "(round up) Round a (floating point) up to nearest integer."
        ____
        _ADD_TEST_FUNCTION_OPTIONS_A_
    
    _CREATE_COMMAND_
        exceptions,
        "exceptions",
        "Test all Bgnm exceptions included in Bgnm library."
        ____
    
}

// STEP 4: ADD CUSTOM FUNCTION DEFINITIONS HERE
//     NOTE THAT THERE ARE SEVERAL AVAILABLE parsed->check() OVERLOADS DEPENDING
//     ON HOW EACH OPTION IS SET UP, ESPECIALLY WHETHER OR NOT ARGUMENTS ARE
//     REQUIRED OR GIVEN. REFER TO parsed.h FOR MORE CLARIFICATION. NOTE THAT IF AN
//     OPTION MAY BE CALLED BY EITHER NICKNAME OR FULLNAME (FOR EXAMPLE -e OR --example_option )
//     YOU MAY USE EITHER THE METHOD parsed->check(const char& nickname) OR THE
//     METHOD parsed->check(const std::string& fullname). IT WILL NOT MATTER WHICH ONE YOU USE.

void threshhold (Parsed const * const parsed)
{
    std::string new_thresh = parsed->check();
    double nt;
    if(new_thresh != "")
    {
        try {nt = std::stod(new_thresh);}
        catch (...)
        {
            std::cout << "    Provided argument is not a number" << std::endl;
        }
        Testing::set_error_threshhold(nt);
        std::cout << "    Error threshhold successfully changed to " << Testing::get_error_threshhold() << ". \n";
    }
    else
    {
        std::cout << "    Error threshhold currently set to " << Testing::get_error_threshhold() << ". \n";
    }
}

void random (Parsed const * const parsed )
{
    bool n,e=true,t=false;
    int n_int;
    std::string number;
    n = parsed->check('n',number);
    e = !(parsed->check('e'));
    t = parsed->check('t');
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
        Testing::random_tester(n_int, e, t);
    }
    else
    {
        Testing::random_tester(20, e, t);
    }
}

void demo (Parsed const * const parsed )
{
    Testing::demo();
}

void exceptions (Parsed const * const parsed)
{
    Testing::exceptions();
}

bool prepare_parameters(Parsed const * const parsed, std::string & constants, int & count, bool & e, bool & t)
{
    std::string count_str;
    int temp = 1;
    bool n = parsed->check('n',count_str);
    e      = !(parsed->check('e'));
    bool c = parsed->check('c',constants);
    t = parsed->check('t');
    bool stoi_valid = true;
    try
    {
        temp = std::stoi(count_str);
    }
    catch (...)
    {
        stoi_valid = false;
    }
    if(c)
    {
        if(n) std::cout << "     -n (--number) is not a valid option together with -c (--constants). -n will be ignored.\n";
        count = 1;
    }
    else if (n && stoi_valid) count = temp;
    else count = 50;
    return true;
}

void add( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("add", constants, count, e, t);
}

void sub( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("sub", constants, count, e, t);
}

void mult( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("mult", constants, count, e, t);
}

void div( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("div", constants, count, e, t);
}

void inc_pre( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("inc_pre", constants, count, e, t);
}

void inc_post( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("inc_post", constants, count, e, t);
}

void dec_pre( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("dec_pre", constants, count, e, t);
}

void dec_post( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("dec_post", constants, count, e, t);
}

void mod( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("mod", constants, count, e, t);
}

void pow( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("pow", constants, count, e, t);
}

void great( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("great", constants, count, e, t);
}

void less( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("less", constants, count, e, t);
}

void great_eql( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("great_eql", constants, count, e, t);
}

void less_eql( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("less_eql", constants, count, e, t);
}

void equal( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("equal", constants, count, e, t);
}

void not_equal( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("not_equal", constants, count, e, t);
}

void shift_r( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("shift_r", constants, count, e, t);
}

void shift_l( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("shift_l", constants, count, e, t);
}

void add_assign( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("add_assign", constants, count, e, t);
}

void sub_assign( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("sub_assign", constants, count, e, t);
}

void mult_assign( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("mult_assign", constants, count, e, t);
}

void div_assign( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("div_assign", constants, count, e, t);
}

void mod_assign( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("mod_assign", constants, count, e, t);
}

void root( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("root", constants, count, e, t);
}

void sqrt( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("sqrt", constants, count, e, t);
}

void cbrt( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("cbrt", constants, count, e, t);
}

void abs( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("abs", constants, count, e, t);
}

void floor( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("floor", constants, count, e, t);
}

void ceil( Parsed const * parsed)
{
    bool e, t; int count; std::string constants;
    prepare_parameters(parsed, constants, count, e, t);
    Testing::test("ceil", constants, count, e, t);
}


