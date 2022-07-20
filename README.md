<br />
<div align="center">

  <h1 align="center">Big Number C++ Library</h3>

  <p align="center">
    Work with gigantic INTEGER and FLOATING POINT numbers.
  </p>
</div>

<!-- ABOUT THE PROJECT -->
# About The Project

C++ library for gigantic numbers (integer or floating point)

  A big number class written in c++ that represents numbers of any size or precision (to millions of places). Included with the **library** is an extensive **testing module** to assist development. With overloaded constructors and operators, objects of the Bgnm class should be able to be instantiated and operated on as simply as using primitive types.

#### What about the dozens of big number libraries already out there?

Many big number libraries only deal with integers and even then they may only have a few basic math functions. This library attempts to make the Bgnm class as usable as any primitive type in your project and it includes full support for floating point numbers. Need to run the nth root of a massive floating point number where the index is itself a floating point? This library can handle it. Moreover, this library attempts to implement efficiencies where many of the other libraries don't bother. The hope is to compare this library's performance to several of the most-forked c++ big number libraries available on GitHub. A few comparisons have already been made, and so far this library is as fast or faster than the ones checked.

#### What's under the hood?

At it's core, the Bgnm object is a `std::string`. Don't worry though, each function and method have been carefully developed and re-developed to avoid additional overhead that might be expected from using strings. For example, wherever possible `std::string_view` (C++17) is used to reduce copying and memory reallocation. Where other libraries may take shortcuts (e.g. implement division by using a simple subtraction loop), this library uses more robust algorithms to make operations as efficient as possible. However, accuracy is prioritized over speed. So where using some kind of Fourier Transform for multiplication operations would greatly reduce the workload, it is not employed here due to the inherent loss of precision (dependence on pi and Euler's number). It should be noted that an earlier iteration of this library attempted to move from a string model to a binary arithmetic model. The idea was that the big number could be stored in binary in arrays of unsigned integers, and then use super fast logic operations to achieve the mathematical operations. After putting a great amount of effort into recoding the main mathematical functions to work with a binary paradigm, testing showed it was no faster, and often slower, than the string-based Bgnm operations.

# The Library

The library consists of just two files; bgnm.hpp (header file) and bgnm.cpp (cpp file). The rest of the project files are all just related to the testing module and are not needed by the library itself (see the Testing Module section below).

### Constructors

    Bgnm objects can be created with the following types
    Bgnm, int, long long, float, double, long double, char*, and std::string

### Overloaded Operators
 
Right hand values may be int, long long, float, double, long double, char*, std::string, and of course Bgnm

     =  assignment
     +  addition
     -  subtraction
     *  multiplication
     /  division
     ++ increment
     -- decrement
     ^  exponent
     %  modulo
     
     << multiply by multiples of ten (base 10 shift left  - analogous to bitwise shift left in binary)
     >> divide by multiples of ten   (base 10 shift right - analogous to bitwise right left in binary)

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
 
     << (ostring << operator overloaded so Bgnm can easily be printed via std::cout)
     
### Other Essential Math Methods

    Bgnm   root(const Bgnm &, const double & index); // return index root of Bgnm
    Bgnm   sqrt(const Bgnm &);  // return square root
    Bgnm   cbrt(const Bgnm &);  // return cube root
    Bgnm    abs(const Bgnm &);  // return absolute value
    Bgnm  floor(const Bgnm &);  // round down to integer
    Bgnm   ceil(const Bgnm &);  // round up to integer
    bool  equal(const Bgnm &, const Bgnm &, const int precision); //same as == operator except can check equality up to a given precision
    Bgnm  round(const Bgnm &, const int i); // round to i number of places
    
### Conversion to Other Types (if not out of range)

Bgnm objects can be converted back to basic types assuming the number is parsable to that type respecting size, int/float, sign, etc. This can be acheived either using the member methods, or friend methods.

    // member methods
    int         to_int() const;
    long long   to_long_long() const;
    float       to_float() const;
    double      to_double() const;
    long double to_long_double() const;
    std::string to_string() const;
    char*       to_c_string() const;
    
    // friend methods equivalent to member methods above
    friend int          to_int(const Bgnm &);
    friend long long    to_long_long(const Bgnm &);
    friend float        to_float(const Bgnm &);
    friend double       to_double(const Bgnm &);
    friend long double  to_long_double(const Bgnm &);
    friend std::string  to_string(const Bgnm &);
    friend char*        to_c_string(const Bgnm &);
  
### Static Members

`static unsigned bgnm_internal_precision_limit //(default: 25)`
    
Sets the default maximum decimal precision limit for INTERNAL root and power calculations. When root and power functions call mult_num_strings() or div_num_strings(), decimal places get added which can quickly be a problem when in loops. This global limit prevents this from getting out of hand. There are times when this needs to be increased or decreased, even temporarily. In such cases, use the set/get_global_internal_precision_limit() functions. Note that this does not get applied to any other calculations (other than power and root) so as to not reduce significant digits or accuracy.
    
`static unsigned bgnm_max_root_guess_count //(default: 200)`

Sets default maximum number of guesses str_root() is allowed to attempt when it's looking for a root. If it hasn't happened in MAX_ROOT_GUESS_COUNT number of guesses, then it has to give up. This might happen because the initial guess wasn't close enough, or because due to the way Newton-Raphson works, the tangent at x^initial_root sent the algorithm too far off base, never to recover in any reasonable time. This value can be adjusted if necessary using the set/get_max_root_guess_count() functions. After adding the new method `intitial_guess_for_root()` which uses log10 and anti logarithm to make a very close initial guess, this counter is less relevant and should probably never be changed.
    
`static unsigned bgnm_root_index_max_precision //(default: 6)`

When finding root, this sets maximum decimal precision for the index. In other words, if using a default value of 6, then the operation 1.123456789 root of 100 will be calculated as 1.123457 root of 100. Floating point indices for root operations are exponentially complicated by each additional decimal place. If the floating point index is rounded to ROOT_INDEX_MAX_PRECISION, calculations are sped up significantly. This is different than global_internal_precision_limit, which prevents multiplication and division processes from adding excessive decimal places in loop situations. This constant can be ignored if not planning to run root operations with floating point indeces.
    
The above static members have the following setters and getters
    
    static      void set_bgnm_internal_precision_limit(unsigned);
    static  unsigned get_bgnm_internal_precision_limit();
    
    static      void set_bgnm_max_root_guess_count(unsigned);
    static  unsigned get_bgnm_max_root_guess_count();
    
    static      void set_bgnm_root_index_max_precision(unsigned);
    static  unsigned get_bgnm_root_index_max_precision();
    
### Exceptions

The Big Number C++ Library has its own error class which is derived from `std::runtime_error`. Bgnm_error objects can be handled in the same way standard exceptions are handled (e.g. `bgnm_error.what()`). Here is a list of all exceptions specific to this library.

- Bgnm_error 101: Cannot divide by zero.
- Bgnm_error 102: Invalid string or c_string provided. Occurs when number represented as a std::string or c string cannot be parsed as a number.
- Bgnm_error 103: Modulo operator cannot take zero as modulus as results in undefined behavior.
- Bgnm_error 104: Cannot calculate a root of base with a zero index.
- Bgnm_error 105: Number of attempts to find root exceeded max_root_guess_count. Review use of str_root() or use set_max_root_guess_count(unsigned) to increase number of allowed attempts.
- Bgnm_error 106: Cannot calculate square root of negative value.
- Bgnm_error 107: Provided precision for internal calculations is out of range.
- Bgnm_error 108: Provided number for max root guess count is out of range.
- Bgnm_error 109: Could not convert Bgnm type to int.
- Bgnm_error 110: Could not convert Bgnm type to long long.
- Bgnm_error 111: Could not convert Bgnm type to float.
- Bgnm_error 112: Could not convert Bgnm type to double.
- Bgnm_error 113: Could not convert Bgnm type to long double.
- Bgnm_error 114: Not used.
- Bgnm_error 115: Provided number for root index max precision was out of range.
- Bgnm_error 116: Internal precision is not high enough to calculate given root. **static unsigned bgnm_internal_precision_limit** needs to be increased in order to allow root function to properly calculate this root.
- Bgnm_error 117: Internal precision is not high enough to calculate given power (exponent). **static unsigned bgnm_internal_precision_limit** needs to be increased in order to allow power function to properly calculate result.
    
<p align="right">(<a href="#top">back to top</a>)</p>

# The Testing Module

The purpose of the Testing Module is: 1) to check for bugs; 2) to check the performance of alleged improvements; 3) to compare this library's performance to that of other big number libraries.

