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

#include "bgnm.hpp"

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

static void strip_leading_0s(std::string &str)
{
    int start;
    str[0] == '-' ? start = 1 : start = 0;
    int i = start,count = 0;
    while(str[i]=='0' && i < str.size())
    {
        if(str[i+1] != '.') count++;
        i++;
    }
    if(str.size() >= count + start) str.erase(start,count);
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

// checks a string representing a number for equivelance to zero (will work on strings like "-0.0", "0", ".00000", "-0000.0" and any other possibility)
static bool equal_to_zero( const std::string_view & str)
{
    for(int i = 0; i < str.size(); i++)
    {
        if(str[i] != '-' && str[i] != '.' && str[i] > '0') return false;
    }
    return true;
}

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
    //a_shift=0; b_shift=0; i=0;
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

// This is a custom (more efficient) subtraction function used ONLY by div_num_strings. Runs about 5 times faster than sub_num_string(). It's more efficient because there's no need to deal with negatives or decimals, the size of b is gauranteed to be <= to a, and the char array[] c is gauranteed big enough. So no new memory allocation, strings, or arrays are needed and no need to compare location of decimals, add places, etc.
static void sub_num_array(std::string_view a, std::string_view b, char * c)
{
    int right_most_digit_index = (int)a.size() - 1;
    int adjust_b_index = (int)a.size()-(int)b.size();
    c[right_most_digit_index + 1] = '\0'; // get that null char in there
    int b_index, b_amount, steal = 0;
    for(int i = right_most_digit_index; i >= 0; i--)
    {
        b_index = i - adjust_b_index;
        b_index < 0 ? b_amount = '0' : b_amount = b[b_index];
        if( a[i] - steal >= b_amount )
        {
            c[i] = '0' + ( a[i] - b_amount - steal );
            steal = 0;
        }
        else
        {
            c[i] = '0' + ( a[i] - b_amount - steal ) + 10;
            steal = 1;
        }
    }
}

//DIVIDE
static std::string div_num_strings( std::string_view num_view, std::string_view div_view, const int max_dec_prec = DONT_ADJUST_PRECISION)
{
    
    //division by zero?!
    if(equal_to_zero(div_view)) throw Bgnm_error("Cannot divide by 0.",__FILENAME__,__LINE__,__FUNCTION__,101);
    
    // check for negatives
    bool negative = false;
    if(num_view[0]=='-' && div_view[0]=='-')
    {
        num_view = num_view.substr(1);
        div_view = div_view.substr(1);
        negative = false;
    }
    else if(num_view[0]=='-')
    {
        num_view = num_view.substr(1);
        negative = true;
    }
    else if(div_view[0]=='-')
    {
        div_view = div_view.substr(1);
        negative = true;
    }
    
    // check for decimals and create temporary char arrays and divisor
    int num_dec = (int)num_view.find('.');
    int div_dec = (int)div_view.find('.');

    // how many decimal places to the right?
    int num_aft_dec, div_aft_dec;
    
    int num_clean_size = (int)num_view.size();
    if (num_dec == std::string::npos)
    {
        //num_dec = (int)num_view.size();
        num_aft_dec = 0;
    }
    else
    {
        //num_has_dec = true;
        num_clean_size--; // cuz we're going to ignore the decimal for now
        num_aft_dec = (int)num_view.size() - (num_dec + 1);
        if(num_aft_dec < 0) num_aft_dec = 0;
    }
    
    int div_clean_size = (int)div_view.size();
    if (div_dec == std::string::npos)
    {
        //div_dec = (int)div_view.size();
        div_aft_dec = 0;
    }
    else
    {
        //div_has_dec = true;
        div_clean_size--;
        div_aft_dec = (int)div_view.size() - (div_dec + 1); if(div_aft_dec < 0) div_aft_dec = 0;
    }
    char div_clean[div_clean_size + 1];
    //copy div_view over to div_clean array but without decimal
    int div_counter = 0;
    for (int i = 0; i < (int)div_view.size(); i++)
    {
        if(div_view[i] != '.')
        {
            div_clean[div_counter] = div_view[i];
            div_counter++;
        }
    }
    div_clean[div_clean_size] = '\0';
        
    // add zeros to numerator
    unsigned zero_amnt = (div_clean_size * 2)+5;
    // if max precision has been specified, make sure enough zeros are added (if requested precision is less than current zero_amnt, then the final answer will be rounded at the end)
    if(max_dec_prec != DONT_ADJUST_PRECISION)
    {
        int adjusted_zero_amnt = max_dec_prec;
        adjusted_zero_amnt -= num_aft_dec;
        adjusted_zero_amnt += div_aft_dec;
        if (zero_amnt < adjusted_zero_amnt) zero_amnt = adjusted_zero_amnt;
    }
    
    // create a new char array that is the numerator, but with the appropriate number of zeros added
    const unsigned num_w_zeros_size = num_clean_size + zero_amnt;
    char num_w_zeros[num_w_zeros_size + 1]; // add one for null char
    for(int i = 0; i < num_w_zeros_size; i++) num_w_zeros[i] = '0';
    num_w_zeros[num_w_zeros_size] = '\0';
    int i = 0, j = 0;
    //copy numerator over to num_clean and num_w_zeros char arrays
    for(; i < num_view.size();)
    {
        if(num_view[i] == '.') { i++; }
        else
        {
            num_w_zeros[j] = num_view[i];
            j++;
            i++;
        }
    }
    
    // create array that will accrue the answer during the division loop (size of answer (digits) array is the same as the num_w_zeros array)
    char digits[num_w_zeros_size];
    for (int i = 0; i < num_w_zeros_size; i++) digits[i] = '0';
    
    // if divisor has leading zeros, need to get rid of them (for example, divide by .00001 - decimal should be moved already, not get rid of zeros)
    char * div_clean_ptr = &div_clean[0];
    while(*div_clean_ptr == '0')
    {
        div_clean_ptr++;
        div_clean_size--;
    }
    
    // global cursor (gl_crs) tracks the cursor location (counting from the right) along the answer array (digits[])
    // so the left-most position of the answer array (digits) is the highest index, reduced by the counter as it increases
    int gl_crs = num_w_zeros_size;
    // counter (cntr) tracks the size from left to right of the numerator substring being worked with at any given time
    int cntr = div_clean_size;
    // note that as we proceed through division loop, the counter (cntr) increases in size, and the global cursor (gl_crs) decreases
    // essentially they track the working point of the loop, cntr tracks position from the left, and gl_crs from the right
    
    
    // create char array called numerator substring (num_subst) that will hold all temporary values of numerator as it is reduced by divisor
    char num_subst[num_w_zeros_size + 1];
    
    // string view of num_w_zeros will be used to create a substring of num_w_zeros, as well as for comparison functions
    std::string_view num_w_zeros_view = num_w_zeros;
    
    //main division loop, loops until global cursor hits zero
    while(comp_strs(num_w_zeros_view,div_clean_ptr) && (gl_crs > 0))
    {
        // 1. get smallest part of numerator that's just larger than divisor
        gl_crs=num_w_zeros_size - cntr + 1;//adjust cursor
        bool check = false;
        while(check==false && cntr <= num_w_zeros_view.size()) // SHOULD THIS BE < OR <= ??
        {
            if(comp_strs(num_w_zeros_view.substr(0,cntr), div_clean_ptr))
            {
                // now numerator substring is just bigger than divisor, time to get to work
                check=true;
                //copy the view over to the num_subst array so we can subtract from it
                for(int i = 0; i < cntr; i++) num_subst[i] = num_w_zeros[i];
                num_subst[cntr] = '\0';
                // now need to remove it (subtract) it from the numerator while working with it
                // this is done by just zeroing out the correct number of digits of num_w_zeros
                for(int i=0;i<cntr;i++)
                {
                    num_w_zeros[0+i]= '0';
                }
            }
            else
            {
                cntr++;
                gl_crs--;
            }
        }
        // 2. now how many divisors fit in numerator?
        while(comp_strs(num_subst, div_clean_ptr))
        {
            sub_num_array(num_subst, div_clean_ptr, num_subst);
            digits[gl_crs-1] ++ ;
        }
        
        // 3. now take remainder and add it back to main numerator
        // "append" zeros to num_subst
        std::string_view num_subst_view = num_subst;
        int k, temp_subst_size = (int)num_subst_view.size();
        for(k = temp_subst_size; k < temp_subst_size + gl_crs - 1; k++) num_subst[k] = '0';
        num_subst[k] = '\0';
        num_subst_view = num_subst;
        
        // add remainder back to numerator
        for(int i = num_w_zeros_size - 1; i >= 0; i--)
        {
            num_w_zeros[i] += num_subst[i] - '0';
        }

        // 4. loop back to the next
    } // end main divisor loop
    
    // create char array for return value
    const int result_size = num_w_zeros_size + 3; // result string needs three places for a negative sign, potential leading zero, and a decimal
    char result[result_size + 1];  // add one for null terminal character
    for (int i = 0; i <= result_size; i ++) result[i] = 0; // initialize entire array to null
    result[0] = '0'; result[1] = '0';// first two char will be either zero or negative sign
    
    //add decimal to string
    int bef = num_w_zeros_size - (num_aft_dec + zero_amnt - div_aft_dec);
    if (bef >= 0 && bef < num_w_zeros_size) result[bef + 2] = '.';
    
    // copy digits array over to result array
    j = num_w_zeros_size - 1; // j iterates through digits array backwards
    i = 0; // i iterates through result string forwards, but leave space for negative sign if needed
    for(; j>= 0;)
    {
        if(result[i+2] != '.')
        {
            result[i+2] += digits[j];
            i++;
            j--;
        }
        else
        {
            i++;
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
        if(n == 0) n++; // test this - it may actually never be true
        result[n-1] = '-';
        final_result = final_result + (n - 1);
    }
    else
    {
        final_result = final_result + n;
    }
    
    return final_result;
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
                if(s.size() < 2) return false; // if string ".", return false
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
    if(!examine_text_number(s)) throw Bgnm_error("Invalid string or c_string provided.",__FILENAME__,__LINE__,f,102);
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
        // if decimal is moving off the right end of the string
        if(i >= s.size())
        {
            s.append("0.");
        }
        else s.insert(i+1,1,'.');
    }
    //if no decimal, add '0' to end
    else
    {
        s += '0';
    }
}

// divides string by ten by moving decimal
static void div_str_10(std::string & s)
{
    //if negative, remove and put back later
    bool negative = false;
    if(s[0]=='-')
    {
        s[0]='0';
        negative = true;
    }
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
    if(negative)
    {
        s.insert(0,1,'-');
    }
}

static std::string str_mod(const std::string & number, const std::string & modulo)
{
    // nothing to do if modulo is greater than number to begin with
    if (comp_strs(modulo,number,true)) return number;
    // if modulo is zero, throw error
    if(equal_to_zero(modulo)) throw Bgnm_error("Modulo operator cannot take zero as modulus. Results in undefined behavior.",__FILENAME__,__LINE__,"str_mod",103);
    
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
    strip_leading_0s(ret);
    return ret;
}

unsigned find_gcd(unsigned int a, unsigned int b)
{
    // using euclidean algorithm
    //std::cout << "running euclid algorithm recursively... \n";
    if (a == b) return a;
    while (a != b)
    {
        if(a > b)
        {
            if (b==0) return a;
            return find_gcd(a % b,b);
        }
        else
        {
            if (a==0) return b;
            return find_gcd(b % a,a);
        }
    }
    return 1;
}

void simplify_fraction(unsigned & numerator, unsigned & denominator)
{
    // Euclid's algorithm for computing GCD
    unsigned gcd = find_gcd(numerator,denominator);
    
    numerator = numerator/gcd;
    denominator = denominator/gcd;
}

// Takes a floating point number (non-negative!) as a string and converts it to the integer, numerator, and denominator components (to be used by str_pow and str_root functions when they are given floating point exponents/indices)
// Default is to return floating point number (2.375) as a mixed fraction (2 3/8). If "improper" is set to true, then will instead return improper fraction (19/8) - in this case, integer is set to 0.

static void str_convert_float_to_fraction_components(const double & floatpoint, unsigned & integer, unsigned & numerator, unsigned & denominator, bool improper = false)
{
    integer = floor(floatpoint);
    unsigned places = Bgnm::get_bgnm_root_index_max_precision();
    numerator = floor((floatpoint - integer) * (pow(10,places)));
    denominator = 1 * pow(10,places);
 
    // now simplify the fraction if possible
    simplify_fraction(numerator,denominator);
    
    if(improper)
    {
        numerator = (integer * denominator) + numerator;
        integer = 0;
    }
}

// forward declare str_root();
static std::string str_root(std::string base, double exp, const unsigned max_dec_prec = DONT_ADJUST_PRECISION);

/*
 power function, works with integer and floating point for both base and power (exponent)
 Warning: If power is floating point, decimal portion is converted to a fraction, and then str_root() is employed on denominator of that fraction. Therefore multiple decimal places such as .01242 can be very slow because will need to find 100000 root of base, then that root to the power of 1242 - so be careful with floating point powers. If possible, round floating point to as few decimal places as practical prior to running str_pow(). A simplifier function has been added that will attempt to simplify the fraction (if possible) using the euclidean algorithm for finding greatest common denominator. Therefor an index of .375 will be converted to 3/8, which will be calculated as pow(root(x,8),3). This is significantly faster than pow(root(x,1000),375). Just remember (as demonstrated below) that if the root function has an even index, then x necessarily cannot be negative.

 Notes regarding str_root() and str_pow() - These are basically sister functions. Consider the following: (graphing these functions is very helpful at grasping what's going on in each case)
    x^   3     equivalent to pow(x,3)
    x^  -3     equivalent to 1/(pow(x,3))
    x^  1/3    equivalent to root(x,3)
    x^ -1/3    equivalent to 1/(root(x,3))
    x^ 1/-3    same as above
    x^  4      equivalent to pow(x,4)
    x^ -4      equivalent to 1/(pow(x,4))
    x^  1/4    equivalent to root(x,4) BUT since root index of x is an even number, x cannot be a negative number
    x^ -1/4    equivalent to 1/(root(x,4)) and x cannot be negative
    x^ 1/-4    same as above
    x^ .375    equivalent to (root(x,1000))^375 or (root(x,8))^3 or pow((root(x,1000),375) or pow(root(x,8)),3)
    x^ -2.375  equivalent to  1 / ( pow(x,2) * (pow(root(x,1000)),375 ) or
                              1 / ( pow(x,2) * (pow(root(x,8),3) ) - and x cannot be negative because 8 (and 1000) is even
    2.375 √ x  (2.375th root of x) = x^(1/(2 3/8)) = x^(8/19) = (19Vx)^8 equivalent to pow(root(x,19),8)
 */
static std::string str_pow(std::string base, double power, const unsigned max_dec_prec = DONT_ADJUST_PRECISION)
{
    
    const unsigned internal_dec_limit = Bgnm::get_bgnm_internal_precision_limit();
    
    // negative power means final result ret needs to be 1/ret
    bool negative_power = false;
    if(power < 0)
    {
        power = (power * -1); // make power positive
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
    if(equal_to_zero(base)) return "0";
    if(power == 0) return "1";
    
    // if power is 1, x^1 = x, so just return base
    if(power == 1) return base;
    
    //if power is floating point, convert decimal portion to fraction for use later
    bool floating_point_power = false;
    std::string temp_base = base;
    unsigned integer = 0, numerator = 0, denominator = 1, power_as_int;
    if((power - floor(power)) > 0)
    {
        floating_point_power = true;
        str_convert_float_to_fraction_components(power, integer, numerator, denominator);
        power_as_int = integer;
//        // ingnore floating point in case of a power such as XXXX.0
//        if(numerator == 0) floating_point_power = false;
    }
    else
    {
        power_as_int = power;
    }
    
    unsigned original_power_as_int = power_as_int;
    
    std::string ret = "1";
    
    while(power_as_int > 0)
    {
        if ((power_as_int % 2) == 1) // if power % 2 = 1
        {
            // ret = ret * base
            ret = mult_num_strings(ret,base,internal_dec_limit);
            // power = (power-1) / 2
            power_as_int = (power_as_int - 1) / 2;
        }
        else
        {
            // power = power / 2
            power_as_int = power_as_int / 2;
        }
        base = mult_num_strings(base,base,internal_dec_limit);
    }
    
    // figure in decimal/fraction portion of exponent if exists
    // x^integer * ((x root denominator)^numerator)
    if(floating_point_power)
    {
        std::string partial;
        partial = str_pow(str_root(temp_base, denominator, internal_dec_limit), numerator, internal_dec_limit);
        ret = mult_num_strings(ret,partial,internal_dec_limit);
    }
    
    // if power was negative, need to inverse ret
    if(negative_power) ret = div_num_strings("1", ret);

    if(max_dec_prec != DONT_ADJUST_PRECISION) round_string(ret,max_dec_prec);
    
    if(negative_base)
    {
        if((original_power_as_int % 2) != 0 && numerator == 0) ret.insert(0,"-");
    }
    return ret;
}


// Time to calculate root is dramatically affected by how good the initial guess iswhen using Newton-Raphson algorithm. Here we convert the string to scientific notation and then use log10 and anti logarithm to estimate the root. The return value should get the first several significant digits correct, and leave the rest up to Newton-Raphson to fine tune.
std::string initial_guess_for_root(const std::string & radicand, const double & index)
{
//    std::string initial_root = "1.1";
//    long int_exp = exp;
//    if(int_exp > 999 && int_exp < 5000)
//    {
//        initial_root = "1.01";
//    }
//    else if(int_exp > 4999 && int_exp < 10000)
//    {
//        initial_root = "1.01";
//    }
//    else if(int_exp > 9999)
//    {
//        int log_10_digits = ((int)log10(int_exp))+1;
//        initial_root = "1.";
//        for(int i=0; i < (log_10_digits - 4);i++)
//        {
//            initial_root.push_back('0');
//        }
//        initial_root.push_back('1');
//    }
//    return initial_root;
    
    // index √ radicand | e.g. 4.31 √ 8714
    double root = 1,x,log_of_x,power,anti_log_of_power;
    int log,power_int;

    // convert radicand to scientific notation | radicand = x * 10^log | e.g. ( 4.31 √ 872 = 4.31 √ (8.714 * 10^3 )
    // start by creating a mantissa array (here called num) with 15 decimal places (plus integer, decimal, and null byte - so char array of 18)
    // (because even a long double type is only accurate about 15 places out at the most)
    int size,dec,first;
    char num[18];
    for(int i = 0; i < 18; i++) num[i]='0'; // initialize to zero
    num[17]='\0'; // null byte
    size = (int)radicand.size();
    dec = (int)radicand.find('.');
    first = (int)radicand.find_first_not_of("0.");
    if(dec == std::string::npos) dec = (int)radicand.size();
    if(first == std::string::npos) return 0; // str equals zero
    if(dec < first)
    {
        log = dec - first;
    }
    else
    {
        log = dec -1 -first;
    }

    int j=0,i=first;
    for(;(i<size && j<17);i++)
    {
        if(radicand[i]=='.') continue;
        else
        {
            if(j==0)
            {
                num[j]=radicand[i];
                num[j+1]='.';
                j=2;
            }
            else
            {
                num[j]=radicand[i];
                j++;
            }
        }
    }
    // convert mantissa from string to double
    x = std::stod(num);
    
    // find log10 of x | e.g. log10(8.714) = 0.940 | therefore 4.31 √ (10^0.94 * 10^3) = 4.31 √ (10^3.94)
    log_of_x = log10(x);

    // index √ (10^(log_of_x+log)) = 10^(10^(log_of_x+log))/index) | e.g. 4.31 √ (10^(.94 + 3))) = 10^(3.94/4.31) = 10^.91415 * 10^0
    power = (log_of_x + log)/index;
    
    // split power into integer portion (power_int) and decimal portion (power)
    power_int = floor(power);
    power = power - power_int;
    
    // find anti logarithm of power | e.g. base 10 anti log of 0.914 = 8.204
    anti_log_of_power = pow(10,power);
    
    // root = anti_log_of_power * 10^power_int | e.g. root = 8.204 * 10^0 = 8.204
    root = anti_log_of_power * pow(10,power_int);
    
    //return as string
    return std::to_string(root);
}

//uses a Newton-Raphson type algorithm to find exp root of base
static std::string str_root(std::string base, double initial_exp, const unsigned max_dec_prec)
{
    
    const unsigned internal_dec_limit   = Bgnm::get_bgnm_internal_precision_limit();
    const unsigned max_root_guess_count = Bgnm::get_bgnm_max_root_guess_count();
    const unsigned index_precision  = Bgnm::get_bgnm_root_index_max_precision();
    
    // exp or intitial_exp stands for exponent - but since we're talking roots, I really should have named it index - as in index of the radical
    
    //std::string numerator = "0", denominator = "1";
    double exp = initial_exp;
    
    // There is no universally accepted definition for how to handle a negative index root of a radical (-n √ x).
    // Here we will define a negative index as:  -n √ x = n √ (1/x)
    if(exp < 0)
    {
        exp = (exp * -1); // make power positive
        initial_exp = (initial_exp * -1);
        base = div_num_strings("1", base);
    }
    
    // negative base - if power is integer and odd, then final result will be negative, otherwise ignore
    bool negative_base = false;
    if(base[0]=='-')
    {
        base.erase(0,1);
        negative_base = true;
    }
    
    if(equal_to_zero(base)) return "0";
    
    std::string root = "1";
    
    // need to issue exception/error in this case, cannot have a zero root
    if(initial_exp == 0)  throw Bgnm_error("Cannot calculate a root of base with a zero index.",__FILENAME__,__LINE__,__FUNCTION__,104);
    
    //if index is floating point...
    unsigned integer = 0, numerator = 0, denominator = 1;
    if((initial_exp - floor(initial_exp)) > 0) // if initial exp is not an integer
    {
        // convert floating point index to improper fraction
        str_convert_float_to_fraction_components(initial_exp, integer, numerator, denominator, true);

        // e.g. 2.375 root of x = 2 3/8 root of x = 19/8 root of x = (19 root of x) ^ 8
        // root = pow(root(x,numerator),denominator)
        root = str_pow(str_root(base,numerator),denominator);
    }
    // otherwise index is not floating point...
    else
    {
        std::string trial, inst_slope, offset;
        
        // Get an apropriate initial guess for root to plug into Newton-Raphson.
        root = initial_guess_for_root(base, exp);
        
        // after adjusting the initial_guess_for_root() function algorithm, probably need to adjust this initial prev_root variable too.
        std::string prev_root = "0";
        int x = 0;
        
        while(
              (!comp_strs_equal(prev_root, root, internal_dec_limit-1)) && // once guessing is good enough, these two will become equal, in which case you're done (required to be equal except for last decimal place in order to prevent unnending loops that toggle the last decimal place due to rounding
              (x < max_root_guess_count) // max_root_guess_count - sets an absolute max amount of work that the function is allowed to put in - if reaches max_root_guess_count, returns zero and should throw exception
              )
        {
            // FOR NOW DELETING THIS FOLLOWING GUARDRAIL ROUTINE BECAUSE I TRUST THE NEW initial_guess_for_root() FUNCTION ENOUGH THAT I DON'T THINK I NEED THIS ANYMORE
            /*
            //set a limit to not allow root to increase or decrease over previously attempted root by more than a factor of ten. If that happens, instead replace root with slightly adjusted root. (In testing, found that the second guess was sometimes astronomically greater, and the algorithm would take forever to come back to reality.
            if(comp_strs(root, mult_num_strings(prev_root, "10")))
            {
                root = mult_num_strings(prev_root, "1.1");
            }
            else if(comp_strs(div_num_strings(prev_root, "10"), root))
            {
                root = div_num_strings(prev_root, "1.1");
            }
             */
            
            prev_root = root;
            trial = str_pow(root,exp,internal_dec_limit);
            //calculate slope of line (derivative) at guess. The first dirivative of x^e is ex^(e-1), which gives the slope at guess x in order to use Newton-Raphson algorithm for adjusting the next guess
            inst_slope = mult_num_strings(std::to_string(exp),str_pow(root,exp-1,internal_dec_limit));
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
                throw Bgnm_error(e.c_str(),__FILENAME__,__LINE__,__FUNCTION__,105);
                
            }
        }
    }
    
    if(max_dec_prec != DONT_ADJUST_PRECISION) round_string(root,max_dec_prec);
    
    if(negative_base)
    {
        if(str_mod(std::to_string(integer), "2") != "0" && numerator == 0) root.insert(0,"-");
    }
    
    // DELETED THE FOLLOWING ROUNDING ACTION BECAUSE I DON'T THINK THE ROOT_INDEX_MAX_PRECISION_CONCEPT IS NECESSARY AT ALL ANYMORE - NEED TO LOOK AT IT AGAIN
    /*
    // Not sure this should be left in here. The idea here is that if the index had a certain number of significant digits, then the final returned root should not pretend to have more precision
    if(floating_point_power) round_string(root,Bgnm::get_bgnm_root_index_max_precision());
    */
    
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

unsigned Bgnm::bgnm_internal_precision_limit = GLOBAL_INTERNAL_PRECISION_LIMIT;
unsigned Bgnm::bgnm_max_root_guess_count     = MAX_ROOT_GUESS_COUNT;
unsigned Bgnm::bgnm_root_index_max_precision = ROOT_INDEX_MAX_PRECISION;

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

template<> Bgnm Bgnm::operator + (const Bgnm & bn) const
{
    Bgnm ret_num;
    ret_num.val = add_num_strings(this->val,bn.val);
    return ret_num;
}

template<> Bgnm Bgnm::operator + (const int & i) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const long long & ll) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const float & f) const
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(f));
    return ret;
}

