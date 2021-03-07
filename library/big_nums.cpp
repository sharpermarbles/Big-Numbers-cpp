//
//  big_nums.cpp
//  big_nums
//
//  Created by Ben Harvey on 10/10/20.
//  Copyright © 2020 Ben Harvey. All rights reserved.

#include <iostream>
#include <sstream>
#include <iomanip>   // for std::setprecision()
#include <vector>
#include <math.h>    // for log10() and frexp() (get exponenet of float)
#include <stdexcept> // for Bgnm_error class to inherit std::runtime_error from std::exception
#include <string.h>  // to convert __FILE__ to __FILENAME__

#include "big_nums.hpp"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) // use __FILENAME__ in lieu of __FILE__ if file path not desired

// error class derived from std::runtime_error
class Bgnm_error : public virtual std::runtime_error
{
public:
    std::string file;
    int line;
    std::string function;
    int error_type;
    std::string input_was;
    std::string explanation;
    std::string print_out;
    char text[400];
    Bgnm_error(const char * e, const char * fi, int l, const char * fu, int e_type = 1) : std::runtime_error (e)
    {
        explanation = e;
        file = fi;
        line = l;
        function = fu;
        error_type = e_type;
        std::string ln = std::to_string(line), num = std::to_string(error_type);
        print_out = "Bgnm ERROR " + num + ": " + explanation + " Exception thrown in file " + file + ", line " + ln + ", function: " + function + "().";
    }
    const char* what() const noexcept
    {
        return print_out.c_str();
    }
};

//helper struct for Bgnm::???_num_strings() functions
struct Num_str
{
    
    friend Bgnm;
    
    std::string num;
    int bef_dec; // places before decimal
    int dec_loc; // decimal location
    int aft_dec; // places after decimal
    int size; // count of places in number
    bool neg;// negative?
    
    Num_str(const std::string & s, bool clean = false)
    {
        num = s;
        neg = false;
        if(s[0]=='-')
        {
            neg = true;
            num[0]= '0';
        }
        size=(int)s.size();
        dec_loc = locate_decimal(num);
        bef_dec = dec_loc;
        aft_dec = size-(dec_loc + 1);
        if(aft_dec < 0) aft_dec=0;
        if(clean)
        {
            for(int i=0;i<num.size();i++)
            {
                if(s[i]=='.')
                {
                    num.erase(i,1);
                }
            }
            size=(int)num.size();
            aft_dec = size-dec_loc;
        }
    }
    
    ~Num_str(){}
    
    int locate_decimal(std::string & s)
    {
        int i = 0;
        for(std::string::iterator it=s.begin();it!=s.end();it++)
        {
            if(s[i] == '.')return i;
            i++;
        }
        return i;
    }
    
    //add zeros to end of num string
    void add_zeros(const int zero_amnt)
    {
        char c = '0';
        if(aft_dec<1)
        {
            bef_dec += zero_amnt;
            size    += zero_amnt;
        }
        else
        {
            aft_dec += zero_amnt;
            size    += zero_amnt;
        }
        for(int x=0; x< zero_amnt; x++)
        {
            num.append(&c,1);
        }
        dec_loc = locate_decimal( num);
    }
    
    //subtracts a substring number from a number string by swaping digits w zeros
    void zero_out(const int & start, const int & howmany)
    {
        int last=(int)num.size();
        for(int i=0;i<howmany;i++)
        {
            if((start+i)>=last) throw Bgnm_error("Num_str::zero_out() - Went out of range.",__FILENAME__,__LINE__,__FUNCTION__,110);
            num[start+i]= '0';
        }
    }
    
};

//helper functions
//static int max(int a, int b){
//   if (a > b) return a; return b;
//}

//static void add_to_vector(std::vector<int> & vec, const Num_str & ns, const int bef){
//    int start = bef - ns.bef_dec;
//    for (int i = 0; i < ns.num.size();i++){
//        if(ns.num[i] != '.'){
//            vec[start] += ns.num[i] - '0';
//            start++;
//        }
//    }
//}
//
//static void sub_from_vector(std::vector<int> & vec, Num_str & ns, const int bef){
//    int start = bef - ns.bef_dec;
//    for (int i = 0; i < ns.num.size();i++){
//        if(ns.num[i] != '.'){
//            vec[start] -= ns.num[i] - '0';
//            start++;
//        }
//    }
//}

// carry() version 2.0
// note that carry() will not carry off the end of the vector - if number carries off either end of vector, it is lost
static void carry(std::vector<int> & vec, bool reverse = false)
{
    int x, size = (int)vec.size();
    if(!reverse)
    {
        for (int it = size-1; it >=0; it--)
        {
            if(vec[it] > 9)
            {
                x = vec[it];
                vec[it] = x%10;
                if(it > 0)
                {
                    vec.at(it-1)+=(x-vec[it])/ 10;
                }
            }
        }
    }
    else
    {
        for (int it = 0; it < size; it++)
        {
            if(vec[it] > 9)
            {
                x = vec[it];
                vec[it]=x%10;
                if(it < size - 1)
                {
                    vec.at(it+1)+=(x-vec[it])/ 10;
                }
            }
        }
    }
}

static void vec_to_string(const std::vector<int> & vec, std::string & str, bool reverse = false)
{
    if(!reverse)
    {
        for(int i = 0; i < vec.size(); i++)
        {
            str += (char)(vec[i] + '0');
        }
    }
    else
    {
        for(int i = (int)vec.size() -1; i >= 0; i--)
        {
            str += (char)(vec[i] + '0');
        }
    }
}

static void add_dec(std::string &str, const int bef)
{
    if(bef < str.size())
    {
        str.insert(bef,1,'.');
    }
}

static void strip_leading_0s(std::string &str)
{
    int i = 0,count = 0;
    while(str[i]=='0')
    {
        if(str[i+1] != '.') count++;
        i++;
    }
    if(str.size() >= count) str.erase(0,count);
    if(str.size() == 0) str = "0";
}

/* default turns 1230.0000 to 1230.0
 strip_trailing_0s("1230.000")           = 1230.0
 strip_trailing_0s("1230.000",true)      = 1230
 strip_trailing_0s("1230.000",false,true)= 1230.
 strip_trailing_0s("1230.000",true,true) = 123
 strip_trailing_0s("1230000")            = 1230
 strip_trailing_0s("1230000",true)       = 1230
 strip_trailing_0s("1230000",false,true) = 123
 strip_trailing_0s("1230000",true,true)  = 123
 */
static void strip_trailing_0s(std::string & str, bool integer = false, bool total = false)
{
    if(str[str.size()-1]=='0' && str[str.size()-2]=='0')
    {
        str.erase(str.size()-1,1);
        strip_trailing_0s(str);
    }
    if(integer)
    {
        if(str[str.size()-1] == '0' && str[str.size()-2] == '.')
        {
            str.erase(str.size()-1);
            str.erase(str.size()-1);
        }
    }
    if(total)
    {
        if(str[str.size()-1]=='0')
        {
            str.erase(str.size()-1,1);
            strip_trailing_0s(str,false,true);
        }
    }
}

//provide how many digits you want to the right of the decimal, string number will be rounded to that decimal place. Default rounds number to integer.
static void round_string(std::string & str, const int max_dec_prec = 0)
{
    
    int where_dec = (int)str.find('.');
    
    if(where_dec != std::string::npos)
    {
        int location = where_dec + max_dec_prec + 1;
        
        if(str.size()>=location+1)
        { // only delete digits, if string is big enough in the first place
            // if() here is to decide if need to round up least significant digits before deleting right end of result string
            if(str[location] >= '5')
            { //check if need to round up
                int p = 1;
                while(true)
                {
                    if(str[location-p] == '.')
                    { // skip over decimal
                        p++;
                    }
                    if( location-p < 0)
                    { // carrying off the left end of the string
                        str.insert(0,"1");
                        location++;
                        break;
                    }
                    if(str[location-p]!='9')
                    {
                        //make sure not out of range before changing something
                        str[location-p]++;
                        break;
                    }
                    else
                    {
                        str[location-p]='0';
                    }
                    p++;
                }
            }
            str.erase(location,std::string::npos); // delete rest of string to the right
        }
    }
    if(str[str.size()-1]=='.') str.erase(str.size()-1);
}

