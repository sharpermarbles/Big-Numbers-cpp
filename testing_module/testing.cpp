

#include <iostream>
#include <cfloat> // for LDBL_EPSILON DBL_EPSILON FLT_EPSILON only
#include <math.h> // for frexp() and fmod() only
#include <ctime>  // for seeding rand() function
#include <sstream>
#include <iostream>
#include <iomanip> // for std::setprecision()
#include <string>

#include "testing.h"


#define MAX_STR_SIZE             20 // rand_bgnm() - maximum size of randomly generated Bgnm to work with (base 10 places)
#define PROBABILITY_INT           1 // rand_bgnm() - bigger number means more likely the returned random bgnm will be integer instead of floating point
#define PROBABILITY_POSSITIVE     2 // rand_bgnm() - bigger number means more likely the returned random bgnm will be possitive
#define MAX_NUM_ZEROS            15 // rand_bgnm() - maximum number of zeros that may be randomly added to random bgnm (floating point)
#define ERROR_THRESHHOLD 0.000000000000001 // not_equal() - difference between bgnm calc and control calc (uses type double) which sounds the alarm

double Testing::error_threshhold = ERROR_THRESHHOLD;

void Testing::set_error_threshhold( const double threshhold)
{
    if (threshhold == 0) error_threshhold = ERROR_THRESHHOLD;
    else if (threshhold > 0)
    {
        error_threshhold = threshhold;
    }
    else throw "    Error threshhold must be greater than zero.\n";
}

double Testing::get_error_threshhold()
{
    return error_threshhold;
}

void Testing::set_bgnm_internal_precision_limit(unsigned lim)
{
    Bgnm::set_bgnm_internal_precision_limit(lim);
}

unsigned Testing::get_bgnm_internal_precision_limit()
{
    return Bgnm::get_bgnm_internal_precision_limit();
}

void Testing::set_bgnm_max_root_guess_count(unsigned count)
{
    Bgnm::set_bgnm_max_root_guess_count(count);
}

unsigned Testing::get_bgnm_max_root_guess_count()
{
    return Bgnm::get_bgnm_max_root_guess_count();
}

void Testing::set_bgnm_root_index_max_precision(unsigned prec)
{
    Bgnm::set_bgnm_root_index_max_precision(prec);
}

unsigned Testing::get_bgnm_root_index_max_precision()
{
    return Bgnm::get_bgnm_root_index_max_precision();
}

// rand_int() will return numbers from flr to ceil inclusive
static uint rand_int(int flr, int ceil)
{
    int mod = ceil - flr + 1;
    if (mod < 0) throw "rand_int function given bad parameters.\n";
    return (rand() % mod) + flr;
}

