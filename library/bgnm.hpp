/*
big_nums.hpp

Created by Ben Harvey on 10/10/20.
Copyright © 2020 Ben Harvey.

This file is part of Big Numbers C++ Library

Big Numbers C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Big Numbers C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Big Numbers C++ Library. If not, see <https://www.gnu.org/licenses/>.
 
The Big Numbers C++ Library creates a C++ class Bgnm that can represent integer and floating point numbers of any size and/or precission (to millions of places). With overloaded constructors and operators, these objects should be able to be instantiated and operated on as simply as with any primitive types. Bgnm objects can be created with any of the following types: int, long long, float, double, long double, char*, std::string, and of course Bgnm

the following operators are overloaded to facilitate seamless math operations and interaction with basic data types
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
    %= modulo assignment

    << (ostring << operator overloaded so bgnm can easily be printed via std::cout)

    void set_global_internal_precision_limit(int) // sets max precision for internal calculations, especially root and power calcs

    int get_global_internal_precision_limit() // returns max set precision for internal calculations, especially root and power calcs

    need to add ability to choose accuracy to how many decimal places (e.g., give me accuracy to 40 decimal places)

    Bgnm sqrt() // return square root
    Bgnm cbrt() // return cube root


    functions that need to be added

    Bgnm pow(precision) will offer precision other than default
    Bgnm root(precision)

    Bgnm round(precision)

    int         toint()        // convert to int
    float       tofloat()      // convert to float
    double      todouble()     // convert to double
    long double tolongdouble() // convert to long double
    char*       tocstring()    // convert to a c string
    std::string tostring()     // overloaded std::to_string() to accept Bgnm
    Bgnm        abs() returns absolute (possitive) value
 
 */


#ifndef bgnm_hpp
#define bgnm_hpp

#include <string>

// GLOBAL_INTERNAL_PRECISION_LIMIT sets the default maximum decimal precision limit for INTERNAL root and power calculations. When root and power functions call mult_num_strings() or div_num_strings(), decimal places get added which can quickly be a problem when in loops. This global limit prevents this from getting out of hand. There are times when this needs to be increased or decreased, even temporarily. In such cases, use the set/get_global_internal_precision_limit() functions. Note that this does not get applied to any other calculations (other than power and root) so as to not reduce significant digits or accuracy.
#define GLOBAL_INTERNAL_PRECISION_LIMIT 25

// MAX_ROOT_GUESS_COUNT sets default maximum number of guesses str_root() is allowed to attempt when it's looking for a root. If it hasn't happened in MAX_ROOT_GUESS_COUNT number of guesses, then it has to give up - it's probably not going to happen because initial guess wasn't close enough, or because due to the way Newton-Raphson works, the tangent at x^initial_root sent the algorithm too far off base, never to recover in any reasonable time. This value can be adjusted if necessary using the set/get_max_root_guess_count() functions. After adding the new method intitial_guess_for_root(), this counter is less relevant, and should probably never be changed.
#define MAX_ROOT_GUESS_COUNT 200

// DONT_ADJUST_PRECISION is a default constant that instructs functions to not round return values. This value should not be changed. If a different parameter/argument is provided to one of these functions instead of this default value, the function will round the return value to the number of decimal places indicated by the parameter/argument. DO NOT CHANGE!
#define DONT_ADJUST_PRECISION 0xffffffff

// When finding root, this sets maximum decimal precision for the index. In other words, if using a default value of 6, then the operation 1.123456789 root of 100 will be calculated as 1.123457 root of 100. Floating point indeces for root operations are exponentially complicated by each additional decimal place. If the floating point index is rounded to ROOT_INDEX_MAX_PRECISION, calculations are sped up significantly. This is different than global_internal_precision_limit, which prevents multiplication and division processes from adding excessive decimal places in loop situations. This constant can be ignored if not planning to run root operations with floating point indeces.
#define ROOT_INDEX_MAX_PRECISION 6