// Provide how many digits you want to the right of the decimal, string number will be rounded to that decimal place. Default rounds number to integer.
// This version of the function assumes an extra digit to the left end of the string for final carry if needed. Undefined behavior may result if this extra '0' isn't there.
static void round_string(char* str, const int max_dec_prec = 0)
{
    
    // first see if there is a decimal
    unsigned null_loc = 0;
    int dec_loc = 0xffffffff;
    while (str[null_loc] != '\0')
    {
        if(str[null_loc] == '.') dec_loc = null_loc;
        null_loc++;
    }
    unsigned str_size = null_loc;
    auto where_dec = dec_loc;
    int location = where_dec + max_dec_prec + 1;
    
    if(where_dec != 0xffffffff)
    {
        
        if(str_size >= location+1)
        { // only delete digits, if string is big enough in the first place
            // if() here is to decide if need to round up least significant digits before deleting right end of result string
            if(str[location] >= '5')
            { //check if need to round up
                int p = 1;
                while(true)
                {
                    if(str[location-p] == '.')
                    { // skip over decimal
                        p++;
                    }
                    if( location-p < 0)
                    { // carrying off the left end of the string
                        //str.insert(0,"1");
                        // NEED TO FIGURE OUT WHAT TO DO HERE? COPY TO A NEW STRING WITH CARRY?
                        location++;
                        break;
                    }
                    if(str[location-p]!='9')
                    {
                        //make sure not out of range before changing something
                        str[location-p]++;
                        break;
                    }
                    else
                    {
                        str[location-p]='0';
                    }
                    p++;
                }
            }
            str[location] = '\0'; // delete rest of string to the right
        }
    }
    if(str[location-1]=='.') str[location-1] = '\0';
}

//static void tens_comp_stp1(std::vector<int> & vec){
//    int x = 10;
//    for(int i=(int)vec.size()-1;i>=0;i--){
//        vec[i] += x;
//        x = 9;
//    }
//}


// compare two string numbers (integer or floating point) return true if a is larger or equal
// to force (a > b) in lieu of (a >= b), set strict_comp to true
// note - undefined behavior if comparing a negative zero to a possitive zero
static bool comp_strs(const std::string_view a_str, const std::string_view b_str, bool strict_comp = false)
{
    
    //note - this will
    if(a_str[0]!='-' && b_str[0]=='-') return true;
    if(a_str[0]=='-' && b_str[0]!='-') return false;
    
    std::string_view a = a_str;
    std::string_view b = b_str;
    
    //If both a and b are negative then the 'smaller' number is greater : switch a and b
    if(a[0]=='-' && b[0]=='-')
    {
        a = b_str; b = a_str;
        a.remove_prefix(1); b.remove_prefix(1); //removes '-'
    }
    
    //compare integers
    std::string_view a_int, b_int, a_dec, b_dec;
    int adl = (int) a.find('.'); // a decimal location
    int bdl = (int) b.find('.'); // b decimal location
    
    a_int = a.substr(0,adl);
    b_int = b.substr(0,bdl);
    a.find('.') != std::string::npos ? a_dec = a.substr(adl+1) : a_dec = a.substr(a.size());
    b.find('.') != std::string::npos ? b_dec = b.substr(bdl+1) : b_dec = b.substr(b.size());
    
    //compare a and b integer components
    int diff = (int)a_int.size() - (int)b_int.size();
    int a_shift=0, b_shift=0, i=0;
    std::string_view long_int = a_int; // assume that a is longer, has the panhandle
    bool response = true;
    int int_size = (int)a_int.size();
    if (int_size < b_int.size()) int_size = (int)b_int.size();
    if(diff > 0)
    { // if a_int is longer, has a panhandle
        b_shift = -diff;
    }
    else if (diff < 0)
    { // b_int is longer, has a panhandle
        a_shift = diff;
        long_int = b_int;
        response = false;
    }
    for(;i<abs(diff);i++)
    { // check the panhadle, if > 0, means true (if a) false (if b)
        if(long_int[i] > '0') return response;
    }
    for(;i<int_size;i++)
    {
        if(a_int[i+a_shift] > b_int[i+b_shift]) return true;
        if(a_int[i+a_shift] < b_int[i+b_shift]) return false;
    }
    
    // compare a and b decimal components
    diff = (int)a_dec.size() - (int)b_dec.size();
    i=0;
    std::string_view long_dec = a_dec; // assume that a is longer, has the panhandle
    response = true;
    int dec_size = (int)a_dec.size();
    if (dec_size > b_dec.size()) dec_size = (int)b_dec.size();
    for(;i<dec_size;i++)
    {
        if(a_dec[i] > b_dec[i]) return true;
        if(a_dec[i] < b_dec[i]) return false;
    }
    if(diff < 0)
    { // b_dec is longer, has a panhandle
        long_dec = b_dec;
        response = false;
    }
    for(;i < dec_size + abs(diff);i++)
    { // check the panhadle, if > 0, means true (if a) false (if b)
        if(long_dec[i] > '0') return response;
    }
    
    // if function has reached this point without returning, then apparently a == b, therefore
    if (strict_comp)
    {
        return false; // if strict_compare, a and b are not allowed to be equal
    }
    else
    {
        return true; // if not strict compare (default), return true ( a == b )
    }
}

// compare two string numbers (integer or floating point) return true if a is numerically equal to b
static bool comp_strs_equal(const std::string_view & a_str, const std::string_view & b_str, const unsigned int precision = 0xffffffff)
{
    //note - precision sets how many digits right of decimal must be equal for the function to return true
    
    //note - undefined behavior if -0 is provided for either number
    if(a_str[0]!='-' && b_str[0]=='-') return false;
    if(a_str[0]=='-' && b_str[0]!='-') return false;
    
    std::string_view a = a_str;
    std::string_view b = b_str;
    
    //If both a and b are negative then negatives not needed for comparison
    if(a[0]=='-' && b[0]=='-')
    {
        a.remove_prefix(1); b.remove_prefix(1); //removes '-'
    }
    
    //compare integers
    std::string_view a_int, b_int, a_dec, b_dec;
    int adl = (int) a.find('.'); // a decimal location
    int bdl = (int) b.find('.'); // b decimal location
    
    a_int = a.substr(0,adl);
    b_int = b.substr(0,bdl);
    a.find('.') != std::string::npos ? a_dec = a.substr(adl+1) : a_dec = a.substr(a.size());
    b.find('.') != std::string::npos ? b_dec = b.substr(bdl+1) : b_dec = b.substr(b.size());
    
    //compare a and b integer components
    int diff = (int)a_int.size() - (int)b_int.size();
    int a_shift=0, b_shift=0, i=0;
    std::string_view long_int = a_int; // assume that a is longer, has the panhandle
    int int_size = (int)a_int.size();
    if (int_size < b_int.size()) int_size = (int)b_int.size();
    if(diff > 0)
    { // if a_int is longer, has a panhandle
        b_shift = -diff;
    }
    else if (diff < 0)
    { // b_int is longer, has a panhandle
        a_shift = diff;
        long_int = b_int;
    }
    for(;i<abs(diff);i++)
    {
        if(long_int[i] > '0') return false;// check the panhadle, if > 0, means true (if a) false (if b)
    }
    for(;i<int_size;i++)
    {
        if(a_int[i+a_shift] > b_int[i+b_shift] || a_int[i+a_shift] < b_int[i+b_shift]) return false;
    }
    
    // compare a and b decimal components
    diff = (int)a_dec.size() - (int)b_dec.size();
    a_shift=0; b_shift=0; i=0;
    std::string_view long_dec = a_dec; // assume that a is longer, has the panhandle
    int dec_size = (int)a_dec.size();
    if (dec_size > b_dec.size()) dec_size = (int)b_dec.size();
    for(;i<dec_size && i<precision;i++)
    {
        if(a_dec[i] > b_dec[i] || a_dec[i] < b_dec[i]) return false;
    }
    if(diff < 0) long_dec = b_dec; // b_dec is longer, has a panhandle
    for(;i < dec_size + abs(diff) && i < precision;i++)
    { // check the panhadle, if > 0, means true (if a) false (if b)
        if(long_dec[i] > '0') return false;
    }
    
    // if function has reached this point without returning, then apparently a == b
    return true;
}

//forward declare sub_num_strings() (note strip_zeros = true)
static std::string sub_num_strings(std::string_view num1, std::string_view num2);