static Bgnm* rand_bgnm(bool force_possitive = false, bool allow_float = true)
{
    
    // how big a number should we make?
    int size = rand_int(1,MAX_STR_SIZE);
    
    Bgnm * random = new Bgnm;
    
    // should we create a float or int?
    int f_or_i = rand_int(0,PROBABILITY_INT);
    if(!allow_float) f_or_i = 1; // force int if allow_float is false
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
        //create an int bgnm
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

static void timer_report( Timer &t, int number = 1 )
{
    t.stop();
    std::cout << "\n    Excecution of " << number << " tests took " << t.report() << std::endl << std::endl;
}

static void print_e(std::exception &e)
{
    std::cout << "      " << e.what() << std::endl;
}

void Testing::exceptions()
{
    std::cout << "    All Bgnm library exceptions:\n";
    
    // division by zero
    try { Bgnm b1 = 1; Bgnm b2 = b1 / 0; }
    catch(std::exception &e) { print_e(e); }
    
    // string will not parse properly to a number
    try { Bgnm b2("-123..345"); }
    catch(std::exception &e) { print_e(e); }
    
    // provided modulo is zero
    try { Bgnm b3, b4(123); b3 = b4 % 0; }
    catch(std::exception &e) { print_e(e); }
    
    // root of base with zero index 1743
    try { Bgnm b6(123), b7; b7 = b6.root(0);}
    catch(std::exception &e) { print_e(e); }
    
    // exceeded max_root_guess_count 1820
    try
    {
        Bgnm::set_bgnm_max_root_guess_count(2);
        Bgnm b8(1234); Bgnm b9 = b8.root(5.6);
    }
    catch(std::exception &e)
    {
        Bgnm::set_bgnm_max_root_guess_count(0);
        print_e(e);
    }
    
    // calculate square root of negative value 2891
    try { Bgnm b10(-1234); Bgnm b11 = b10.sqrt(); }
    catch(std::exception &e) { print_e(e); }
    
    // bgnm_internal_precision_limit out of range 2902
    try { Bgnm::set_bgnm_internal_precision_limit(1);}
    catch(std::exception &e) { print_e(e); }
    
    // bgnm_max_root_guess_count out of range 2913
    try { Bgnm::set_bgnm_max_root_guess_count(1);}
    catch(std::exception &e) { print_e(e); }
    
    Bgnm b12 ("98765432198765432100098717623984619273846832423451650239847523451362356234523462069384209837405826340587230498572304860238475023984752063452384523745623452345265976293874517823416529387461234013456234875019382475623049857203845623745069872364723984523045983266234562345623462562654321598764325676697005198769238745629387423094857203948750293847520348752039847520938457203948572039845720398475203984752039847520983748783405923740987348758236419298376452734052938452936459238745623847028345982763452374058256293876106203485203.7456");
    
    // cannot convert Bgnm to int (std::stoi error)
    try { std::cout << b12.to_int() << std::endl;}
    catch(std::exception &e) { print_e(e); }

    // cannot convert Bgnm to long long (sdt::stoll error)
    try { std::cout << b12.to_long_long() << std::endl;}
    catch(std::exception &e) { print_e(e); }
    
    // cannot convert Bgnm to float (std::stof error)
    try { std::cout << b12.to_float() << std::endl;}
    catch(std::exception &e) { print_e(e); }
    
    // cannot convert Bgnm to double (std::stod error)
    try { std::cout << b12.to_double() << std::endl;}
    catch(std::exception &e) { print_e(e); }
    
    // cannot convert Bgnm to long double (std::stold error)
    // would need a number with over 2000 decimal places to get this exception to throw even though long_doubles only have about 15 to 20 decimal places of accuracy
    try { std::cout << "    Succesfully converted Bgnm to long double: " << b12.to_long_double() << std::endl;}
    catch(std::exception &e) { print_e(e); }
    
    // in str_pow() the exponent provided as a string is converted to a float - if this is not possible, this exception is thrown
    try { Bgnm b13(1234); b13^1.1234; }
    catch(std::exception &e) { print_e(e); }

}

// provide both the bgnm answer and the float answer, and also a double pointer to put the calculated difference into
static bool not_equal(Bgnm &bn, long double &db, long double *diff)
{
    long double temp = bn.to_long_double();
    long double denominator;
    db == 0 ? denominator = 1 : denominator = db ; // just in case div by zero problem
    *diff =  abs((temp - db)/denominator); // difference is the absolute value of the difference between the two numbers divided by the size of one of them (to give it relative weighting)
    if(*diff < Testing::get_error_threshhold()) // determine both to be equivelant if the two results are within the error threshhold
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::string opers[10] = {" ++X"," --X"," X++"," X--"," 2√ "," 3√ ", " abs"," flr", " clg"};
static bool single_term(const std::string_view operand)
{
    for (int i = 0; i < 11; i++)
    {
        if (operand == opers[i]) return true;
    }
    return false;
}

static void print_success(Bgnm *a = NULL, Bgnm* b = NULL, Bgnm* c = NULL, std::string oper = "")
{
#define CHAR_ARRAY_SIZE (MAX_STR_SIZE + MAX_NUM_ZEROS + 2)
    std::string a_ar = ""; a_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string b_ar = ""; b_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string c_ar = ""; c_ar.insert(0,CHAR_ARRAY_SIZE * 2,' ');
    std::string d_ar = ""; d_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string as = a->to_string(), bs = b->to_string(), cs = c->to_string();
    if(oper == " 2√ " || oper == " 3√ ") a_ar.replace(0,as.size(),as);
    else a_ar.replace(a_ar.size()-as.size(),as.size(),as);
    b_ar.replace(0,bs.size(),bs);
    c_ar.replace(0,cs.size(),cs);
    if(single_term(oper)) // ignore term b if operand doesn't take a second term (e.g. ++ or << or clg)
    {
        if(oper == " 2√ " || oper == " 3√ ")
        {
            std::cout << "    " << d_ar << oper << a_ar << " = " << c_ar << std::endl;
        }
        else std::cout << "    " << a_ar << oper << d_ar << " = " << c_ar << std::endl;
    }
    else
    {
        std::cout << "    " << a_ar << oper << b_ar << " = " << c_ar << std::endl;
    }
}

static void print_success(Bgnm *a = NULL, Bgnm* b = NULL, bool c = true, std::string oper = "")
{
#define CHAR_ARRAY_SIZE (MAX_STR_SIZE + MAX_NUM_ZEROS + 2)
    std::string a_ar = ""; a_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string b_ar = ""; b_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string c_ar = "true";
    std::string d_ar = ""; d_ar.insert(0,CHAR_ARRAY_SIZE,' ');
    std::string as = a->to_string(), bs = b->to_string();
    if(oper == " 2√ " || oper == " 3√ ") a_ar.replace(0,as.size(),as);
    else a_ar.replace(a_ar.size()-as.size(),as.size(),as);
    b_ar.replace(0,bs.size(),bs);
    if(!c) c_ar = "false";
    if(single_term(oper)) // ignore term b if operand doesn't take a second term (e.g. ++ or << or clg)
    {
        if(oper == " 2√ " || oper == " 3√ ")
        {
            std::cout << "    " << d_ar << oper << a_ar << " = " << c_ar << std::endl;
        }
        else std::cout << "    " << a_ar << oper << d_ar << " = " << c_ar << std::endl;
    }
    else
    {
        std::cout << "    " << a_ar << oper << b_ar << " = " << c_ar << std::endl;
    }
}

static void print_failure(Bgnm* a = NULL, Bgnm* b = NULL, Bgnm* c = NULL, long double *af = NULL, long double* bf = NULL, long double* cf = NULL, long double* diff = NULL, std::string oper = "")
{
    
    std::cout << "\n   ---- NOTICE! CONTROL ANSWER DIFFERS BY MORE THAN ERROR THRESHHOLD ----\n";
    std::cout << "   Fractional difference of " << *diff << " is greater than allowed error threshhold of " << Testing::get_error_threshhold() << std::endl;
    if(bf == NULL)
    {
        std::cout << "   bgnm    operation: " << *a << oper << " = " << *c << std::endl;
        std::cout << "   control operation: " << *af << oper << " = " << *cf << std::endl;
    }
    else{
        std::cout << "   bgnm    operation: " << *a << oper << *b << " = " << *c << std::endl;
        std::cout << "   control operation: " << *af << oper << *bf << " = " << *cf << std::endl;
    }
    std::cout << "\n";
}

static void print_failure(Bgnm* a = NULL, Bgnm* b = NULL, bool c = false, long double* af = NULL, long double* bf = NULL, bool cf = false, std::string oper = "")
{
    std::string str_false = "false", ans1 = "true ", ans2 = "true ";
    if(!c) ans1 = str_false;
    if(!cf) ans2 = str_false;
    std::cout << "\n   ---- NOTICE! CONTROL ANSWER DOES NOT AGREE ----\n";
    std::cout << "   bgnm    operation: " << *a << oper << *b << " = " << ans1 << std::endl;
    std::cout << "   control operation: " << *af << oper << *bf << " = " << ans2 << std::endl;
    std::cout << "\n";
}


static void test_add(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a + *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af + bf;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," +  ");
    else if ( show_success ) print_success(a,b,&c," +  ");
}


static void test_sub(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a - *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af - bf;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," -  ");
    else if ( show_success ) print_success(a,b,&c," -  ");
}

