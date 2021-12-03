/*
  big_nums.hpp
  big_nums

  Created by Ben Harvey on 10/10/20.
  Copyright © 2020 Ben Harvey. All rights reserved.

  A big number class that represents numbers of any size and/or precission. With overloaded constructors and operators, these objects should be able to be instantiated and operated on/with as simply as with any primitive types. Bgnm objects can be created with any of the following types: int, long long, float, double, long double, char*, std::string, and of course Bgnm

 the following operators are overloaded to facilitate seamless math operations and interaction with other data types
     =  assigment
     +  addition
     -  subtraction
     *  multiplication
     /  division
     ++ increment
     -- decrement
     ^  exponent
     %  modulo

     << multiply by multiples of ten (base 10 shift left analogous to bitwise shift left in binary)
     >> divide by multiples of ten (base 10 shift right analogous to bitwise shift right in binary)

     <  less than
     >  greater than
     <= less than or equal to
     >= greater than or equal to
     == equality
     != inequality
 
     += addition assignment
     -= subtraction assignment
     *= multiplication assignment
     /= division assignment
     %= modulus assignment
 
     << (ostring << operator overloaded so bgnm can easily be printed via std::cout)
 
     void set_global_internal_precision_limit(int) // sets max precision for internal calculations, especially root and power calcs
 
     int get_global_internal_precision_limit() // returns max set precision for internal calculations, especially root and power calcs
 
 need to add ability to choose accuracy to how many decimal places (e.g., give me accuracy to 40 decimal places)
 
     Bgnm sqrt() // return square root
     Bgnm cbrt() // return cube root
 
 
 functions that need to be added

     Bgnm pow(precision) will offer precision other than
     Bgnm root(precision)
 
     Bgnm round(precision)
 
     int         toint() // convert to int
     float       tofloat() // convert to float
     double      todouble() // convert to double
     long double tolongdouble() // convert to long double
     char*       tocstring()
     std::string tostring() // overloaded std::to_string() to accept Bgnm
     Bgnm        abs() returns absolute (possitive) value
 
     bool Bgnm::equal(precision) same as == operator, but can set decimal precision, after which the method ignores differences and returns true
 
     Bgnm floor() // rounds down to nearest integer value
     Bgnm ceil()  // rounds up to nearest integer value
 
     ADD support for hexadecimal input?
     Add conversion of Bgnm to string hexa
 
     Add somthing like sizeof() or length() to get number of digits (with or without decimal)
 
     Add option that keeps Bgnm library from checking cstrings and stdstrings for validity - this would improve performance if parent program checks/trusts all input
 
     fix exception handling so properly passes exceptions back to parent program - or should I just make the thing crash if error exceptions are thrown??
  
    fix issue with modulo operator - what to do with negative modulus (or remainder) - need to figure out whether this should be allowed, make modulo operator have exactly the same behavior as c++ % operator and fmod() function
 
    fix issues with power operator - issues with large powers (or negative), find ways to speed it up
 
    << and >> could be more efficient - right now they loop through mult_str_10 and div_str_10 but this isn't efficient
            one possibility would be to change mult_str_10 and div_str_10 so that they take an int for how many operations to perform, then they can be way more efficient instead of str.erase()/str.append()/str.insert() multiple times in a row
 
 */



#ifndef bgnm_hpp
#define bgnm_hpp

#include <string>

// DEC_LIMIT sets the default maximum decimal precision limit for INTERNAL root and power calculations. When root and power functions call mult_num_strings() or div_num_strings(), decimal places get added which can quickly be a problem when in loops. This global limit prevents this from getting out of hand. There are times when this needs to be increased or decreased, even temporarily. In such cases, use the set/get_global_internal_precision_limit() functions.
#define DEC_LIMIT 25

// MAX_ROOT_GUESS_COUNT sets default maximum number of guesses str_root() is allowed to attempt when it's looking for a root. If it hasn't happened in MAX_ROOT_GUESS_COUNT number of guesses, then it has to give up - it's probably not going to happen because initial guess wasn't close enough, or because due to the way Newton-Raphson works, the tangent at x^initial_root sent the algorithm too far off base, never to recover in any reasonable time. This value can be adjusted if necessary using the set/get_max_root_guess_count() functions.
#define MAX_ROOT_GUESS_COUNT 200

// DONT_ADJUST_PRECISION is a default constant that instructs functions to not round return values. This value should not be changed. If a different parameter/argument is provided to one of these functions instead of this default value, the function will round the return value to the number of decimal places indicated by the parameter/argument.
#define DONT_ADJUST_PRECISION 0xffffffff