Once you compile and run the testing module, you can type `help` to get a list of all commands in the module, or type `random` to see a quick test of a few dozen random operations using the Big Number C++ Library. For each test this module executes, it generates two random big numbers and then calculates the result twice. The first result is calculated using the Big Number library, and the second using `long double` arithmatic. It then compares the two results to check for accuracy `(abs(result_a - result_b) / result_a)`. If the difference between the results is greater than the error threshold, the module reports that there is a discrepancy. Sometimes the module will report a discrepancy even though the two results are essentially the same. This may mean that the error threshold is too small. It is often the case that the `long double` arithmetic will only be significant to about 5 or 6 decimal places and therefore not be accurate enough to compare to the Bgnm result. Hence the need for a Big Number library!

To run 1000 random tests, type ` random -n 1000 `.

To run 500 random tests, but only show results of tests that have an error or discrepancy enter ` random -e -n 500 `.

### All Testing Module Commands

The module has command line function to test every Bgnm method. Most of the commands have the following flags/options. Type ` help ` to see a full list of commands and options.

#### Common Flags/Options

`-c, --constants`
Use this option in order to specify which constants to test. Without this option, the test will always use random numbers.  For example, to test ` 12 + 3.45 ` enter ` add -c 12 3.45 `. Always provide a and b separated by space. If a and/or b is negative, enclose string in quotes (e.g. ` add -c "0.02352 -26234.734409" `).