// ADDITION
//static std::string add_num_strings( const std::string & num1, const std::string & num2, bool strip_zeros = true){
//    int bef, aft, vec_max;
//    std::string result;
//
//    Num_str n1(num1);
//    Num_str n2(num2);
//
//    //check for negatives
//    if(n1.neg==true && n2.neg==true){
//        std::string s = "-" + add_num_strings(n1.num,n2.num, strip_zeros);
//        return s;
//    } else if(n1.neg==true){
//        return sub_num_strings(n2.num,n1.num, strip_zeros);
//    } else if(n2.neg==true){
//        return sub_num_strings(n1.num,n2.num, strip_zeros);
//    }
//
//    bef = max(n1.bef_dec, n2.bef_dec) + 1; // 1 extra for carry
//    aft = max(n1.aft_dec, n2.aft_dec);
//    vec_max = bef + aft;
//
//    std::vector<int> digits (vec_max);
//    add_to_vector(digits, n1, bef);
//    add_to_vector(digits, n2, bef);
//
//    carry(digits);
//    vec_to_string(digits, result);
//    add_dec(result, bef);
//    if(strip_zeros) {
//        strip_leading_0s(result);
//    } else {
//        if(result[0]=='0' && result[1]!='.') result.erase(0,1);
//    }
//    return result;
//}
static std::string add_num_strings(std::string_view a, std::string_view b)
{
    
    //check if a or b or both are negative
    if(a[0]=='-' && b[0]=='-')
    {
        a = a.substr(1);
        b = b.substr(1);
        return "-" + add_num_strings(a,b);
    }
    else if(a[0]=='-')
    {
        a = a.substr(1);
        return sub_num_strings(b,a);
    }
    else if(b[0]=='-')
    {
        b = b.substr(1);
        return sub_num_strings(a,b);
    }
    
    //declare string views to be used for integer and decimal portions of each number
    std::string_view a_int, b_int, a_dec, b_dec;
    auto adl = a.find('.'); // a decimal location
    auto bdl = b.find('.'); // b decimal location
    
    a_int = a.substr(0,adl);
    b_int = b.substr(0,bdl);
    adl != std::string::npos ? a_dec = a.substr(adl+1) : a_dec = a.substr(a.size());
    bdl != std::string::npos ? b_dec = b.substr(bdl+1) : b_dec = b.substr(b.size());
    
    int max_int = (int)a_int.size();
    if(b_int.size() > max_int) max_int = (int)b_int.size();
    
    std::string_view first_dec = a_dec, second_dec = b_dec;
    
    int max_dec = (int)a_dec.size();
    if(b_dec.size() > max_dec)
    {
        max_dec = (int)b_dec.size();
        first_dec = b_dec; second_dec = a_dec;
    }
    
    //char array "digits" shall have a carry digit, integer digits, digit for decimal, and decimal digits (and then be null termianted)
    int digit_count = 1 + max_int;
    if(max_dec > 0) digit_count += 1 + max_dec;
    int digit_cursor = digit_count-1;
    
    //create char array of digits
    char digits[digit_count + 1]; digits[digit_count] = '\0';
    char *digits_p = digits; //pointer to digits array
    for(int i=0;i<digit_count;i++) digits[i] = '0';
    if(max_dec > 0) digits[1+max_int] = '.';
    
    //add first number to digits
    int dec_cursor = max_dec-1;
    for(; digit_cursor > max_int +1; digit_cursor--) // loop from right side until hit the decimal
    {
        digits[digit_cursor] += first_dec[dec_cursor] - '0';
        if(dec_cursor < second_dec.size()) digits[digit_cursor] += second_dec[dec_cursor] - '0';
        dec_cursor--;
    }
    if(max_dec > 0) digit_cursor--; //skip the decimal (if there is one)
    int a_int_cursor = (int)a_int.size() -1, b_int_cursor = (int)b_int.size() -1;
    for(; digit_cursor >= 0; digit_cursor--) // now loop through left (integer) side of decimal
    {
        if(a_int_cursor > -1) digits[digit_cursor] += a_int[a_int_cursor] - '0';
        if(b_int_cursor > -1) digits[digit_cursor] += b_int[b_int_cursor] - '0';
        a_int_cursor--;
        b_int_cursor--;
    }
    
    
    //carry
    for(int i = digit_count-1; i > 0;i--)
    {
        if(i != max_int + 1) // avoid the decimal
        {
            if(digits[i]-'9' > 0)
            {
                i-1 == max_int + 1 ? digits[i-2] ++: digits[i-1] ++; // carry left, except skip decimal
                digits[i] = ( (digits[i] - '0') % 10) + '0'; //leave this digit as number mod 10
            }
        }
    }
    
    // get rid of first "carry" digit if still zero
    if(digits_p[0]=='0' && digits_p[1]!='.') digits_p++;
    
    return digits_p;
}

// SUBTRACT (this function is forward-declared above add_num_strings() )
//static std::string sub_num_strings( const std::string & num1, const std::string & num2, bool strip_zeros){
//    int bef, aft, vec_max;
//    std::string result;
//
//    Num_str n1(num1);
//    Num_str n2(num2);
//
//    //check for negatives
//    if(n1.neg==true && n2.neg==true){
//        return sub_num_strings(n2.num,n1.num, strip_zeros);
//    } else if(n1.neg==true){
//        std::string s = "-" + add_num_strings(n1.num,n2.num, strip_zeros);
//        return s;
//    } else if(n2.neg==true){
//        return add_num_strings(n1.num,n2.num, strip_zeros);
//    }
//
//    bef = max(n1.bef_dec, n2.bef_dec) + 1; // 1 extra for carry
//    aft = max(n1.aft_dec, n2.aft_dec);
//    vec_max = bef + aft;
//
//    std::vector<int> digits (vec_max);
//    add_to_vector(digits, n1, bef);
//
//    //compute 10's compliment in two steps
//    tens_comp_stp1(digits);
//    sub_from_vector(digits, n2, bef);
//
//    carry(digits);
//
//    vec_to_string(digits, result);
//
//    //check to see if negative - if so, do 10s compliment again
//    bool negative = false;
//    if(digits[0]==9){
//        int comp = 10 + '0';
//        for(int i=(int)result.size()-1;i>=0;i--){
//            result[i]=comp-result[i] + '0';
//            comp = 9 + '0';
//        }
//        negative=true;
//    }
//
//    add_dec(result, bef);
//    if(strip_zeros){
//        strip_leading_0s(result);
//    } else {
//        if(result[0]=='0' && result.size()>1){
//            result.erase(0,1);
//        }
//    }
//    if(negative)result = "-" + result;
//
//    return result;
//}
static std::string sub_num_strings(std::string_view a, std::string_view b)
{
    
    //check if a or b or both are negative
    if(a[0]=='-' && b[0]=='-')
    {
        a = a.substr(1);
        b = b.substr(1);
        return sub_num_strings(b,a);
    }
    else if(a[0]=='-')
    {
        a = a.substr(1);
        return "-" + add_num_strings(a,b);
    }
    else if(b[0]=='-')
    {
        b = b.substr(1);
        return add_num_strings(a,b);
    }
    
    //now a must be larger than or equal to b - otherwise, switch the numbers and recursively call sub_num_strings on reversed numbers
    bool switched = false;
    if(!comp_strs(a,b))
    {
        std::string_view temp = a;
        a = b;
        b = temp;
        switched = true;
    }
    
    //declare string views to be used for integer and decimal portions of each number
    std::string_view a_int, b_int, a_dec, b_dec;
    auto adl = a.find('.'); // a decimal location
    auto bdl = b.find('.'); // b decimal location
    
    a_int = a.substr(0,adl);
    b_int = b.substr(0,bdl);
    adl != std::string::npos ? a_dec = a.substr(adl+1) : a_dec = a.substr(a.size());
    bdl != std::string::npos ? b_dec = b.substr(bdl+1) : b_dec = b.substr(b.size());
    
    int max_int = (int)a_int.size();
    if(b_int.size() > max_int) max_int = (int)b_int.size();
    
    int max_dec = (int)a_dec.size();
    if(b_dec.size() > max_dec) max_dec = (int)b_dec.size();
    
    //char array "digits" shall have a carry digit, integer digits, digit for decimal, and decimal digits (and then be null termianted)
    int digit_count = 1 + max_int; // includes an extra character for a possible negative sign if a and b are "switched"
    if(max_dec > 0) digit_count += 1 + max_dec; // includes an extra character for the decimal
    int digit_cursor = digit_count-1;
    
    //create char array of digits
    char digits[digit_count + 1]; digits[digit_count] = '\0';
    char *digits_p = digits; //pointer to digits array - used to chop off extra leading char if not needed
    for(int i=0;i<digit_count;i++) digits[i] = '0';
    if(max_dec > 0) digits[max_int + 1] = '.';
    
    //add a to digits and subtract b
    int dec_cursor = max_dec-1;
    for(; digit_cursor > max_int + 1; digit_cursor--) // loop from right side until hit the decimal
    {
        if(dec_cursor < a_dec.size()) digits[digit_cursor] += a_dec[dec_cursor] - '0';
        if(dec_cursor < b_dec.size()) digits[digit_cursor] -= b_dec[dec_cursor] - '0';
        dec_cursor--;
    }
    if(max_dec > 0) digit_cursor--; //skip the decimal (if there is one)
    int a_int_cursor = (int)a_int.size() -1, b_int_cursor = (int)b_int.size() -1;
    for(; digit_cursor >= 0; digit_cursor--) // now loop through left (integer) side of decimal
    {
        if(a_int_cursor > -1) digits[digit_cursor] += a_int[a_int_cursor] - '0';
        if(b_int_cursor > -1) digits[digit_cursor] -= b_int[b_int_cursor] - '0';
        a_int_cursor--;
        b_int_cursor--;
    }
    
    
    // carry to the right
    for(int i = digit_count-1; i > 0;i--)
    {
        if(i != max_int + 1) // avoid the decimal
        {
            if(digits[i] < '0')
            {
                i-1 == max_int + 1? digits[i-2] --: digits[i-1] --; // steal from digit to the left, except skip decimal
                digits[i] += 10; //add 10 to this digit
            }
        }
    }
    
    // get rid of first "sign" digit if a and b are not switched (if a >= b)
    if(!switched)
    {
        digits_p++;
        return digits_p;
    }
    else
    {
        digits[0] = '-';
        return digits;
    }
}