template<> Bgnm Bgnm::operator + (const double & d) const
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(d));
    return ret;
}

template<> Bgnm Bgnm::operator + (const long double & ld) const
{
    Bgnm ret;
    ret.val = add_num_strings(this->val,floatconversion(ld));
    return ret;
}

Bgnm Bgnm::operator + (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator + (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = add_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const Bgnm & bn) const
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,bn.val);
    return ret;
}

template<> Bgnm Bgnm::operator - (const int & i) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const long long & ll) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const float & f) const
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(f));
    return ret;
}

template<> Bgnm Bgnm::operator - (const double & d) const
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(d));
    return ret;
}

template<> Bgnm Bgnm::operator - (const long double & ld) const
{
    Bgnm ret;
    ret.val = sub_num_strings(this->val,floatconversion(ld));
    return ret;
}

Bgnm Bgnm::operator - (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator - (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = sub_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const Bgnm & bn) const
{
    Bgnm ret;
    std::string s = bn.val;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const int & i) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const long long & ll) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const float & f) const
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const double & d) const
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const long double & ld) const
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

Bgnm Bgnm::operator * (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator * (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = mult_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const Bgnm & bn) const
{
    Bgnm ret;
    std::string s = bn.val;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const int & i) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(i);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const long long & ll) const
{
    Bgnm ret;
    std::string s;
    s=std::to_string(ll);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const float & f) const
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const double & d) const
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const long double & ld) const
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = div_num_strings(this->val,s);
    return ret;
}

