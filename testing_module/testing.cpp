
#include "testing.h"
#include <iostream>
#include <ctime> // for seeding rand() function

#define MAX_STR_SIZE             20 // rand_bgnm() - maximum size of randomly generated Bgnm to work with (base 10 places)
#define PROBABILITY_INT           2 // rand_bgnm() - bigger number means more likely the returned random bgnm will be integer instead of floating point
#define PROBABILITY_POSSITIVE     2 // rand_bgnm() - bigger number means more likely the returned random bgnm will be possitive
#define MAX_NUM_ZEROS            15 // rand_bgnm() - maximum number of zeros that may be randomly added to random bgnm (floating point)
#define ERROR_THRESHHOLD 0.000000000000000001 // not_equal() - difference between bgnm calc and control calc which sounds the alarm

//const std::string Command::commands[] = {
//    // built in commands
//    "help", "exit",
//
//    //custom commands - must be entered here exactly as user should enter command
//    "random"
//};

// pointers to custom classes should be passed to the Command constructor
//Command::Command()
//{
//
//    //make sure rand() function is seeded
//    std::srand((uint)std::time(0));
//
//    // set member pointer equal to pointer to custom class
//    //sc = _sc;
//
//    std::string s = "";
//    s += "    help          - List all possible user commands.\n";
//    s += "    exit          - Terminate program.\n";
//
//    // edit list below to include description/help for each custom function
//    s += "    random        - performs random tests on Bgnm class objects and methods.\n";
//    help_text = s;
//}

//Command::~Command()
//{
//}

//void Command::eval(int c)
//{
//    switch (c)
//    {
//            // built-in functions
//        case 0:
//            help(); //provides list of possible user commands
//            break;
//        case 1:
//            exit_program(); //terminate program
//            break;
//
//            // custom functions (case number MUST MATCH INDEX of command in commands[] array above
//        case 2:
//            random_tester();
//            break;
//
//            // default case
//        default:
//            std::cout << "Not a command. Please try again." << std::endl;
//            break;
//    }
//}

// function to get and process user input commands - loops forever until Command member keep_going set to false
//void Command::get()
//{
//    int command_count = sizeof(commands) / sizeof(commands[0]);
//    std::string s;
//    int c = 100; // if no match found, 100 will send eval() function to default error message
//    //std::cout << "Enter command (or type 'help'): ";
//    std::cout << "Enter command: ";
//    getline(std::cin, s);
//    for (int i = 0; i < command_count; i++)
//    {
//        if (s == commands[i])
//        {
//            c = i;
//            break;
//        }
//    }
//    eval(c);
//    std::cout << std::endl;
//    if(keep_going) get();
//}

//// built-int functions
//void Command::help()
//{
//    std::cout << help_text << std::endl;
//}
//
//void Command::exit_program()
//{
//    keep_going = false;
//    std::cout << "Closing program..." << std::endl;
//}

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
        if(fl == ".") fl = "0";
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

static void test_add         ( bool show_success = true);
static void test_sub         ( bool show_success = true);
static void test_mult        ( bool show_success = true);
static void test_div         ( bool show_success = true);
static void test_inc_prefix  ( bool show_success = true);
static void test_inc_postfix ( bool show_success = true);
static void test_dec_prefix  ( bool show_success = true);
static void test_dec_postfix ( bool show_success = true);

static void perform_random_test()
{
    // how many switch cases are there?
    int types_of_tests = 8;
    // pick one at random
    int rand = rand_int(0, types_of_tests - 1);
    switch (rand) {
        case 0:
            test_add(); break;
        case 1:
            test_sub(); break;
        case 2:
            test_mult(); break;
        case 3:
            test_div(); break;
        case 4:
            test_inc_prefix(); break;
        case 5:
            test_inc_postfix(); break;
        case 6:
            test_dec_prefix(); break;
        case 7:
            test_dec_postfix(); break;
        default:
            //std::cout << "error running test case\n";
            break;
    }
}