static void test_mult(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a * *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af * bf;
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,b,&c,&af,&bf,&cf,&diff," x  ");
    }
    else if ( show_success ) print_success(a,b,&c," x  ");
}


static void test_div(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a / *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af / bf;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,b,&c,&af,&bf,&cf,&diff," /  ");
    else if ( show_success ) print_success(a,b,&c," /  ");
}

static void test_inc_prefix(Bgnm * _a, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    Bgnm c = ++*a;
    long double af, bf, cf, af_temp;
    af = a_temp.to_long_double();
    af_temp = af;
    bf = b->to_long_double();
    cf = ++af;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," ++X");
    else if ( show_success ) print_success(&a_temp,b,&c," ++X");
}

static void test_inc_postfix(Bgnm * _a, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    Bgnm c = *a++;
    long double af, bf, cf, af_temp;
    af = a_temp.to_long_double();
    af_temp = af;
    bf = b->to_long_double();
    cf = af++;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," X++");
    else if ( show_success ) print_success(&a_temp,b,&c," X++");
}

static void test_dec_prefix(Bgnm * _a, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    Bgnm c = --*a;
    long double af, bf, cf, af_temp;
    af = a_temp.to_long_double();
    af_temp = af;
    bf = b->to_long_double();
    cf = --af;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," --X");
    else if ( show_success ) print_success(&a_temp,b,&c," --X");
}

static void test_dec_postfix(Bgnm * _a, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = new Bgnm(0);//rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    Bgnm c = *a--;
    long double af, bf, cf, af_temp;
    af = a_temp.to_long_double();
    af_temp = af;
    bf = b->to_long_double();
    cf = af--;
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," X--");
    else if ( show_success ) print_success(&a_temp,b,&c," X--");
}

static void test_mod(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm(true,true); // forcing possitive numbers until can fix issue with negative modulo operations
    Bgnm* b = rand_bgnm(true,false); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a % *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = fmod(af,bf);
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,b,&c,&af,&bf,&cf,&diff," %  ");
    }
    else if ( show_success ) print_success(a,b,&c," %  ");
}

static void test_pow(Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm(); // forcing possitive numbers until can fix issue with negative modulo operations
    float randflt = (int)rand_int(1,45); // FOR NOW JUST STICKING TO SMALL INTEGER POWERS - NEED TO DO MORE ROBUST TESTING!!
    
    // make about half of the tests have a floating point exponent
    if(rand_int(1,2) == 1) randflt = randflt/rand_int(2,45);
    
    // make about a fourth of the test have a negative exponent
    if(rand_int(1,4) == 1) randflt *= -1;

    Bgnm* b = new Bgnm(randflt); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm c = *a ^ *b;
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = pow(af,bf);
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,b,&c,&af,&bf,&cf,&diff," ^  ");
    }
    else if ( show_success ) print_success(a,b,&c," ^  ");
}


static void test_great( Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 20% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,5) == 1 ? *b = *a: true;
    bool c = *a > *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af > bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," >  ");
    }
    else if ( show_success ) print_success(a,b,c," >  ");
}

static void test_less( Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 20% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,5) == 1 ? *b = *a: true;
    bool c = *a < *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af < bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," <  ");
    }
    else if ( show_success ) print_success(a,b,c," <  ");
}

static void test_great_eql( Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 50% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,2) == 1 ? *b = *a: true;
    bool c = *a >= *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af >= bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," >= ");
    }
    else if ( show_success ) print_success(a,b,c," >= ");
}

static void test_less_eql( Bgnm * _a, Bgnm* _b, bool show_success)
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 50% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,2) == 1 ? *b = *a: true;
    bool c = *a <= *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af <= bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," <= ");
    }
    else if ( show_success ) print_success(a,b,c," <= ");
}

static void test_equal( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 50% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,2) == 1 ? *b = *a: true;
    bool c = *a == *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af == bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," == ");
    }
    else if ( show_success ) print_success(a,b,c," == ");
}

static void test_not_equal( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    // 50% of time Bgnm b should be equal to a for testing purposes
    rand_int(1,2) == 1 ? *b = *a: true;
    bool c = *a != *b;
    bool cf;
    long double af, bf;
    af = a->to_long_double();
    bf = b->to_long_double();
    cf = af != bf;
    if(c != cf)
    {
        print_failure(a,b,c,&af,&bf,cf," != ");
    }
    else if ( show_success ) print_success(a,b,c," != ");
}