`-e`
Only show test results that indicate errors/discrepancies (helpful when running thousands of tests).

`-n, --number`
Number of operations to perform if not the default number. For example ` add -n 10000 ` will perform 10000 random addition operations and show results.

`-t`
Shows the elapsed time to execute tests. For example ` add -n 10000 -t ` will report the time it takes to run 10000 random addition operations.

#### Math Commands

`abs`
(absolute) Finds absolute value of a.

`add`
(add) Executes a + b.

`add_assign`
(add and assign) Executes a + b and assigns result to a.

`cbrt`
(cube root) Calculates cube root of a.

`ceil`
(round up) Round a (floating point) up to nearest integer.

`dec_post`
(decrement postfix) Executes a--.

`dec_pre`
(decrement prefix) Executes --a.

`div`
(divide) Executes a / b.

`div_assign`
(divide and assign) Executes a / b and assigns result to a.

`equal`
(equal) Answers true if a == b.

`floor`
(round down) Round a (floating point) down to nearest integer.

`great`
(greater than) Answers true if a > b.

`great_eql`
(greater than or equal) Answers true if a >= b.

`inc_post`
(increment postfix) Executes a++.

`inc_pre`
(increment prefix) Executes ++a.

`less`
(less than) Answers true if a < b.

`less_eql`
(less than or equal) Answers true if a <= b.

`mod`
(modulo) Executes a % b.

`mod_assign`
(modulo and assign) Executes a % b and assigns result to a.

`mult`
(greater than) Executes a x b.

`mult_assign`
(multiply and assign) Executes a x b and assigns result to a.

`not_equal`
(not equal) Answers true if a != b.

`pow`
(power) Executes a ^ b. (NOTE: in Bgnm library, '^' operator is the power operator, not XOR operator)

`root`
(find nth root) Calculates b(th) root of a.

`shift_l`
(shift left) Base-ten operation analogous to bitwise shift left. Executes a x (10^b).

`shift_r`
(shift right) Base-ten operation analogous to bitwise shift right. Executes a / (10^b).