Bgnm Bgnm::operator / (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator / (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = div_num_strings(this->val,s);
    return ret;
}

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

template<> Bgnm Bgnm::operator % (const Bgnm & bn) const
{
    Bgnm ret;
    ret.val = str_mod(this->val,bn.val);
    return ret;
}

template<> Bgnm Bgnm::operator % (const int & i) const
{
    Bgnm ret;
    ret.val = str_mod(this->val,std::to_string(i));
    return ret;
}

template<> Bgnm Bgnm::operator % (const long long & ll) const
{
    Bgnm ret;
    ret.val = str_mod(this->val,std::to_string(ll));
    return ret;
}

template<> Bgnm Bgnm::operator % (const float & f) const
{
    Bgnm ret;
    std::string s = floatconversion(f);
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const double & d) const
{
    Bgnm ret;
    std::string s = floatconversion(d);
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const long double & ld) const
{
    Bgnm ret;
    std::string s = floatconversion(ld);
    ret.val = str_mod(this->val,s);
    return ret;
}

Bgnm Bgnm::operator % (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator % (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_mod(this->val,s);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const Bgnm & bn) const
{
    Bgnm ret;
    ret.val = str_pow(this->val,bn.to_double());
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const int & i) const
{
    Bgnm ret;
    ret.val = str_pow(this->val,i);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const float & f) const
{
    Bgnm ret;
    ret.val = str_pow(this->val,f);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const double & d) const
{
    Bgnm ret;
    ret.val = str_pow(this->val,d);
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const long double & ld) const
{
    Bgnm ret;
    ret.val = str_pow(this->val,ld);
    return ret;
}

Bgnm Bgnm::operator ^ (const char * cs) const
{
    Bgnm ret;
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_pow(this->val,std::stod(cs));
    return ret;
}

template<> Bgnm Bgnm::operator ^ (const std::string & s) const
{
    Bgnm ret;
    if(!check_input(s,__FUNCTION__)) return ret;
    ret.val = str_pow(this->val,std::stod(s));
    return ret;
}

template<> bool Bgnm::operator > (const Bgnm & bn) const
{
    return comp_strs(this->val,bn.val,true);
}

template<> bool Bgnm::operator > (const int & i) const
{
    return comp_strs(this->val,std::to_string(i),true);
}

template<> bool Bgnm::operator > (const long long & ll) const
{
    return comp_strs(this->val,std::to_string(ll),true);
}

template<> bool Bgnm::operator > (const float & f) const
{
    std::string s = floatconversion(f);
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const double & d) const
{
    std::string s = floatconversion(d);
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return comp_strs(this->val,s,true);
}

bool Bgnm::operator > (const char * cs) const
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator > (const std::string & s) const
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s,true);
}