static void test_shft_rght( Bgnm * _a, Bgnm * _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    int shift_x = 1;
    if (_b != NULL)
    {
        shift_x = _b->to_int() ;
    }
    else
    {
        shift_x = (int)rand_int(1,MAX_STR_SIZE);
    }
    Bgnm* b = new Bgnm(shift_x);
    *a >> shift_x;
    Bgnm c = *a;
    long double af, af_temp, bf, cf;
    try
    {
        af = a_temp.to_long_double();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    af_temp = af;
    bf = (double)shift_x;
    cf = af / pow(10,shift_x);
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," >> ");
    else if ( show_success ) print_success(&a_temp,b,&c," >> ");
}

static void test_shft_left( Bgnm * _a, Bgnm * _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm a_temp = *a;
    int shift_x = 1;
    if (_b != NULL)
    {
        shift_x = _b->to_int() ;
    }
    else
    {
        shift_x = (int)rand_int(1,MAX_STR_SIZE);
    }
    Bgnm* b = new Bgnm(shift_x);
    *a << shift_x;
    Bgnm c = *a;
    long double af, af_temp, bf, cf;
    try
    {
        af = a_temp.to_long_double();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    af_temp = af;
    bf = (double)shift_x;
    cf = af * (double)pow(10,shift_x);
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(&a_temp,b,&c,&af_temp,&bf,&cf,&diff," << ");
    else if ( show_success ) print_success(&a_temp,b,&c," << ");
}

static void test_add_eqls( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm a_temp = *a; // save a
    *a += *b;
    long double af, af_temp, bf;
    af = a_temp.to_long_double(); // save af
    bf = b->to_long_double();
    af += bf;
    long double diff;
    if(not_equal(*a,af,&diff)) print_failure(&a_temp,b,a,&af_temp,&bf,&af,&diff," += ");
    else if ( show_success ) print_success(&a_temp,b,a," += ");
}

static void test_sub_eqls( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm a_temp = *a; // save a
    *a -= *b;
    long double af, af_temp, bf;
    af = a_temp.to_long_double(); // save af
    af_temp = af; // save af
    bf = b->to_long_double();
    af -= bf;
    long double diff;
    if(not_equal(*a,af,&diff)) print_failure(&a_temp,b,a,&af_temp,&bf,&af,&diff," -= ");
    else if ( show_success ) print_success(&a_temp,b,a," -= ");
}

static void test_mult_eqls( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm a_temp = *a; // save a
    *a *= *b;
    long double af, af_temp, bf;
    af = a_temp.to_long_double();
    af_temp = af; // save af
    bf = b->to_long_double();
    af *= bf;
    long double diff;
    if(not_equal(*a,af,&diff)) print_failure(&a_temp,b,a,&af_temp,&bf,&af,&diff," *= ");
    else if ( show_success ) print_success(&a_temp,b,a," *= ");
}

static void test_div_eqls( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm();
    Bgnm* b = rand_bgnm();
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm a_temp = *a; // save a
    *a /= *b;
    long double af, af_temp, bf;
    af = a_temp.to_long_double();
    af_temp = af; // save af
    bf = b->to_long_double();
    af /= bf;
    long double diff;
    if(not_equal(*a,af,&diff)) print_failure(&a_temp,b,a,&af_temp,&bf,&af,&diff," /= ");
    else if ( show_success ) print_success(&a_temp,b,a," /= ");
}

static void test_mod_eqls( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm(true,false); // forcing possitive integer numbers until can fix issue with negative modulo operations
    Bgnm* b = rand_bgnm(true,false); // forcing possitive integer numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    if (_b != NULL) b = _b ;
    Bgnm a_temp = *a; // save a
    *a %= *b;
    long double af, bf, cf;
    af = a_temp.to_long_double();
    bf = b->to_long_double();
    cf = fmod(af,bf);
    long double diff;
    if(not_equal(*a,cf,&diff)) print_failure(&a_temp,b,a,&af,&bf,&cf,&diff," %= ");
    else if ( show_success ) print_success(&a_temp,b,a," %= ");
}

static void test_root( Bgnm * _a, Bgnm* _b, bool show_success )
{
    Bgnm* a = rand_bgnm(true); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    float randflt = (float)rand_int(1,45); // FOR NOW JUST STICKING TO SMALL INTEGER POWERS - NEED TO DO MORE ROBUST TESTING!!
        
    // make about half of the tests have a floating point index
    if(rand_int(1,2) == 2) randflt = randflt/rand_int(2,45);
    
    // make about a fourth of the test have a negative exponent
    if(rand_int(1,4) == 1) randflt *= -1;
    
    Bgnm* b = new Bgnm(randflt); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_b != NULL) b = _b ;
    Bgnm c = a->root(b->to_float());
    long double af, bf, cf;
    af = a->to_long_double();
    bf = b->to_float();
    cf = pow(af,1.0/bf);
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(b,a,&c,&bf,&af,&cf,&diff," √  ");
    }
    else if ( show_success ) print_success(b,a,&c," √  ");
}

static void test_sqrt( Bgnm * _a, bool show_success )
{
    Bgnm* a = rand_bgnm(true); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    Bgnm c = a->sqrt();
    long double af, cf;
    af = a->to_long_double();
    cf = pow(af,1.0/2.0);
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,new Bgnm,&c,&af,NULL,&cf,&diff," 2√ ");
    }
    else if ( show_success ) print_success(a,new Bgnm,&c," 2√ ");
}