void Testing::random_tester(int number)
{
    for (int i = 0; i < number; i++)
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

// provide both the bgnm answer and the float answer, and also a double pointer to put the calculated difference into
bool static not_equal(Bgnm &bn, double &db, double *diff)
{
    double temp = bn.to_double();
    double denominator;
    db == 0 ? denominator = 1 : denominator = db ; // just in case div by zero problem
    *diff =  (temp - db)/denominator; // difference is the difference between the two numbers divided by the size of one of them (to give it relative weighting)
    //std::cout << "denom is " << denominator << "  ";
    if(*diff < ERROR_THRESHHOLD) // determine both to be equal is the two results are within one one-hundred-millionth of each other
    {
        return false;
    }
    else
    {
        return true;
    }
}

static void print_success(Bgnm *a = NULL, Bgnm* b = NULL, Bgnm* c = NULL, std::string oper = "")
{
#define CHAR_ARRAY_SIZE (MAX_STR_SIZE + MAX_NUM_ZEROS + 2)
    std::string a_ar = ""; a_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string b_ar = ""; b_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string c_ar = ""; c_ar.insert(0,CHAR_ARRAY_SIZE * 2,' ');
    std::string d_ar = ""; d_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string as = a->to_string(), bs = b->to_string(), cs = c->to_string();
    a_ar.replace(a_ar.size()-as.size(),as.size(),as);
    b_ar.replace(0,bs.size(),bs);
    c_ar.replace(0,cs.size(),cs);
    if(oper == " ++X" || oper == " --X" || oper == " X++" || oper == " X--")
    {
        std::cout << "    " << a_ar << oper << d_ar << " = " << c_ar << std::endl;
    }
    else
    {
        std::cout << "    " << a_ar << oper << b_ar << " = " << c_ar << std::endl;
    }
}

static void print_failure(Bgnm* a = NULL, Bgnm* b = NULL, Bgnm* c = NULL, double *af = NULL, double* bf = NULL, double* cf = NULL, double* diff = NULL, std::string oper = "")
{
    print_success(a,b,c,oper);
    std::cout << "\n---- WE HAVE US A BIG PROBLEM HERE! ----\n";
    std::cout << "   Fractional difference of " << *diff << " is greater than allowed error threshhold of " << ERROR_THRESHHOLD << std::endl;
    std::cout << "   bgnm    operation: " << *a << oper << *b << " = " << *c << std::endl;
    std::cout << "   control operation: " << *af << oper << *bf << " = " << *cf << std::endl;
    std::cout << "\n";
}


static void test_add(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    Bgnm c = *a + *b;
    double af, bf, cf;
    af = a->to_double();
    bf = b->to_double();
    cf = af + bf;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," +  ");
    else if ( show_success ) print_success(a,b,&c," +  ");
}


static void test_sub(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    Bgnm c = *a - *b;
    double af, bf, cf;
    af = a->to_double();
    bf = b->to_double();
    cf = af - bf;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," -  ");
    else if ( show_success ) print_success(a,b,&c," -  ");
}

static void test_mult(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    Bgnm c = *a * *b;
    double af, bf, cf;
    af = a->to_double();
    bf = b->to_double();
    cf = af * bf;
    double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,b,&c,&af,&bf,&cf,&diff," x  ");
    }
    else if ( show_success ) print_success(a,b,&c," x  ");
}


static void test_div(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    Bgnm c = *a / *b;
    double af, bf, cf;
    af = a->to_double();
    bf = b->to_double();
    cf = af / bf;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," /  ");
    else if ( show_success ) print_success(a,b,&c," /  ");
}

static void test_inc_prefix(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    Bgnm a_temp = *a;
    Bgnm c = ++*a;
    double af, bf, cf, af_temp;
    af = a_temp.to_double();
    af_temp = af;
    bf = b->to_double();
    cf = ++af;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," ++X");
    else if ( show_success ) print_success(&a_temp,b,&c," ++X");
}

static void test_inc_postfix(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    Bgnm a_temp = *a;
    Bgnm c = *a++;
    double af, bf, cf, af_temp;
    af = a_temp.to_double();
    af_temp = af;
    bf = b->to_double();
    cf = af++;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," X++");
    else if ( show_success ) print_success(&a_temp,b,&c," X++");
}

static void test_dec_prefix(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    Bgnm a_temp = *a;
    Bgnm c = --*a;
    double af, bf, cf, af_temp;
    af = a_temp.to_double();
    af_temp = af;
    bf = b->to_double();
    cf = --af;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," --X");
    else if ( show_success ) print_success(&a_temp,b,&c," --X");
}

static void test_dec_postfix(bool show_success)
{
//  std::cout << "running test case 0\n";
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    Bgnm a_temp = *a;
    Bgnm c = *a--;
    double af, bf, cf, af_temp;
    af = a_temp.to_double();
    af_temp = af;
    bf = b->to_double();
    cf = af--;
    double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," X--");
    else if ( show_success ) print_success(&a_temp,b,&c," X--");
}