`sqrt`
(square root) Calculates square root of a.

`sub`
(subtract) Executes a - b.

`sub_assign`
(subtract and assign) Executes a - b and assigns result to a.

#### Other Commands

`demo`
Demo runs a demonstration, testing all the overrides of the Bgnm constructor for various data types. The results will be printed out in comparison to the original data to confirm fidelity.

`exceptions`
Test all Bgnm exceptions included in Bgnm library.

`exit`
Terminate program.

`help`
List all possible user commands.

`random`
Performs a default of 50 random operations with bgnm objects and check for errors or discrepencies. Use ` -n ` to specify different number of tests to run.

`threshhold`
Set or view error threshold. When a test is performed with Bgnm objects, the result is compared against the same operation with type double. If there is a discrepancy between the two results, the error threshold sets whether the discrepancy is reported. Remember that the discrepancy is calculated as a fraction of the difference between the results divided by the Bgnm result. Therefore in most cases the discrepancy will be something like 3.412e-16. Therefore the error threshold should be set to something in the range of 1e-15 to 1e-18. However the only rule is that it be a positive number. Enter new threshold or leave empty to view current threshold. Type "threshold 0" to reset error threshold to default value.
  
`bgnm_int_prec_limit`
Set or view static Bgnm::bgnm_internal_precision_limit that sets the default maximum decimal precision limit for INTERNAL root and power calculations. When root and power functions call mult_num_strings() or div_num_strings(), decimal places get added which can quickly be a problem when in loops. This global limit prevents this from getting out of hand. There are times when this needs to be increased or decreased, even temporarily. In such cases, use the set/get_global_internal_precision_limit() functions. Note that this does not get applied to any other calculations (other than power and root) so as to not reduce significant digits or accuracy. Enter new precision limit or leave empty to view current threshhold. Enter "0" to reset Bgnm::bgnm_internal_precision_limit to default value.

`bgnm_max_root_guess`
Set or view static Bgnm::bgnm_max_root_guess_count that sets default maximum number of guesses str_root() is allowed to attempt when it's looking for a root. If it hasn't happened in MAX_ROOT_GUESS_COUNT number of guesses, then it has to give up - it's probably not going to happen because initial guess wasn't close enough, or because due to the way Newton-Raphson works, the tangent at x^initial_root sent the algorithm too far off base, never to recover in any reasonable time. Enter new guess count limit or leave empty to view current limit. Enter "0" to reset Bgnm::bgnm_max_root_guess_count to default value.

`bgnm_root_ind_max_prec`
When finding x root of a number where x is a floating point value, this sets Bgnm::bgnm_root_index_max_precision controling the maximum decimal precision for the index. In other words, if using a default value of 6, then the operation 1.123456789 root of 100 will be calculated as 1.123457 root of 100. Floating point indices for root operations are exponentially complicated by each additional decimal place. If the floating point index is rounded to ROOT_INDEX_MAX_PRECISION, calculations are sped up significantly. This is different than global_internal_precision_limit, which prevents multiplication and division processes from adding excessive decimal places in loop situations. Enter new precision limit or leave empty to view current limit. Enter "0" to reset Bgnm::bgnm_root_index_max_precision to default value.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- LICENSE -->
# License

Big Numbers C++ Library is distributed under GNU Lesser General Public License version 3 as published by the Free Software Foundation. See `LICENSE` file for more information.

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
# Getting Started / Installation