// When finding root, this sets maximum decimal precision for index. In other words, if using a default value of 6, 1.123456789 root of 100 will be calculated as 1.123457 root of 100. This is different than global_internal_precision_limit, which prevents multiplication and division processes from adding excessive decimal places in loop situations.
#define ROOT_INDEX_MAX_PRECISION 6

//  define main Bgnm (big number) class
class Bgnm
{
    
    static unsigned bgnm_internal_precision_limit;
    static unsigned bgnm_max_root_guess_count;
    
    std::string val;

    public:
    
    //  overload << operator for ostream so you can do stuff like "std::cout << Bgnm << std::endl"
    friend std::ostream &operator<<( std::ostream &s, const Bgnm & bn)
    {
       s << bn.val;
       return s;
    }
    
    // overload std::to_string() for easy conversion from Bgnm to std::string
    friend std::string to_string( const Bgnm & bn)
    {
        return bn.val;
    }
    
    //  Constructors
    Bgnm();
    Bgnm(const Bgnm & bn);
    Bgnm(const int i);
    Bgnm(const long long ll);
    Bgnm(const float & f);
    Bgnm(const double & d);
    Bgnm(const long double & ld);
    Bgnm(const char* str_ptr);
    Bgnm(const std::string & str);

    //  Destructor
    ~Bgnm(){};
    
    static      void set_bgnm_internal_precision_limit(unsigned precision);
    static  unsigned get_bgnm_internal_precision_limit();
    static      void set_max_root_guess_count(unsigned count);
    static  unsigned get_max_root_guess_count();
    
    //  Overloaded operators
    //  NOTE: function templates for overloading operators accept the following parameter types:
    //        bgnm, int, long long, float, double, long double, std::string
    template<typename N>
    void operator = (const N & number);
    void operator = (const char * cs);
    
    template<typename N>
    Bgnm operator + (const N & number);
    Bgnm operator + (const char * cs);

    template<typename N>
    Bgnm operator - (const N & number);
    Bgnm operator - (const char * cs);
    
    template<typename N>
    Bgnm operator * (const N & number);
    Bgnm operator * (const char * cs);
    
    template<typename N>
    Bgnm operator / (const N & number);
    Bgnm operator / (const char * cs);
    
    Bgnm& operator ++ ();    // prefix operation
    Bgnm operator ++ (int); // postfix operation
    
    Bgnm& operator -- ();    // prefix operation
    Bgnm operator -- (int); // postfix operation
    
    template<typename N>
    Bgnm operator % (const N & number);
    Bgnm operator % (const char * cs);
    
    template<typename N>
    Bgnm operator ^ (const N & number);
    Bgnm operator ^ (const char * cs);
    
    template<typename N>
    bool operator > (const N & number);
    bool operator > (const char * cs);
    
    template<typename N>
    bool operator < (const N & number);
    bool operator < (const char * cs);
    
    template<typename N>
    bool operator >= (const N & number);
    bool operator >= (const char * cs);
    
    template<typename N>
    bool operator <= (const N & number);
    bool operator <= (const char * cs);
    
    template<typename N>
    bool operator == (const N & number);
    bool operator == (const char * cs);
    
    template<typename N>
    bool operator != (const N & number);
    bool operator != (const char * cs);
    
    void operator << (const int i);
    void operator >> (const int i);
    
    template<typename N>
    void operator += (const N & number);
    void operator += (const char * cs);
    
    template<typename N>
    void operator -= (const N & number);
    void operator -= (const char * cs);

    template<typename N>
    void operator *= (const N & number);
    void operator *= (const char * cs);
    
    template<typename N>
    void operator /= (const N & number);
    void operator /= (const char * cs);
    
    template<typename N>
    void operator %= (const N & number);
    void operator %= (const char * cs);
    
    Bgnm        root(const float & index);
    Bgnm        sqrt();     // return square root
    Bgnm        cbrt();     // return cube root
    Bgnm        abs();      // return absolute value
    Bgnm        floor();    // round down to integer
    Bgnm        ceil();     // round up to integer
    bool        equal(const Bgnm & bn, const int precision); //same as == operator except will check equality
    Bgnm        round(const int i);
    
    int         to_int();
    long long   to_long_long();
    float       to_float();
    double      to_double();
    long double to_long_double();
    std::string to_string();
    char*       to_c_string();
    
}; //end of class definition

typedef Bgnm bgnm; //allow for using either Bgnm or bgnm

#endif /* big_nums_hpp */
