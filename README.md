# Big-Numbers-cpp
C++ library for gigantic numbers (integer or floating point)

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
     >> divide by multiples of ten (base 10 shift right analogous to bitwise right left in binary)

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
  