///MULTIPLY
//static std::string mult_num_strings( const std::string & num1, const std::string & num2, const int max_dec_prec = DONT_ADJUST_PRECISION){
//    int aft, vec_max;
//    std::string result;
//
//    Num_str n1(num1, true);
//    Num_str n2(num2, true);
//
//    //check for negatives
//    bool negative = false;
//    if(n1.neg==true && n2.neg==true){
//        negative = false;
//    } else if(n1.neg==true || n2.neg==true){
//        negative = true;
//    }
//
//    //count dec place from right
//    aft = n1.aft_dec + n2.aft_dec;
//    //maximum size of product
//    vec_max = n1.size + n2.size + 1;
//    std::vector<int> digits (vec_max);
//
//    //multiply loop
//    int offset=0;
//    int buff;
//    for(int x=n2.size -1;x>=0;x--){
//        for(int y=n1.size -1;y>=0;y--){
//            if(n1.num[y]==0){
//                n1.num[y]='0';
//            }
//            buff = (n1.num[y]-'0')*(n2.num[x]-'0');
//
//            digits[vec_max-1-(n1.size-1-y)-offset]+=buff;
//        }
//        offset++;
//    }
//
//    carry(digits);
//    vec_to_string(digits, result);
//    //add decimal to string
//    int bef = (int)result.size()-aft;
//    add_dec(result,bef);
//
//    strip_leading_0s(result);
//    if(negative)result = "-" + result;
//
//    // Following code is ignored by default. It can be used to set precision level of result. Bignum library needs this function for loops that call div_num_strings() because decimal precision grows with each consecutive division due to the step where precision zeros are added to the numerator. In such cases, this trimming is needed to prevent adding decimal places which with each loop.
//    if(max_dec_prec != DONT_ADJUST_PRECISION){
//        round_string(result, max_dec_prec);
//    }
//
//    return result;
//}
static std::string mult_num_strings( std::string_view a, std::string_view b, const int max_dec_prec = DONT_ADJUST_PRECISION)
{
    
    //multiplication by zero?
    bool a_mult_by_0 = true, b_mult_by_0 = true;
    for(int n = 0; n < a.size(); n++) if(a[n] > '0') a_mult_by_0 = false;
    for(int n = 0; n < b.size(); n++) if(b[n] > '0') b_mult_by_0 = false;
    if(a_mult_by_0 || b_mult_by_0) return "0";
    
    //check for negatives
    bool negative = false;
    if(a[0]=='-' && b[0]=='-')
    {
        a = a.substr(1);
        b = b.substr(1);
        negative = false;
    }
    else if(a[0]=='-')
    {
        a = a.substr(1);
        negative = true;
    }
    else if(b[0]=='-')
    {
        b = b.substr(1);
        negative = true;
    }
    
    int a_dec = (int)a.find('.');
    int b_dec = (int)b.find('.');
    int a_has_dec = 0, b_has_dec = 0;
    if (a_dec == std::string::npos)
    {
        a_dec = (int)a.size();
    }
    else
    {
        a_has_dec = 1;
    }
    if (b_dec == std::string::npos)
    {
        b_dec = (int)b.size();
    }
    else
    {
        b_has_dec = 1;
    }
    
    //count dec place from right
    int a_aft = (int)a.size() - (a_dec + 1); if(a_aft < 0) a_aft = 0;
    int b_aft = (int)b.size() - (b_dec + 1); if(b_aft < 0) b_aft = 0;
    auto aft = a_aft + b_aft;
    //maximum size of product
    int digits_size = (int)(a.size() + b.size()) - a_has_dec - b_has_dec;
    int digits[digits_size];
    for (int i = 0; i < digits_size; i ++) digits[i] = 0;
    
    //multiply loop
    int buff, offset=0, shift_left;
    for(int x=(int)b.size() -1;x>=0;x--)
    {
        if(b[x] == '.' )
        {
            continue;
        }
        shift_left = 0;
        for(int y=(int)a.size() -1;y>=0;y--)
        {
            if(a[y] == '.')
            {
                continue;
            }
            buff = (b[x]-'0') * (a[y]-'0');
            digits[digits_size-1-offset-shift_left] += buff;
            shift_left++;
        }
        offset++;
    }
    
    //perform carry-left on digits array
    int x;
    for (int it = digits_size-1; it >=0; it--)
    {
        if(digits[it] > 9)
        {
            x = digits[it];
            digits[it] = x%10;
            if(it > 0)
            {
                digits[it-1]+=(x-digits[it])/ 10;
            }
        }
    }
    
    // create char array for return value
    int result_size = digits_size + 3; // result string needs three places for potential negative sign, leading zero, decimal
    char result[result_size + 1];  // add one for null terminal character
    for (int i = 0; i < result_size; i ++) result[i] = '0'; // initialize to zeros
    result[result_size] = '\0';    // null terminal char
    
    //add decimal to string
    int bef = result_size - ((int)aft + 1);
    if (bef >= 0 && aft > 0) result[bef] = '.';
    
    int j = digits_size - 1; // j iterates through digits array
    int i = result_size - 1; // i iterates through result string
    for(; j>= 0;)
    {
        if(result[i] != '.')
        {
            result[i] += digits[j];
            i--;
            j--;
        }
        else
        {
            i--;
        }
    }
    
    // is last character a decimal? then get rid of it
    if(result[result_size-1]=='.')
    {
        result[result_size-1] = '\0';
    }
    
    char* final_result = result;
    
    // Following code is ignored by default. It can be used to set precision level of result. Bignum library needs this function for loops that call div_num_strings() because decimal precision grows with each consecutive division due to the step where precision zeros are added to the numerator. In such cases, this trimming is needed to prevent adding decimal places which with each loop.
    if(max_dec_prec != DONT_ADJUST_PRECISION)
    {
        round_string(final_result, max_dec_prec);
    }
    
    // handle the negative sign and leading zero if any
    int n = 0;
    while(result[n] == '0' && result[n+1] != '.') n++;
    if(negative)
    {
        result[n-1] = '-';
        final_result = final_result + (n - 1);
    }
    else
    {
        final_result = final_result + n;
    }
    
    return final_result;
}