static void test_cbrt( Bgnm * _a, bool show_success )
{
    Bgnm* a = rand_bgnm(true); // forcing possitive numbers until can fix issue with negative modulo operations
    if (_a != NULL) a = _a ;
    Bgnm c = a->cbrt();
    long double af, cf;
    af = a->to_long_double();
    cf = pow(af,1.0/3.00);
    long double diff;
    if(not_equal(c,cf,&diff))
    {
        print_failure(a,new Bgnm,&c,&af,NULL,&cf,&diff," 3√ ");
    }
    else if ( show_success ) print_success(a,new Bgnm,&c," 3√ ");
}

static void test_abs( Bgnm * _a, bool show_success )
{
    Bgnm* a = rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm c = a->abs();
    long double af, cf;
    af = a->to_long_double();
    cf = abs(af);
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,new Bgnm,&c,&af,NULL,&cf,&diff," abs");
    else if ( show_success ) print_success(a,new Bgnm,&c," abs");
}

static void test_floor( Bgnm * _a, bool show_success )
{
    Bgnm* a = rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm c = a->floor();
    long double af, cf;
    af = a->to_long_double();
    cf = floor(af);
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,new Bgnm,&c,&af,NULL,&cf,&diff," flr");
    else if ( show_success ) print_success(a,new Bgnm,&c," flr");
}

static void test_ceil( Bgnm * _a, bool show_success )
{
    Bgnm* a = rand_bgnm();
    if (_a != NULL) a = _a ;
    Bgnm c = a->ceil();
    long double af, cf;
    af = a->to_long_double();
    cf = ceil(af);
    long double diff;
    if(not_equal(c,cf,&diff)) print_failure(a,new Bgnm,&c,&af,NULL,&cf,&diff," clg");
    else if ( show_success ) print_success(a,new Bgnm,&c," clg");
}

static void try_test(void (*f)(Bgnm* a, Bgnm* b, bool show_all), int number, Bgnm* a, Bgnm* b, bool show_all, bool timer)
{
    Timer t;
    for (int i = 0; i < number; i++)
    {
        try { f(a,b,show_all);}
        catch (std::exception &e) { std::cout << "    " << e.what() << std::endl;}
    }
    if(timer) timer_report(t,number);
}

static void try_test(void (*f)(Bgnm* a, bool show_all), int number, Bgnm* a, bool show_all, bool timer)
{
    Timer t;
    for (int i = 0; i < number; i++)
    {
        try { f(a,show_all);}
        catch (std::exception &e) { std::cout << "    " << e.what() << std::endl;}
    }
    if(timer) timer_report(t,number);
}

void Testing::test(const std::string & op, std::string constants, int number, bool show_all, bool timer )
{
    Bgnm * a = NULL;
    Bgnm * b = NULL;
    if(constants != "")
    {
        std::string str_a = "";
        std::string str_b = "";
        std::string * str = &str_a;
        for (int i = 0; i < constants.size(); i++)
        {
            if (constants[i] != ' ')
            {
                str->push_back(constants[i]);
            }
            else
            {
                str = &str_b;
            }
        }
        try {a = new Bgnm(str_a);}      catch (...){ std::cout << "     Could not convert string to Bgnm type. Please try again.\n";return;}
        try {b = new Bgnm(str_b);}      catch (...){ std::cout << "     Could not convert string to Bgnm type. Please try again.\n";return;}
    }
    if      (op == "add")
    {
        try_test( test_add, number, a, b, show_all, timer);
    }
    else if (op == "sub")
    {
        try_test( test_sub, number, a, b, show_all, timer);
    }
    else if (op == "mult")
    {
        try_test( test_mult, number, a, b, show_all, timer);
    }
    else if (op == "div")
    {
        try_test( test_div, number, a, b, show_all, timer);
    }
    else if (op == "inc_pre")
    {
        try_test( test_inc_prefix, number, a, show_all, timer);
    }
    else if (op == "inc_post")
    {
        try_test( test_inc_postfix, number, a, show_all, timer);
    }
    else if (op == "dec_pre")
    {
        try_test( test_dec_prefix, number, a, show_all, timer);
    }
    else if (op == "dec_post")
    {
        try_test( test_dec_postfix, number, a, show_all, timer);
    }
    else if (op == "mod")
    {
        try_test( test_mod, number, a, b, show_all, timer);
    }
    else if (op == "pow")
    {
        try_test( test_pow, number, a, b, show_all, timer);
    }
    else if (op == "great")
    {
        try_test( test_great, number, a, b, show_all, timer);
    }
    else if (op == "less")
    {
        try_test( test_less, number, a, b, show_all, timer);
    }
    else if (op == "great_eql")
    {
       try_test( test_great_eql, number, a, b, show_all, timer);
    }
    else if (op == "less_eql")
    {
        try_test( test_less_eql, number, a, b, show_all, timer);
    }
    else if (op == "equal")
    {
        try_test( test_equal, number, a, b, show_all, timer);
    }
    else if (op == "not_equal")
    {
        try_test( test_not_equal, number, a, b, show_all, timer);
    }
    else if (op == "shift_r")
    {
        try_test( test_shft_rght, number, a, b, show_all, timer);
    }
    else if (op == "shift_l")
    {
        try_test( test_shft_left, number, a, b, show_all, timer);
    }
    else if (op == "add_assign")
    {
        try_test( test_add_eqls, number, a, b, show_all, timer);
    }
    else if (op == "sub_assign")
    {
        try_test( test_sub_eqls, number, a, b, show_all, timer);
    }
    else if (op == "mult_assign")
    {
        try_test( test_mult_eqls, number, a, b, show_all, timer);
    }
    else if (op == "div_assign")
    {
        try_test( test_div_eqls, number, a, b, show_all, timer);
    }
    else if (op == "mod_assign")
    {
        try_test( test_mod_eqls, number, a, b, show_all, timer);
    }
    else if (op == "root")
    {
        try_test( test_root, number, a, b, show_all, timer);
    }
    else if (op == "sqrt")
    {
        try_test( test_sqrt, number, a, show_all, timer);
    }
    else if (op == "cbrt")
    {
        try_test( test_cbrt, number, a, show_all, timer);
    }
    else if (op == "abs")
    {
        try_test( test_abs, number, a, show_all, timer);
    }
    else if (op == "floor")
    {
        try_test( test_floor, number, a, show_all, timer);
    }
    else if (op == "ceil")
    {
        try_test( test_ceil, number, a, show_all, timer);
    }
    else
    {
        std::cout << "    \"" << op << "\" is an invalid command. Please try again.\n" << std::endl;
    }
}