template<> bool Bgnm::operator < (const Bgnm & bn) const
{
    return comp_strs(bn.val,this->val,true);
}

template<> bool Bgnm::operator < (const int & i) const
{
    return comp_strs(std::to_string(i),this->val,true);
}

template<> bool Bgnm::operator < (const long long & ll) const
{
    return comp_strs(std::to_string(ll),this->val,true);
}

template<> bool Bgnm::operator < (const float & f) const
{
    std::string s = floatconversion(f);
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const double & d) const
{
    std::string s = floatconversion(d);
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return comp_strs(s,this->val,true);
}

bool Bgnm::operator < (const char * cs) const
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator < (const std::string & s) const
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val,true);
}

template<> bool Bgnm::operator >= (const Bgnm & bn) const
{
    return comp_strs(this->val,bn.val);
}

template<> bool Bgnm::operator >= (const int & i) const
{
    return comp_strs(this->val,std::to_string(i));
}

template<> bool Bgnm::operator >= (const long long & ll) const
{
    return comp_strs(this->val,std::to_string(ll));
}

template<> bool Bgnm::operator >= (const float & f) const
{
    std::string s = floatconversion(f);
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const double & d) const
{
    std::string s = floatconversion(d);
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return comp_strs(this->val,s);
}

bool Bgnm::operator >= (const char * cs) const
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator >= (const std::string & s) const
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(this->val,s);
}