//DIVIDE
static std::string div_num_strings( const std::string & numerator, const std::string & divisor, const int max_dec_prec = DONT_ADJUST_PRECISION)
{
    
    //division by zero?!
    bool div_by_0_error = true;
    for(int n = 0; n < divisor.size(); n++)
    {
        if(divisor[n] > '0') div_by_0_error = false;
    }
    
    if(div_by_0_error) throw Bgnm_error("Cannot divide by 0.",__FILENAME__,__LINE__,__FUNCTION__,101);
    
    std::string result, subst;
    
    //check for negatives
    bool negative = false;
    if(numerator[0]=='-' && divisor[0]=='-')
    {
        negative = false;
    }
    else if(numerator[0]=='-' || divisor[0]=='-')
    {
        negative = true;
    }
    
    Num_str nu(numerator,true);
    Num_str di(divisor,true);
    
    int global_dec_loc = nu.aft_dec - di.aft_dec;
    int gl_crs; //global cursor
    int cntr; //counter
    
    //add zeros to numerator
    //then update global_dec_loc
    int zero_amnt = (di.size * 2)+5;
    nu.add_zeros(zero_amnt);
    global_dec_loc += zero_amnt;
    std::vector<int> vec(nu.size, 0);
    
    //in case divisor has many leading zeros, get rid of zeros and adjust global_dec_loc, otherwise division loop does way too many iterations in cases like 80 / .00000001
    while(di.num[0] == '0')
    {
        di.num.erase(0,1);
        di.size--;
    }
    
    //main divisor loop, loops until global cursor hits zero
    gl_crs=nu.size;
    cntr=di.size;
    
    while(comp_strs(nu.num,di.num) && (gl_crs > 0))
    {
        // 1. get smallest part of numerator that's larger than divisor
        gl_crs=nu.size - cntr + 1;//adjust cursor
        bool check = false;
        while(check==false && cntr <= nu.size)
        {
            subst = nu.num.substr(0,cntr);
            if(comp_strs(subst, di.num))
            {
                check=true; // got a chunk to work with
                // so need to take it to work with and remove it (subtract) it from the numerator while working with it
                nu.zero_out(0,cntr);
            }
            else
            {
                cntr++;
                gl_crs--;
            }
        }
        // 2. now how many divisors fit in numerator?
        while(comp_strs(subst, di.num))
        {
            subst= sub_num_strings(subst, di.num);
            vec[gl_crs-1] ++ ;
        }
        
        // 3. now take remainder and add it back to main numerator
        subst.append(gl_crs-1,'0');
        nu.num = add_num_strings(subst, nu.num);
        
        // 4. loop back to the next
    } // end main divisor loop
    
    carry(vec, true);
    vec_to_string(vec, result, true);
    
    add_dec(result,((int)result.size()-global_dec_loc ));
    
    strip_leading_0s(result);
    strip_trailing_0s(result);
    if(negative) result = "-" + result;
    
    // Following code is ignored by default. It can be used to set precision level of result. Bignum library needs this function for loops that call div_num_strings() because decimal precision grows with each consecutive division due to the step where precision zeros are added to the numerator. In such cases, this trimming is needed to prevent adding decimal places which with each loop.
    if(max_dec_prec != DONT_ADJUST_PRECISION)
    {
        round_string(result, max_dec_prec);
    }
    
    return result;
}

//function used by Bgnm constructors and overloaded operators to convert floats, doubles and long doubles to strings
template <typename T>
static std::string floatconversion(const T & f)
{
    //max reliable precision conservatively to right of decimal based on type
    int precision = 5; // base condition for float
    if (typeid(T) == typeid(double)) precision = 14; // if double
    if (typeid(T) == typeid(long double)) precision = 16; // if long double
    int base_two_exponent = 0;
    frexp(f, &base_two_exponent); // get exponent from float
    int places = floor(base_two_exponent * 0.30103); // convert exponent to base ten (approximate), round down to integer
    int rightofdecimal = precision - places; //how much of precision will be to the right of decimal
    if(rightofdecimal < 0) rightofdecimal = 0;
    std::ostringstream os;
    os << std::fixed;
    os << std::setprecision(rightofdecimal); //decimal precision anywhere from 5 for floats to 17 for double longs
    os << f;
    std::string s = os.str();
    strip_trailing_0s(s);
    return s;
}

static bool examine_text_number(const std::string & s)
{
    int has_decimal = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if((s[i] >= '0' && s[i] <= '9' ) || (s[i] == '-' && i == 0) || s[i] == '.')
        {
            if(s[i]=='-' && i > 0) return false; //found a '-' other than at beginning
            if(s[i]=='.')
            {
                if(has_decimal > 0) return false;
                has_decimal++;
            }
        }
        else
        {
            return false;
        }
    }
    return true;
}

static bool check_input(const std::string & s, const char* f)
{
    if(!examine_text_number(s)) throw Bgnm_error("Invalid string or c_string provided.",__FILENAME__,__LINE__,f,108);
    return true;
}

// multiplies string by ten by moving decimal
static void mult_str_10(std::string & s)
{
    //if there's a decimal, move it
    double i = s.find('.');
    if(i!=std::string::npos)
    {
        s.erase(i,1);
        s.insert(i+1,1,'.');
    }
    else
    { //if no decimal, add '0' to end
        s += '0';
    }
}

// divides string by ten by moving decimal
static void div_str_10(std::string & s)
{
    //if there's a decimal, move it
    double i = s.find('.');
    if(i!=std::string::npos)
    {
        s.erase(i,1);
        if(i==0)
        {
            s.insert(i,".0");
        }
        else
        {
            s.insert(i-1,1,'.');
        }
    }
    else
    { //if no decimal, add one at before last char
        double sz = s.size();
        s.insert(sz-1,1,'.');
        while(s[s.size()-1]=='0')
        { // delete trailing 0's
            s.erase(s.size()-1);
        }
        if(s[s.size()-1]=='.') s.erase(s.size()-1); //delete lone decimal
    }
}

static std::string str_mod(const std::string & number, const std::string & modulo)
{
    // nothing to do if modulo is greater than number to begin with
    if (comp_strs(modulo,number,true)) return number;
    
    std::string temp = modulo; // temp is modulo or a multiple of modulo
    std::string ret = number;
    
    // loop through, subtracting biggest chunks possible
    while(comp_strs(ret,modulo))
    {
        
        //get biggest subtractable chunk
        //multiple of modulo that is just larger than ret
        while(comp_strs(ret,temp))
        {
            mult_str_10(temp); // temp * 10
        }
        // divide temp if and while it's bigger than return (and also bigger than modulo)
        if (comp_strs(temp,ret,true) && comp_strs(temp,modulo,true))
        {
            div_str_10(temp); // temp / 10
        }
        //in case temp is less than modulo, just take modulo as new temp
        if(comp_strs(modulo,temp,true)) temp = modulo;
        
        //now subtract that biggest chunk as many times as possble
        while (comp_strs(ret,temp))
        {
            ret = sub_num_strings(ret,temp);
        }
    }
    return ret;
}

// takes a floating point number as a string and converts it to the integer, numerator, and denominator components (to be used by str_pow and str_root functions when they are given floating point exponents
// if strip_zeros = true, gets rid of extra zeros in numerator - e.g. if numerator is 0034200, numerator will become 342 and denominator will become 100000 - or essentially 342/100000
static void str_convert_float_to_fraction_components(const std::string & s, std::string & integer, std::string & numerator, std::string & denominator, bool strip_zeros = false)
{
    integer = numerator = "0";
    denominator = "1";
    int dec_loc = (int)s.find('.');
    integer = s.substr(0,dec_loc);
    if(integer == "") integer = "0";
    if(s.size() > dec_loc)
    {
        numerator = s.substr(dec_loc +1);
        if(strip_zeros) strip_trailing_0s(numerator,false,true);
        denominator.append(numerator.size(),'0');
        if(strip_zeros) strip_leading_0s(numerator);
        if(numerator == "") numerator = "0";
    }
}

// forward declare str_root();
static std::string str_root(std::string base, std::string exp, const unsigned max_dec_prec = DONT_ADJUST_PRECISION);

// power function, works with integer and floating point base and power numbers
// Warning: If power is floating point, decimal portion is converted to a fraction, and then str_root() is employed on denominator of that fraction. Therefore multiple decimal places such as .01242 can be very slow because will need to find 100000 root of base, then that root to the power of 1242 - so be careful with floating point powers. If possible, round floating point to as few decimal places as practical prior to running str_pow().
static std::string str_pow(std::string base, std::string power, const unsigned max_dec_prec = DONT_ADJUST_PRECISION)
{
    
    const unsigned internal_dec_limit = Bgnm::get_bgnm_internal_precision_limit();
    
    // negative power means final result ret needs to be 1/ret
    bool negative_power = false;
    if(power[0]=='-')
    {
        power.erase(0,1);
        negative_power = true;
    }
    
    // negative base - if power is integer and odd, then final result will be negative, otherwise ignore
    bool negative_base = false;
    if(base[0]=='-')
    {
        base.erase(0,1);
        negative_base = true;
    }
    
    // anything to power of zero equals 1
    if(comp_strs_equal(base, "0")) return "0";
    if(comp_strs_equal(power, "0")) return "1";
    
    //if power is floating point, convert decimal portion to fraction for use later
    bool floating_point_power = false;
    std::string numerator = "0", denominator = "1", integer = power, temp_base = base;
    if(power.find('.')!=std::string::npos)
    {
        floating_point_power = true;
        str_convert_float_to_fraction_components(power, integer, numerator, denominator,true);
        power = integer;
        // ingnore floating point in case of a power such as XXXX.0
        if(comp_strs_equal(numerator,"0")) floating_point_power = false;
    }
    
    std::string ret = "1", zero = "0", one = "1", two = "2";
    
    while(comp_strs(power,zero,true))
    {
        float fl;
        try{
            fl = stof(str_mod(power,two));
        }
        catch(std::exception &e)
        {
            std::cout << "error with " << fl << " e is : " << e.what() << std::endl;
        }
        if(fl == 1.0 )
        { // if power % 2 = 1
            //if(stof(str_mod(power,two)) == 1.0 ){ // if power % 2 = 1
            // ret = ret * base
            ret = mult_num_strings(ret,base,internal_dec_limit);
            // power = (power-1) / 2
            power = div_num_strings(sub_num_strings(power,one),two,internal_dec_limit);
        }
        else
        {
            // power = power / 2
            power = div_num_strings(power,two,internal_dec_limit);
        }
        base = mult_num_strings(base,base,internal_dec_limit);
    }
    
    // figure in decimal/fraction portion of exponent if exists
    // x^integer * ((x root denominator)^numerator)
    if(floating_point_power)
    {
        std::string partial;
        partial = str_pow(str_root(temp_base,denominator,internal_dec_limit), numerator,internal_dec_limit);
        ret = mult_num_strings(ret,partial,internal_dec_limit);

    }
    
    // if power was negative, need to inverse ret
    if(negative_power) ret = div_num_strings("1", ret);

    if(max_dec_prec != DONT_ADJUST_PRECISION) round_string(ret,max_dec_prec);
    
    if(negative_base)
    {
        if(str_mod(integer, "2") != "0" && numerator == "0") ret.insert(0,"-");
    }
    return ret;
    
}