//class Bgnm_error;

//  define main Bgnm (big number) class
class Bgnm
{
private:
    
    static unsigned bgnm_internal_precision_limit;
    static unsigned bgnm_max_root_guess_count;
    static unsigned bgnm_root_index_max_precision;
    
    std::string val;

public:
    
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
    static      void set_bgnm_max_root_guess_count(unsigned count);
    static  unsigned get_bgnm_max_root_guess_count();
    static      void set_bgnm_root_index_max_precision(unsigned count);
    static  unsigned get_bgnm_root_index_max_precision();
    
    //  Overloaded operators
    //  NOTE: function templates for overloading operators accept the following parameter types:
    //        bgnm, int, long long, float, double, long double, std::string
    template<typename N>
    void operator = (const N & number);
    void operator = (const char * cs);
    
    template<typename N>
    Bgnm operator + (const N & number) const;
    Bgnm operator + (const char * cs) const;

    template<typename N>
    Bgnm operator - (const N & number) const;
    Bgnm operator - (const char * cs) const;
    
    template<typename N>
    Bgnm operator * (const N & number) const;
    Bgnm operator * (const char * cs) const;
    
    template<typename N>
    Bgnm operator / (const N & number) const;
    Bgnm operator / (const char * cs) const;
    
    Bgnm& operator ++ ();    // prefix operation
    Bgnm operator ++ (int); // postfix operation
    
    Bgnm& operator -- ();    // prefix operation
    Bgnm operator -- (int); // postfix operation
    
    template<typename N>
    Bgnm operator % (const N & number) const;
    Bgnm operator % (const char * cs) const;
    
    template<typename N>
    Bgnm operator ^ (const N & number) const;
    Bgnm operator ^ (const char * cs) const;
    
    template<typename N>
    bool operator > (const N & number) const;
    bool operator > (const char * cs) const;
    
    template<typename N>
    bool operator < (const N & number) const;
    bool operator < (const char * cs) const;
    
    template<typename N>
    bool operator >= (const N & number) const;
    bool operator >= (const char * cs) const;
    
    template<typename N>
    bool operator <= (const N & number) const;
    bool operator <= (const char * cs) const;
    
    template<typename N>
    bool operator == (const N & number) const;
    bool operator == (const char * cs) const;
    
    template<typename N>
    bool operator != (const N & number) const;
    bool operator != (const char * cs) const;
    
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
    
    int         to_int() const;
    long long   to_long_long() const;
    float       to_float() const;
    double      to_double() const;
    long double to_long_double() const;
    std::string to_string() const;
    char*       to_c_string() const;
    
    // friend functions...
    
    // equivelant to member functions above
    friend int          to_int(const Bgnm &);
    friend long long    to_long_long(const Bgnm &);
    friend float        to_float(const Bgnm &);
    friend double       to_double(const Bgnm &);
    friend long double  to_long_double(const Bgnm &);
    friend std::string  to_string(const Bgnm &);
    friend char*        to_c_string(const Bgnm &);
    
    //  overload << operator for stuff like "std::cout << Bgnm;"
    friend std::ostream &operator<<( std::ostream &s, const Bgnm & bn);

    // other essential functions
    friend Bgnm  root(const Bgnm &, const double & index); // return index root of Bgnm
    friend Bgnm  sqrt(const Bgnm &);  // return square root
    friend Bgnm  cbrt(const Bgnm &);  // return cube root
    friend Bgnm   abs(const Bgnm &);  // return absolute value
    friend Bgnm floor(const Bgnm &);  // round down to integer
    friend Bgnm  ceil(const Bgnm &);  // round up to integer
    friend bool equal(const Bgnm &, const Bgnm &, const int precision); //same as == operator except can check equality up to a given precision
    friend Bgnm round(const Bgnm &, const int i);

    
}; //end of class definition

typedef Bgnm bgnm; //allow for using either Bgnm or bgnm

#endif /* big_nums_hpp */