The library consists of just two files; bgnm.hpp (header file) and bgnm.cpp (cpp file). The rest of the project files are all just related to the testing module and are not needed by the library itself. To incorporate into your project, simply include the bgnm.hpp header file in your source code and add the bgnm.cpp file to your project directory and compile away.
<p align="right">(<a href="#top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
# Usage and Examples

#### Big Number C++ Libarary

    // create Bgnm objects
    Bgnm bgnm_a(12.345);
    Bgnm bgnm_b("00.0121345");
    double c = 123456789;
    Bgnm bgnm_c = c;
    Bgnm bgnm_f("-0.0000001234");
    
    // mathematical operations
    Bgnm bgnm_d = bgnm_c + c;
    Bgnm bgnm_e = root(bgnm_d,1.125);
    bgnm_e = round(bgnm_e,5);
    bgnm_f *= 10000;
    
    // conversions
    std::string s = bgnm_e.to_string();
    long double d = bgnm_e.to_long_double();
    int i = bgnm_d.to_int();
    
    // library management
    Bgnm::set_bgnm_internal_precision_limit(50);
    unsigned ui = Bgnm::get_bgnm_root_index_max_precision();

#### Testing Module

Example output if 20 random tests are executed. The below example does not show any discrepancies.

    Enter command: random -n 20
                              -8 --X                         = -9                                                                        
                              09 <  470320145234.7           = true
                            9016 <  57077413888622303        = true
                            044. *= 54331717921376044452     = 2390595588540545955888                                                    
             -337978.22486666838 += 0359417779.6809          = 0359079801.45603333162                                                    
                  92727768335779 -= 715601401627111614       = -715508673858775835                                                       
                                 2√ 7.000000003              = 2.6457513116315372999547123                                               
            65055317450824620989 flr                         = 65055317450824620989                                                      
                     2.000000762 x  -190749                  = -381498.145350738                                                         
             1534224301743996180 <= 1534224301743996180      = true
            8.000000857989052484 %  115467                   = 8.000000857989052484                                                      
                 130610533951924 += 673349643990252          = 803960177942176                                                           
            0673887.241653041699 != .000000002               = true
              -78093098064254669 >> 6                        = -78093098064.254669                                                       
                                 3√ 64366.9                  = 40.0762918961534021741793585                                              
                      -537868413 >  -939578.71373768         = false
                                 3√ 62743184810129407.63     = 397364.3059589460068847456571491                                          
                               0 X++                         = 0                                                                         
               -8925004291997345 <= -8925004291997345        = true
                             6.0 √  874.2                    = 3.0922063453612262208996372

Example output when using `add` command
                                                                                
    Enter command: add -c 12.345 6789
                                       12.345 +  6789                  = 6801.345                                                                  
    
Example output when test reveals a discrepancy that is greater than allowed threshold. In this case, the control operation does not have high enough precision and the Bgnm operation actually produces the correct and highl-precision result.

    Enter command: root -c 7898850. 0.75610
    
    ---- NOTICE! CONTROL ANSWER DIFFERS BY MORE THAN ERROR THRESHHOLD ----
    Fractional difference of 2.91438e-08 is greater than allowed error threshhold of 1e-15
    bgnm    operation: 0.75610 √  7898850.   = 1326035164.7078233504968041059134598
    control operation: 0.7561 √  7.89885e+06 = 1.32604e+09
    
Example output when testing module catches a Bgnm exception.

    Issue with test number 760
    Bgnm ERROR 113: Could not convert Bgnm type to long double. Exception thrown in file bgnm.cpp, line 2643, function: to_long_double().

Change error threshold

    // first check current threshold
    Enter command: threshold
        Error threshold currently set to 1e-15. 
        
    // change threshold to allow larger discrepancies
    Enter command: threshold .0000001
        Error threshold successfully changed to 1e-07.
        
    // change threshold back to default (1e-15)
    Enter command: threshold 0
        Error threshold successfully changed to 1e-15. 
        
Now rerun root test from above and see that module no longer reports an error.

    Enter command: root -c 7898850. 0.75610
                   0.75610 √  7898850.            = 1326035164.7078233504968041059134598 


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
# Contributing

I would appreciate any help in further developing this library. If you have a suggestion that would make it better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- TO DO -->
# Ideas, Issues and To Do

Refer to the repository's Issues section of GitHub for tracking bugs and ideas.
https://github.com/sharpermarbles/Big-Numbers-cpp/issues

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTACT -->
# Contact

Ben - sharpermarbles@gmail.com

Project Link: [https://github.com/sharpermarbles/Big-Numbers-cpp](https://github.com/sharpermarbles/Big-Numbers-cpp)

<p align="right">(<a href="#top">back to top</a>)</p>