//uses a Newton-Raphson type algorithm to find exp root of base
static std::string str_root(std::string base, std::string initial_exp, const unsigned max_dec_prec)
{
    
    const unsigned internal_dec_limit = Bgnm::get_bgnm_internal_precision_limit();
    const unsigned max_root_guess_count = Bgnm::get_max_root_guess_count();
    
    // exp or intitial_exp stand for exponent - but since we're talking roots, I really should have named it index - as in index of the radical
    
    std::string numerator = "0", denominator = "1", integer = initial_exp, exp = initial_exp;
    
    // negative exponent means issue error
    // There is no universally accepted definition for how to handle Negative exponents (indices) for radicals. Here we will define a negative index as -index root of x = index root of (1/x)
    try{
        if(exp[0]=='-') throw Bgnm_error("Negative index provided.",__FILENAME__,__LINE__,__FUNCTION__,102);
    } catch (std::exception & e)
    {
        std::cout << e.what() << std::endl;
        exp.erase(0,1);
        initial_exp.erase(0,1);
        base = div_num_strings("1", base);
    }
    
    // negative base - if power is integer and odd, then final result will be negative, otherwise ignore
    bool negative_base = false;
    if(base[0]=='-')
    {
        base.erase(0,1);
        negative_base = true;
    }
    
    if(comp_strs_equal(base, "0")) return "0";
    
    // need to issue exception/error in this case, cannot have a zero root
    if(comp_strs_equal(initial_exp, "0"))  throw Bgnm_error("Cannot calculate a root of base with a zero index.",__FILENAME__,__LINE__,__FUNCTION__,103);
    
    //if exp is floating point, convert decimal portion to fraction for use later
    bool floating_point_power = false;
    if(initial_exp.find('.')!=std::string::npos)
    {
        floating_point_power = true;
        str_convert_float_to_fraction_components(initial_exp, integer, numerator, denominator,true);
        exp = integer;
        // ingnore floating point in case of a power such as XXXX.0
        if(comp_strs_equal(numerator,"0")) floating_point_power = false;
    }
    
    //come up with initial guess
    //if answer base is less than one
    
    std::string trial, inst_slope, offset;
    
    // at some point need to come up with a better algorithm for guessing initial_root based on magnitude of base and exp. Time to calculate is dramatically affected by how good the initial guess is, and sometimes simply by weather guess is above or below the real answer - mostly due to the way Newton-Raphson works
    std::string initial_root = "1.1";
    long int_exp = stol(exp);
    if(int_exp > 999 && int_exp < 5000)
    {
        initial_root = "1.01";
    }
    else if(int_exp > 4999 && int_exp < 10000)
    {
        initial_root = "1.01";
    }
    else if(int_exp > 9999)
    {
        int log_10_digits = ((int)log10(int_exp))+1;
        initial_root = "1.";
        for(int i=0; i < (log_10_digits - 4);i++)
        {
            initial_root.push_back('0');
        }
        initial_root.push_back('1');
    }
    
    std::string root = initial_root;
    std::string prev_root = ".9";
    int x = 0;
    
    while(
          (!comp_strs_equal(prev_root, root, internal_dec_limit-1)) && // once guessing is good enough, these two will become equal, in which case you're done (required to be equal except for last decimal place in order to prevent unnending loops that toggle the last decimal place due to rounding
          (x < max_root_guess_count) // max_root_guess_count - sets an absolute max amount of work that the function is allowed to put in - if reaches max_root_guess_count, returns zero and should issue error
          )
    {
        //set a limit to not allow root to increase or decrease over previously attempted root by more than a factor of ten. If that happens, instead replace root with slightly adjusted root. (In testing, found that the second guess was sometimes astronomically greater, and the algorithm would take forever to come back to reality.
        if(comp_strs(root, mult_num_strings(prev_root, "10")))
        {
            root = mult_num_strings(prev_root, "1.1");
        }
        else if(comp_strs(div_num_strings(prev_root, "10"), root))
        {
            root = div_num_strings(prev_root, "1.1");
        }
        
        prev_root = root;
        trial = str_pow(root,exp,internal_dec_limit);
        //calculate slope of line (derivative) at guess. The first dirivative of x^e is ex^(e-1), which gives the slope at guess x in order to use newton(?) algorithm for adjusting the next guess
        inst_slope = mult_num_strings(exp,str_pow(root,(sub_num_strings(exp,"1")),internal_dec_limit));
        //now find the y offset of that line when the y axis value is equal to trial
        // trial = (slope * root) + offset     (e.g.  y = 2x - 5 )
        // offset = trial - (slope * root)
        offset = sub_num_strings(trial,(mult_num_strings(inst_slope, root)));
        
        //now if the slope is the new line of guestimation, set y value equal to base, and find the x or root value for that line
        // y = (slope * root) + y_offset   (y is base)
        // (base - y_offset)/slope = root
        root = div_num_strings(sub_num_strings(base,offset),inst_slope,internal_dec_limit);
        
        x++;
        //RETURN 0 AS ERROR IF MAXIMUM LOOP COUNT REACHED
        if(x >= max_root_guess_count)
        {
            std::string e = "Number of attempts to find root exceeded max_root_guess_count (" + std::to_string(max_root_guess_count) + "). Review use of str_root() or use set_max_root_guess_count(unsigned count) to increase number of allowed attempts.";
            throw Bgnm_error(e.c_str(),__FILENAME__,__LINE__,__FUNCTION__,104);
            
        }
    }
    
    // figure in decimal/fraction portion of exponent if exists
    if(floating_point_power)
    {
        //final root = base^integer * 1/base^(1/integer - 1/exp)
        std::string temp_power = sub_num_strings(div_num_strings("1", integer), div_num_strings("1", initial_exp));
        round_string(temp_power,ROOT_INDEX_MAX_PRECISION);
        std::string factor = div_num_strings("1", str_pow(base, temp_power,internal_dec_limit));
        root = mult_num_strings(root, factor);
    }
    
    if(max_dec_prec != DONT_ADJUST_PRECISION) round_string(root,max_dec_prec);
    
    if(negative_base)
    {
        if(str_mod(integer, "2") != "0" && numerator == "0") root.insert(0,"-");
    }
    
    if(floating_point_power) round_string(root,ROOT_INDEX_MAX_PRECISION);
    
    return root;
}


//  BGNM DEFINITIONS

Bgnm::Bgnm()
{
    val = "0";
}

Bgnm::Bgnm(const Bgnm & bn)
{
    val = bn.val;
}

Bgnm::Bgnm(const int i)
{
    val = std::to_string(i);
}

Bgnm::Bgnm(const long long ll)
{
    val = std::to_string(ll);
}

Bgnm::Bgnm(const float & f)
{
    val = floatconversion(f);
}

Bgnm::Bgnm(const double & d)
{
    val = floatconversion(d);
}

Bgnm::Bgnm(const long double & ld)
{
    val = floatconversion(ld);
}

Bgnm::Bgnm(const char* cs)
{
    std::string s(cs);
    check_input(s,__FUNCTION__)? val = s: val = "0";
}

Bgnm::Bgnm(const std::string & str)
{
    check_input(str,__FUNCTION__)? val = str: val = "0";
}

unsigned Bgnm::bgnm_internal_precision_limit = DEC_LIMIT;
unsigned Bgnm::bgnm_max_root_guess_count = MAX_ROOT_GUESS_COUNT;