static void perform_random_test(bool show_all = true)
{
    // how many switch cases are there?
    int types_of_tests = 29;
    // pick one at random
    int rand = rand_int(0, types_of_tests - 1);
    //int rand = rand_int(26, 28);

    switch (rand) {
        case 0:
            test_add(NULL, NULL, show_all); break;
        case 1:
            test_sub(NULL, NULL, show_all); break;
        case 2:
            test_mult(NULL, NULL, show_all); break;
        case 3:
            test_div(NULL, NULL, show_all); break;
        case 4:
            test_inc_prefix(NULL, show_all); break;
        case 5:
            test_inc_postfix(NULL, show_all); break;
        case 6:
            test_dec_prefix(NULL, show_all); break;
        case 7:
            test_dec_postfix(NULL, show_all); break;
        case 8:
            test_mod(NULL, NULL, show_all); break;
        case 9:
            test_pow(NULL, NULL, show_all); break;
        case 10:
            test_great(NULL, NULL, show_all); break;
        case 11:
            test_less(NULL, NULL, show_all); break;
        case 12:
            test_great_eql(NULL, NULL, show_all); break;
        case 13:
            test_less_eql(NULL, NULL, show_all); break;
        case 14:
            test_equal(NULL, NULL, show_all); break;
        case 15:
            test_not_equal(NULL, NULL, show_all); break;
        case 16:
            test_shft_rght(NULL, NULL, show_all); break;
        case 17:
            test_shft_left(NULL, NULL, show_all); break;
        case 18:
            test_add_eqls(NULL, NULL, show_all); break;
        case 19:
            test_sub_eqls(NULL, NULL, show_all); break;
        case 20:
            test_mult_eqls(NULL, NULL, show_all); break;
        case 21:
            test_div_eqls(NULL, NULL, show_all); break;
        case 22:
            test_mod_eqls(NULL, NULL, show_all); break;
        case 23:
            test_root(NULL, NULL, show_all); break;
        case 24:
            test_sqrt(NULL, show_all); break;
        case 25:
            test_cbrt(NULL, show_all); break;
        case 26:
            test_abs(NULL, show_all); break;
        case 27:
            test_floor(NULL, show_all); break;
        case 28:
            test_ceil(NULL, show_all); break;
        default:
            throw "types_of_tests does not correspond to number of cases in perform_random_test() in testing.cpp \n";
            break;
    }
}

void Testing::random_tester(int number, bool show_all, bool timer)
{
    Timer t;
    for (int i = 0; i < number; i++)
    {
        try {
            perform_random_test(show_all);
        }
        catch (std::exception& e)
        {
            std::cout << "    Issue with test number " << i + 1 << std::endl;
            std::cout << e.what() << std::endl;
        }
    }
    if(timer) timer_report(t, number);
}



template <typename T>
void pt(T n){
    std::cout << n << std::endl;
}