template<> bool Bgnm::operator <= (const Bgnm & bn) const
{
    return comp_strs(bn.val,this->val);
}

template<> bool Bgnm::operator <= (const int & i) const
{
    return comp_strs(std::to_string(i),this->val);
}

template<> bool Bgnm::operator <= (const long long & ll) const
{
    return comp_strs(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator <= (const float & f) const
{
    std::string s = floatconversion(f);
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const double & d) const
{
    std::string s = floatconversion(d);
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return comp_strs(s,this->val);
}

bool Bgnm::operator <= (const char * cs) const
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator <= (const std::string & s) const
{
    if(!check_input(s,__FUNCTION__)) return false;
    return comp_strs(s,this->val);
}

template<> bool Bgnm::operator == (const Bgnm & bn) const
{
    return comp_strs_equal(bn.val,this->val);
}

template<> bool Bgnm::operator == (const int & i) const
{
    return comp_strs_equal(std::to_string(i),this->val);
}

template<> bool Bgnm::operator == (const long long & ll) const
{
    return comp_strs_equal(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator == (const float & f) const
{
    std::string s = floatconversion(f);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const double & d) const
{
    std::string s = floatconversion(d);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return comp_strs_equal(s,this->val);
}

bool Bgnm::operator == (const char * cs) const
{
    std::string s(cs);
    check_input(s,__FUNCTION__);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator == (const std::string & s) const
{
    check_input(s,__FUNCTION__);
    return comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const Bgnm & bn) const
{
    return !comp_strs_equal(bn.val,this->val);
}

template<> bool Bgnm::operator != (const int & i) const
{
    return !comp_strs_equal(std::to_string(i),this->val);
}

template<> bool Bgnm::operator != (const long long & ll) const
{
    return !comp_strs_equal(std::to_string(ll),this->val);
}

template<> bool Bgnm::operator != (const float & f) const
{
    std::string s = floatconversion(f);
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const double & d) const
{
    std::string s = floatconversion(d);
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const long double & ld) const
{
    std::string s = floatconversion(ld);
    return !comp_strs_equal(s,this->val);
}

bool Bgnm::operator != (const char * cs) const
{
    std::string s(cs);
    if(!check_input(s,__FUNCTION__)) return false;
    return !comp_strs_equal(s,this->val);
}

template<> bool Bgnm::operator != (const std::string & s) const
{
    if(!check_input(s,__FUNCTION__)) return false;
    return !comp_strs_equal(s,this->val);
}

void Bgnm::operator << (const int i)
{
    if(equal_to_zero(this->val)) return;
    for(int n = 0; n < i; n++)
    {
        mult_str_10(this->val);
    }
    strip_leading_0s(this->val);
    // delete trailing decimal if exists
    if(this->val[this->val.size()-1] == '.') this->val.erase(this->val.size()-1,1);
}

void Bgnm::operator >> (const int i)
{
    if(equal_to_zero(this->val)) return;
    for(int n = 0; n < i; n++)
    {
        div_str_10(this->val);
    }
    strip_leading_0s(this->val);
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

Bgnm Bgnm::root(const double & index) const
{
    return str_root(this->val,index);
}

Bgnm Bgnm::sqrt() const
{
    if(this->val[0] == '-') throw Bgnm_error("Cannot calculate square root of negative value.",__FILENAME__,__LINE__,__FUNCTION__,106);
    return str_root(this->val,2);
}

Bgnm Bgnm::cbrt() const
{
    return str_root(this->val,3);
}

void Bgnm::set_bgnm_internal_precision_limit(unsigned precision)
{
    // set back to default if zero passed as argument
    if(precision == 0) bgnm_internal_precision_limit = GLOBAL_INTERNAL_PRECISION_LIMIT;
    else if(precision < 2 || precision > 0xffffffff)
    {
        throw Bgnm_error("Provided precision for internal calculations was out of range.",__FILENAME__,__LINE__,__FUNCTION__,107);
    }
    else bgnm_internal_precision_limit = precision;
}

unsigned Bgnm::get_bgnm_internal_precision_limit()
{
    return bgnm_internal_precision_limit;
}

void Bgnm::set_bgnm_max_root_guess_count(unsigned count)
{
    // set back to default if zero passed as argument
    if(count == 0) bgnm_max_root_guess_count = MAX_ROOT_GUESS_COUNT;
    else if(count < 2 || count > 0xffffffff)
    {
        throw Bgnm_error("Provided number for max root guess count was out of range.",__FILENAME__,__LINE__,__FUNCTION__,108);
    }
    else bgnm_max_root_guess_count = count;
}

unsigned Bgnm::get_bgnm_max_root_guess_count()
{
    return bgnm_max_root_guess_count;
}

void Bgnm::set_bgnm_root_index_max_precision(unsigned number)
{
    // set back to default if zero passed as argument
    if(number == 0) bgnm_root_index_max_precision = ROOT_INDEX_MAX_PRECISION;
    else if(number < 0 || number > 200)
    {
        throw Bgnm_error("Provided number for root index max precision was out of range (must be between 1 and 200. Enter '0' to reset to default).",__FILENAME__,__LINE__,__FUNCTION__,115);
    }
    else bgnm_root_index_max_precision = number;
}

unsigned Bgnm::get_bgnm_root_index_max_precision()
{
    return bgnm_root_index_max_precision;
}



int Bgnm::to_int() const
{
    try { return std::stoi(this->val);}
    catch (std::exception e)
    {
        throw Bgnm_error("Could not convert Bgnm type to int.",__FILENAME__,__LINE__,__FUNCTION__,109);
    }
}

long long Bgnm::to_long_long() const
{
    try { return std::stoll(this->val);}
    catch (std::exception e)
    {
        throw Bgnm_error("Could not convert Bgnm type to long long.",__FILENAME__,__LINE__,__FUNCTION__,110);
    }
}

float Bgnm::to_float() const
{
    try { return std::stof(this->val);}
    catch (std::exception e)
    {
        throw Bgnm_error("Could not convert Bgnm type to float.",__FILENAME__,__LINE__,__FUNCTION__,111);
    }
}

double Bgnm::to_double() const
{
    try { return std::stod(this->val);}
    catch (std::exception e)
    {
        
        // GET RID OF THIS NEXT LINE - ONLY FOR TESTING PURPOSES
        std::cout << "THIS BGNM CANNOT CONVERT TO DOUBLE:  " << this->val << std::endl;
        
        throw Bgnm_error("Could not convert Bgnm type to double.",__FILENAME__,__LINE__,__FUNCTION__,112);
    }
}

long double Bgnm::to_long_double() const
{
    //return std::stold(this->val);
    try { return std::stold(this->val);}
    catch (std::exception e)
    {
        // GET RID OF THIS NEXT LINE - ONLY FOR TESTING PURPOSES
        std::cout << "THIS BGNM CANNOT CONVERT TO LONG DOUBLE:  " << this->val << std::endl;
        
        throw Bgnm_error("Could not convert Bgnm type to long double.",__FILENAME__,__LINE__,__FUNCTION__,113);
    }
}

std::string Bgnm::to_string() const
{
    return this->val;
}

char* Bgnm::to_c_string() const
{
    char * ret = new char[this->val.length() + 1];
    std::strcpy(ret,this->val.c_str());
    return ret;
}

Bgnm Bgnm::abs() const
{
    Bgnm ret = this->val;
    if(ret.val[0] == '-') ret.val.erase(0,1);
    return ret;
}

Bgnm Bgnm::floor() const
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
    if(ret.val == "" || ret.val == ".") ret.val = "0";
    return ret;
}

Bgnm Bgnm::ceil() const
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
    if(ret.val == "" || ret.val == ".") ret.val = "0";
    return ret;
}

bool Bgnm::equal(const Bgnm & bn, const int precision) const
{
    return comp_strs_equal(this->val, bn.val,precision);
}

Bgnm Bgnm::round(const int i) const
{
    std::string ret_str = this->val;
    round_string(ret_str, i);
    Bgnm ret(ret_str);
    return ret;
}