template<> void Bgnm::operator = (const Bgnm & bn)
{
    this->val = bn.val;
}

template<> void Bgnm::operator = (const int & i)
{
    this->val = std::to_string(i);
}

template<> void Bgnm::operator = (const long long & ll)
{
    this->val = std::to_string(ll);
}

template<> void Bgnm::operator = (const float & f)
{
    this->val = floatconversion(f);
}

template<> void Bgnm::operator = (const double & d)
{
    this->val = floatconversion(d);
}

template<> void Bgnm::operator = (const long double & ld)
{
    this->val = floatconversion(ld);
}

void Bgnm::operator = (const char * cs)
{
    std::string s(cs);
    check_input(s,__FUNCTION__)? val = s: val = "0";
}

template<> void Bgnm::operator = (const std::string & s)
{
    check_input(s,__FUNCTION__)? val = s: val = "0";
}

template<> Bgnm Bgnm::operator + (const Bgnm & bn)
{
    Bgnm ret_num;
    ret_num.val = add_num_strings(this->val,bn.val);
    return ret_num;
}

template<> Bgnm Bgnm::operator + (const int & i)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const long long & ll)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const float & f)
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(f));
    return ret;
}

template<> Bgnm Bgnm::operator + (const double & d)
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(d));
    return ret;
}

template<> Bgnm Bgnm::operator + (const long double & ld)
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(ld));
    return ret;
}

Bgnm Bgnm::operator + (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const Bgnm & bn)
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,bn.val);
    return ret;
}

template<> Bgnm Bgnm::operator - (const int & i)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const long long & ll)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const float & f)
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(f));
    return ret;
}

template<> Bgnm Bgnm::operator - (const double & d)
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(d));
    return ret;
}

template<> Bgnm Bgnm::operator - (const long double & ld)
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(ld));
    return ret;
}

Bgnm Bgnm::operator - (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const Bgnm & bn)
{
    Bgnm ret;
    std::string s = bn.val;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const int & i)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const long long & ll)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const float & f)
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const double & d)
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const long double & ld)
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

Bgnm Bgnm::operator * (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const Bgnm & bn)
{
    Bgnm ret;
    std::string s = bn.val;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const int & i)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const long long & ll)
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const float & f)
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const double & d)
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const long double & ld)
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

Bgnm Bgnm::operator / (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

//for now add_num_strings can be used for increment - but really this should be a new function to perform the operation much faster
// prefix ++ overload
Bgnm& Bgnm::operator ++ ()
{
    std::string s = "1";
    this->val = add_num_strings(this->val,s);
    return *this;
}

// postfix ++ overload
Bgnm Bgnm::operator ++ (int)
{
    Bgnm ret(this->val);
    std::string s = "1";
    this->val = add_num_strings(this->val,s);
    return ret;
}

// prefix -- overload
Bgnm& Bgnm::operator -- ()
{
    std::string s = "-1";
    this->val = add_num_strings(this->val,s);
    return *this;
}

// postfix -- overload
Bgnm Bgnm::operator -- (int)
{
    Bgnm ret(this->val);
    std::string s = "-1";
    this->val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const Bgnm & bn)
{
    Bgnm ret;
    ret.val = str_mod(this->val,bn.val);
    return ret;
}

template<> Bgnm Bgnm::operator % (const int & i)
{
    Bgnm ret;
    ret.val = str_mod(this->val,std::to_string(i));
    return ret;
}

template<> Bgnm Bgnm::operator % (const long long & ll)
{
    Bgnm ret;
    ret.val = str_mod(this->val,std::to_string(ll));
    return ret;
}

template<> Bgnm Bgnm::operator % (const float & f)
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const double & d)
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const long double & ld)
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = str_mod(this->val,s);
    return ret;
}

Bgnm Bgnm::operator % (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const Bgnm & bn)
{
    Bgnm ret;
    ret.val = str_pow(this->val,bn.val);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const int & i)
{
    Bgnm ret;
    ret.val = str_pow(this->val,std::to_string(i));
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const float & f)
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = str_pow(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const double & d)
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = str_pow(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const long double & ld)
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = str_pow(this->val,s);
    return ret;
}

Bgnm Bgnm::operator ^ (const char * cs)
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_pow(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const std::string & s)
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_pow(this->val,s);
    return ret;
}

template<> bool Bgnm::operator > (const Bgnm & bn)
{
    return comp_strs(this->val,bn.val,true);
}

template<> bool Bgnm::operator > (const int & i)
{
    return comp_strs(this->val,std::to_string(i),true);
}

template<> bool Bgnm::operator > (const long long & ll)
{
    return comp_strs(this->val,std::to_string(ll),true);
}

template<> bool Bgnm::operator > (const float & f)
{
    std::string s = floatconversion(f);
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const double & d)
{
    std::string s = floatconversion(d);
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const long double & ld)
{
    std::string s = floatconversion(ld);
    return comp_strs(this->val,s,true);
}

bool Bgnm::operator > (const char * cs)
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const std::string & s)
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator < (const Bgnm & bn)
{
    return comp_strs(bn.val,this->val,true);
}

template<> bool Bgnm::operator < (const int & i)
{
    return comp_strs(std::to_string(i),this->val,true);
}

template<> bool Bgnm::operator < (const long long & ll)
{
    return comp_strs(std::to_string(ll),this->val,true);
}

template<> bool Bgnm::operator < (const float & f)
{
    std::string s = floatconversion(f);
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const double & d)
{
    std::string s = floatconversion(d);
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const long double & ld)
{
    std::string s = floatconversion(ld);
    return comp_strs(s,this->val,true);
}

bool Bgnm::operator < (const char * cs)
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const std::string & s)
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator >= (const Bgnm & bn)
{
    return comp_strs(this->val,bn.val);
}

template<> bool Bgnm::operator >= (const int & i)
{
    return comp_strs(this->val,std::to_string(i));
}

template<> bool Bgnm::operator >= (const long long & ll)
{
    return comp_strs(this->val,std::to_string(ll));
}

template<> bool Bgnm::operator >= (const float & f)
{
    std::string s = floatconversion(f);
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const double & d)
{
    std::string s = floatconversion(d);
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const long double & ld)
{
    std::string s = floatconversion(ld);
    return comp_strs(this->val,s);
}

bool Bgnm::operator >= (const char * cs)
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const std::string & s)
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator <= (const Bgnm & bn)
{
    return comp_strs(bn.val,this->val);
}

template<> bool Bgnm::operator <= (const int & i)
{
    return comp_strs(std::to_string(i),this->val);
}