void Testing::demo ()
{
    std::cout << "All examples for initiating and using big number class bgnm" << std::endl;

    //temp stuff for examples
    std::string string1 = "5";
    std::string string2 = "6";
    std::string string3 = "11";
    bgnm bgnm1(7);
    bgnm bgnm2(8);
    bgnm bgnm3(12);
    bgnm bgnm4(25);
    bgnm pos_ten(10);
    bgnm neg_ten(-10);
    bgnm i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af;

    //OVERLOAD CONSTRUCTOR
    // constructor w int
    bgnm a(1.1);
    // constructor w = and int
    bgnm b = 2;
    // constructor w float
        //  0.00009876543000300405021
    bgnm b_float((float)66655577744488833399.9222000111);
    // constructor w double
    bgnm b_double((double)666555.777444888333999222000111);
    // constructor w long double
    long double ld = 66655.577744488833399922200011L;
    bgnm b_ldouble(ld);
    // constructor w Cstring
    bgnm c("3");
    // constructor w = and Cstring
    bgnm d = "4";
    // constructor w string
    bgnm e(string1);
    // constructor w = and string
    bgnm f = string2;
    // constructor w bgnm
    bgnm g(bgnm1);
    // constructor w = and bgnm
    bgnm h = bgnm2;

    // OVERLOAD = OPERATOR
    // overload = using int
    i = 9;
    // overload = using Cstring
    j = "10";
    // overload = using string
    k = string3;
    // overload = using bgnm
    l = bgnm3;

    // OVERLOAD + OPERATOR
    // overload + using int
    m = l + 1;
    // overload + using Cstring
    n = l + "2";
    // overload + using string
    o = j + string1;
    // overload + using bgnm
    p = o + a;

    // OVERLOAD - OPERATOR
    // overload - using int
    q = bgnm4 - 8;
    // overload - using Cstring
    r = bgnm4 - "7";
    // overload - using string
    s = bgnm4 - string2;
    // overload - using bgnm
    t = bgnm4 - e;

    //test addition and subtraction of positive and negative values
    // + + +
    u = pos_ten + "11";
    // + + -
    v = pos_ten + "-32";
    // - + +
    w = neg_ten + "33";
    // - + -
    x = neg_ten + "-14";
    // + - +
    y = pos_ten - "35";
    // + - -
    z = pos_ten - "-16";
    // - - +
    aa = neg_ten - "17";
    // - - -
    ab = neg_ten - "-38";

    //OVERLOAD * OPERATOR
    //overload * using int
    bgnm yy("12");
    bgnm zz("-1242.9484");
    ac = yy * zz;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << b_float << std::endl;
    std::cout << b_double << std::endl;
    std::cout << b_ldouble << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;
    std::cout << e << std::endl;
    std::cout << f << std::endl;
    std::cout << g << std::endl;
    std::cout << h << std::endl;
    std::cout << i << std::endl;
    std::cout << j << std::endl;
    std::cout << k << std::endl;
    std::cout << l << std::endl;
    std::cout << m << std::endl;
    std::cout << n << std::endl;
    std::cout << o << std::endl;
    std::cout << p << std::endl;
    std::cout << q << std::endl;
    std::cout << r << std::endl;
    std::cout << s << std::endl;
    std::cout << t << std::endl;
    std::cout << u << std::endl;
    std::cout << v << std::endl;
    std::cout << w << std::endl;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    std::cout << z << std::endl;
    std::cout << aa << std::endl;
    std::cout << ab << std::endl;
    std::cout << ac << std::endl;

    std::cout << sizeof(float) << std::endl;
    std::cout << sizeof(double) << std::endl;
    std::cout << sizeof(long double) << std::endl;
    std::cout << FLT_EPSILON << std::endl;
    std::cout << DBL_EPSILON << std::endl;
    std::cout << LDBL_EPSILON << std::endl;
    int xxx = 0; double ddd;
    ddd = frexp(ld, &xxx);

    std::ostringstream bla;
    bla << std::fixed;
    bla << std::setprecision(2);
    bla << pow(2,xxx);
    std::cout << bla.str() << std::endl;
    int places = floor(xxx * 0.30102999566); // convert from binary exponent to base ten exponent
    //int precision = 19;
    //int rightofdecimal = precision - places;
    std::cout << "digits: " << places << std::endl;
    std::cout << ddd << " - " << xxx << std::endl;

    bgnm s1 = "-161.6900000", s2 = "1723500000.", s3;

    //0.0000000938149115172613867130838410211778357992457209167391935015955903684363214389324049898462431099
    //    //0.000000093814911517261386713083840
    s3 = s1 / s2;
    std::cout << s3 << '=' << s1 << '/'<<s2<< std::endl;

    bgnm b00 ((float)0.31415926535897932384626433832795028841971);
    bgnm b0  ((float).31415926535897932384626433832795028841971);
    bgnm b1  ((float)3.1415926535897932384626433832795028841971);
    bgnm b2  ((float)314.15926535897932384626433832795028841971);
    bgnm b3  ((float)31415.926535897932384626433832795028841971);
    bgnm b4  ((float)3141592.6535897932384626433832795028841971);
    bgnm b5  ((float)314159265.35897932384626433832795028841971);
    bgnm b6  ((float)31415926535.897932384626433832795028841971);
    bgnm b7  ((float)3141592653589.7932384626433832795028841971);
    bgnm b8  ((float)314159265358979.32384626433832795028841971);
    bgnm b9  ((float)31415926535897932.384626433832795028841971);
    bgnm b10 ((float)314159265358979323.84626433832795028841971);
    bgnm b11 ((float)31415926535897932384.626433832795028841971);
    bgnm b12 ((float)3141592653589793238462.6433832795028841971);

    bgnm d00 ((double)0.31415926535897932384626433832795028841971);
    bgnm d0  ((double).31415926535897932384626433832795028841971);
    bgnm d1  ((double)3.1415926535897932384626433832795028841971);
    bgnm d2  ((double)314.15926535897932384626433832795028841971);
    bgnm d3  ((double)31415.926535897932384626433832795028841971);
    bgnm d4  ((double)3141592.6535897932384626433832795028841971);
    bgnm d5  ((double)314159265.35897932384626433832795028841971);
    bgnm d6  ((double)31415926535.897932384626433832795028841971);
    bgnm d7  ((double)3141592653589.7932384626433832795028841971);
    bgnm d8  ((double)314159265358979.32384626433832795028841971);
    bgnm d9  ((double)31415926535897932.384626433832795028841971);
    bgnm d10 ((double)314159265358979323.84626433832795028841971);
    bgnm d11 ((double)31415926535897932384.626433832795028841971);
    bgnm d12 ((double)3141592653589793238462.6433832795028841971);
    
    bgnm ld00 ((long double)0.031415926535897932384626433832795028841971);
    bgnm ld0  ((long double).31415926535897932384626433832795028841971);
    bgnm ld1  ((long double).00000031415926535897932384626433832795028841971);
    bgnm ld2  ((long double)314.15926535897932384626433832795028841971);
    bgnm ld3  ((long double)31415.926535897932384626433832795028841971);
    bgnm ld4  ((long double)3141592.6535897932384626433832795028841971);
    bgnm ld5  ((long double)314159265.35897932384626433832795028841971);
    bgnm ld6  ((long double)31415926535.897932384626433832795028841971);
    bgnm ld7  ((long double)3141592653589.7932384626433832795028841971);
    bgnm ld8  ((long double)314159265358979.32384626433832795028841971);
    bgnm ld9  ((long double)31415926535897932.384626433832795028841971);
    bgnm ld10 ((long double)314159265358979323.84626433832795028841971);
    bgnm ld11 ((long double)31415926535897932384.626433832795028841971);
    bgnm ld12 ((long double)3141592653589793238462.6433832795028841971);

    pt("3.1415926535897932384626433832795028841971");
    pt(b00);
    pt(b0);
    pt(b1);
    pt(b2);
    pt(b3);
    pt(b4);
    pt(b5);
    pt(b6);
    pt(b7);
    pt(b8);
    pt(b9);
    pt(b10);
    pt(b11);
    pt(b12);

    pt("3.1415926535897932384626433832795028841971");
    pt(d00);
    pt(d0);
    pt(d1);
    pt(d2);
    pt(d3);
    pt(d4);
    pt(d5);
    pt(d6);
    pt(d7);
    pt(d8);
    pt(d9);
    pt(d10);
    pt(d11);
    pt(d12);

    pt("3.1415926535897932384626433832795028841971");
    pt(ld00);
    pt(ld0);
    pt(ld1);
    pt(ld2);
    pt(ld3);
    pt(ld4);
    pt(ld5);
    pt(ld6);
    pt(ld7);
    pt(ld8);
    pt(ld9);
    pt(ld10);
    pt(ld11);
    pt(ld12);
        
    bgnm bn, bn2 = "-9834850322.09820935245";
    pt(ld8);
    bn = ld8 + (long double)314.1592653589793238462643383;
    std::cout << bn << std::endl;
    bn--;
    std::cout << bn << std::endl;
    bn++;
    std::cout << bn << std::endl;
        
    Bgnm obj(3);
    Bgnm obj1(3);
    pt(obj);
    pt(obj1);

    // Operator function is called, only then value of obj is assigned to obj1
    obj1 = ++obj;
    pt(obj);
    pt(obj1);

    // Assigns value of obj to obj1, only then operator function is called.
    obj1 = obj++;
    pt(obj);
    pt(obj1);

    obj++;
    obj1++;
    pt(obj);
    pt(obj1);
    pt(obj++);
    pt(obj1++);
    pt(obj);
    pt(obj1);
    
    
    
    //    Bgnm base("922337492837413");
    //    int mod = 25;
    //    Bgnm answer = base % mod;
    //    std::cout << base << " mod " << mod << " = " << answer << std::endl;
    //
    //    Bgnm base2 = "-2.34", answer2;
    //    //float power = 3;
    //    //answer2 = base2 ^ power;
    //    //std::cout << base2 << " ^ " << power << " = " << answer2 << std::endl;
    //
    //    Bgnm t = "8", y = "20";
    //    std::cout << t << " / " << y << " = " << t/y << std::endl;
        
    //    Bgnm nega = ".081", negb = ".081";
    //    if(nega != negb){
    //        std::cout << nega << " != " << negb << " : TRUE" << std::endl;
    //    } else {
    //        std::cout << nega << " != " << negb << " : FALSE" <<  std::endl;
    //    }
        
    //    Bgnm radicant = -30.12352,answer3;
    //    float index = 8.125;
    //
    //    answer3 = radicant ^ index;
    //    std::cout << index << " root of " << radicant << " = " << answer3 << "\n";
    //
    //    Bgnm::set_bgnm_internal_precision_limit(30);
    //
    //    Bgnm big = "-.002", small = "-.002";
    //    if(big >= small){
    //        std::cout << "big is bigger than small\n";
    //    } else {
    //        std::cout << "big is NOT bigger\n";
    //    }
    //    bgnm sr = "81", cr = "-81";
    //    std::cout << "square root of " << sr << " is " << sr.sqrt() << std::endl;
    //    std::cout << "cube root of " << cr << " is " << cr.cbrt() << std::endl;
        
    //    bgnm                bg = "-4.004";
    //    int            integer = bg.to_int();
    //    long long     longlong = bg.to_long_long();
    //    float         floating = bg.to_float();
    //    double        doubling = bg.to_double();
    //    long double longdouble = bg.to_long_double();
    //    char*          cstring = bg.to_c_string();
    //    std::string  stdstring = bg.to_string();
        
    //    p(integer);
    //    p(longlong);
    //    p(floating);
    //    p(doubling);
    //    p(longdouble);
    //    p(cstring);
    //    p(stdstring);
    //    p("----------------");
    //    p(bg.ceil());
    //    p(bg.floor());

    //        std::string tempy = "12344";
    //        Bgnm bgnm100="12345";
    //        //bgnm100 == "123..23";
    //        bgnm100 *= tempy;
    //        //p(bgnm100);
    //
    //

        

    //    bgnm bn("7");
    //    double db = -7;
    //    double temp = bn.to_double();
    //    double diff = (temp - db)/db;
    //
    //    std::cout << "bn   : " << bn << std::endl;
    //    std::cout << "temp : " << temp << std::endl;
    //    std::cout << "db   : " << db << std::endl;
    //    std::cout << "diff : " << diff << std::endl;
    //
    
}