template<> bool Bgnm::operator <= (const long long & ll)
{
    return comp_strs(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator <= (const float & f)
{
    std::string s = floatconversion(f);
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const double & d)
{
    std::string s = floatconversion(d);
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const long double & ld)
{
    std::string s = floatconversion(ld);
    return comp_strs(s,this->val);
}

bool Bgnm::operator <= (const char * cs)
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const std::string & s)
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator == (const Bgnm & bn)
{
    return comp_strs_equal(bn.val,this->val);
}

template<> bool Bgnm::operator == (const int & i)
{
    return comp_strs_equal(std::to_string(i),this->val);
}

template<> bool Bgnm::operator == (const long long & ll)
{
    return comp_strs_equal(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator == (const float & f)
{
    std::string s = floatconversion(f);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const double & d)
{
    std::string s = floatconversion(d);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const long double & ld)
{
    std::string s = floatconversion(ld);
    return comp_strs_equal(s,this->val);
}

bool Bgnm::operator == (const char * cs)
{
    std::string s(cs);
    check_input(s,__FUNCTION__);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const std::string & s)
{
    check_input(s,__FUNCTION__);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const Bgnm & bn)
{
    return !comp_strs_equal(bn.val,this->val);
}

template<> bool Bgnm::operator != (const int & i)
{
    return !comp_strs_equal(std::to_string(i),this->val);
}

template<> bool Bgnm::operator != (const long long & ll)
{
    return !comp_strs_equal(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator != (const float & f)
{
    std::string s = floatconversion(f);
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const double & d)
{
    std::string s = floatconversion(d);
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const long double & ld)
{
    std::string s = floatconversion(ld);
    return !comp_strs_equal(s,this->val);
}

bool Bgnm::operator != (const char * cs)
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const std::string & s)
{
    if(!check_input(s,__FUNCTION__)) return false;
    return !comp_strs_equal(s,this->val);
}

void Bgnm::operator << (const int i)
{
    for(int n = 0; n < i; n++)
    {
        mult_str_10(this->val);
    }
}

void Bgnm::operator >> (const int i)
{
    for(int n = 0; n < i; n++)
    {
        div_str_10(this->val);
    }
}

template<> void Bgnm::operator += (const Bgnm & bn)
{
    this->val = add_num_strings(this->val,bn.val);
}

template<> void Bgnm::operator += (const int & i)
{
    std::string s;
    s=std::to_string(i);
    this->val = add_num_strings(this->val,s);
}

template<> void Bgnm::operator += (const long long & ll)
{
    std::string s;
    s=std::to_string(ll);
    this->val = add_num_strings(this->val,s);
}

template<> void Bgnm::operator += (const float & f)
{
    this->val = add_num_strings(this->val,floatconversion(f));
}

template<> void Bgnm::operator += (const double & d)
{
    this->val = add_num_strings(this->val,floatconversion(d));
}

template<> void Bgnm::operator += (const long double & ld)
{
    this->val = add_num_strings(this->val,floatconversion(ld));
}

void Bgnm::operator += (const char * cs)
{
    std::string s(cs);
    if(check_input(s,__FUNCTION__)) this->val = add_num_strings(this->val,s);
}

template<> void Bgnm::operator += (const std::string & s)
{
    if(check_input(s,__FUNCTION__)) this->val = add_num_strings(this->val,s);
}

template<> void Bgnm::operator -= (const Bgnm & bn)
{
    this->val = sub_num_strings(this->val,bn.val);
}

template<> void Bgnm::operator -= (const int & i)
{
    std::string s;
    s=std::to_string(i);
    this->val = sub_num_strings(this->val,s);
}

template<> void Bgnm::operator -= (const long long & ll)
{
    std::string s;
    s=std::to_string(ll);
    this->val = sub_num_strings(this->val,s);
}

template<> void Bgnm::operator -= (const float & f)
{
    this->val = sub_num_strings(this->val,floatconversion(f));
}

template<> void Bgnm::operator -= (const double & d)
{
    this->val = sub_num_strings(this->val,floatconversion(d));
}

template<> void Bgnm::operator -= (const long double & ld)
{
    this->val = sub_num_strings(this->val,floatconversion(ld));
}

void Bgnm::operator -= (const char * cs)
{
    std::string s(cs);
    if(check_input(s,__FUNCTION__)) this->val = sub_num_strings(this->val,s);
}

template<> void Bgnm::operator -= (const std::string & s)
{
    if(check_input(s,__FUNCTION__)) this->val = sub_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const Bgnm & bn)
{
    std::string s = bn.val;
    this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const int & i)
{
    std::string s;
    s=std::to_string(i);
    this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const long long & ll)
{
    std::string s;
    s=std::to_string(ll);
    this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const float & f)
{
    std::string s = floatconversion(f);
    this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const double & d)
{
    std::string s = floatconversion(d);
    this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const long double & ld)
{
    std::string s = floatconversion(ld);
    this->val = mult_num_strings(this->val,s);
}

void Bgnm::operator *= (const char * cs)
{
    std::string s(cs);
    if(check_input(s,__FUNCTION__)) this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator *= (const std::string & s)
{
    if(check_input(s,__FUNCTION__)) this->val = mult_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const Bgnm & bn)
{
    std::string s = bn.val;
    this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const int & i)
{
    std::string s;
    s=std::to_string(i);
    this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const long long & ll)
{
    std::string s;
    s=std::to_string(ll);
    this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const float & f)
{
    std::string s = floatconversion(f);
    this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const double & d)
{
    std::string s = floatconversion(d);
    this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const long double & ld)
{
    std::string s = floatconversion(ld);
    this->val = div_num_strings(this->val,s);
}

void Bgnm::operator /= (const char * cs)
{
    std::string s(cs);
    if(check_input(s,__FUNCTION__)) this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator /= (const std::string & s)
{
    if(check_input(s,__FUNCTION__)) this->val = div_num_strings(this->val,s);
}

template<> void Bgnm::operator %= (const Bgnm & bn)
{
    this->val = str_mod(this->val,bn.val);
}

template<> void Bgnm::operator %= (const int & i)
{
    this->val = str_mod(this->val,std::to_string(i));
}

template<> void Bgnm::operator %= (const long long & ll)
{
    this->val = str_mod(this->val,std::to_string(ll));
}

template<> void Bgnm::operator %= (const float & f)
{
    std::string s = floatconversion(f);
    this->val = str_mod(this->val,s);
}

template<> void Bgnm::operator %= (const double & d)
{
    std::string s = floatconversion(d);
    this->val = str_mod(this->val,s);
}

template<> void Bgnm::operator %= (const long double & ld)
{
    std::string s = floatconversion(ld);
    this->val = str_mod(this->val,s);
}

void Bgnm::operator %= (const char * cs)
{
    std::string s(cs);
    if(check_input(s,__FUNCTION__)) this->val = str_mod(this->val,s);
}

template<> void Bgnm::operator %= (const std::string & s)
{
    if(check_input(s,__FUNCTION__)) this->val = str_mod(this->val,s);
}

Bgnm Bgnm::root(const float & index)
{
    std::string ind = std::to_string(index);
    return str_root(this->val,ind);
}

Bgnm Bgnm::sqrt()
{
    if(this->val[0] == '-') throw Bgnm_error("Cannot calculate square root of negative value.",__FILENAME__,__LINE__,__FUNCTION__,105);
    return str_root(this->val,"2");
}

Bgnm Bgnm::cbrt()
{
    return str_root(this->val,"3");
}

void Bgnm::set_bgnm_internal_precision_limit(unsigned precision)
{
    if(precision < 0 || precision > 0xffffffff) throw Bgnm_error("Provided precision was out of range.",__FILENAME__,__LINE__,__FUNCTION__,106);
    bgnm_internal_precision_limit = precision;
}

unsigned Bgnm::get_bgnm_internal_precision_limit()
{
    return bgnm_internal_precision_limit;
}

void Bgnm::set_max_root_guess_count(unsigned count)
{
    if(count < 0 || count > 0xffffffff) throw Bgnm_error("Provided count was out of range.",__FILENAME__,__LINE__,__FUNCTION__,107);
    bgnm_max_root_guess_count = count;
}

unsigned Bgnm::get_max_root_guess_count()
{
    return bgnm_max_root_guess_count;
}

int Bgnm::to_int()
{
    return std::stoi(this->val);
}

long long Bgnm::to_long_long()
{
    return std::stoll(this->val);
}

float Bgnm::to_float()
{
    return std::stof(this->val);
}

double Bgnm::to_double()
{
    return std::stod(this->val);
}

long double Bgnm::to_long_double()
{
    return std::stold(this->val);
}

std::string Bgnm::to_string()
{
    return this->val;
}

char* Bgnm::to_c_string()
{
    char * ret = new char[this->val.length() + 1];
    //const char * sp = this->val.c_str();
    std::strcpy(ret,this->val.c_str());
    return ret;
}

Bgnm Bgnm::abs()
{
    Bgnm ret = this->val;
    if(ret.val[0] == '-') ret.val.erase(0,1);
    return ret;
}

Bgnm Bgnm::floor()
{
    Bgnm ret = this->val;
    unsigned long dec_loc = ret.val.find('.');
    //if negative, then basically figure ceil() for negative number
    if(ret.val[0] == '-' && dec_loc != std::string::npos)
    {
        for(unsigned long i = (dec_loc + 1); i < ret.val.size(); i++)
        {
            // if there is even a small fraction, increment integer value and return
            if(ret.val[i] > '0')
            {
                ret.val = ret.val.erase(dec_loc,std::string::npos);
                return --ret;
            }
        }
    }
    if (dec_loc != std::string::npos)
    {
        ret.val.erase(dec_loc,std::string::npos);
    }
    return ret;
}

Bgnm Bgnm::ceil()
{
    Bgnm ret = this->val;
    unsigned long dec_loc = ret.val.find('.');
    //if negative, then basically, figure floor() for negative number
    if(ret.val[0] == '-' && dec_loc != std::string::npos)
    {
        ret.val.erase(dec_loc,std::string::npos);
    }
    if (dec_loc != std::string::npos)
    {
        for(unsigned long i = (dec_loc + 1); i < ret.val.size(); i++)
        {
            // if there is even a small fraction, increment integer value and return
            if(ret.val[i] > '0')
            {
                ret = ret.floor();
                return ++ret;
            }
        }
        ret.val.erase(dec_loc,std::string::npos);
    }
    return ret;
}

bool Bgnm::equal(const Bgnm & bn, const int precision)
{
    return comp_strs_equal(this->val, bn.val,precision);
}

Bgnm Bgnm::round(const int i)
{
    std::string ret_str = this->val;
    round_string(ret_str, i);
    Bgnm ret(ret_str);
    return ret;
}

