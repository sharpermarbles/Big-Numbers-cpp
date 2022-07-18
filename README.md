
[![GNU LGPL License][license-shield]][license-url]
[![C++][code-shield][https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white]


<!-- PROJECT LOGO -->
<br />
<div align="center">

  <h1 align="center">Big Number C++ Library</h3>

  <p align="center">
    Work with gigantic INTEGER and FLOATING POINT numbers!
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

C++ library for gigantic numbers (integer or floating point)

  A big number class written in c++ that can represent numbers of any size or precission (up to millions of places). Included with the library is an extensive testing module to asist in development. With overloaded constructors and operators, objects of the Bgnm class should be able to be instantiated and operated on as simply as using primitive types.

## What About the Dozens of Other Big Number Libraries Out There?

First, most big number libraries only deal with integers and even then they may only have a few basic math functions. This library is attempts to make the Bgnm class as usable as any int or double primitive type in your project. Secondly, this library is fast. I'm still working to compare this library to several of the most-forked c++ big number libraries out there in GitHub, but so far, this one is as fast or faster than the ones I've checked. But really, the main reason is that I just wanted to make it, so I did. I'll be glad if someone else finds it useful or interesting. I'll be even more thrilled if anyone out there would wish to help develop it further.

## The Library

### Overloaded Operators
 
     =  assigment
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

    // other essential functions
    Bgnm   root(const Bgnm &, const double & index); // return index root of Bgnm
    Bgnm   sqrt(const Bgnm &);  // return square root
    Bgnm   cbrt(const Bgnm &);  // return cube root
    Bgnm    abs(const Bgnm &);  // return absolute value
    Bgnm  floor(const Bgnm &);  // round down to integer
    Bgnm   ceil(const Bgnm &);  // round up to integer
    bool  equal(const Bgnm &, const Bgnm &, const int precision); //same as == operator except can check equality up to a given precision
    Bgnm  round(const Bgnm &, const int i); // round to i number of places
    
### Conversion to Other Types (if not out of range)

    // Member methods
    int         to_int() const;
    long long   to_long_long() const;
    float       to_float() const;
    double      to_double() const;
    long double to_long_double() const;
    std::string to_string() const;
    char*       to_c_string() const;
    
    // friend methods equivelant to member functions above
    friend int          to_int(const Bgnm &);
    friend long long    to_long_long(const Bgnm &);
    friend float        to_float(const Bgnm &);
    friend double       to_double(const Bgnm &);
    friend long double  to_long_double(const Bgnm &);
    friend std::string  to_string(const Bgnm &);
    friend char*        to_c_string(const Bgnm &);
  
### Three static members, and what they do

<!-- LICENSE -->
## License

Big Numbers C++ Library is distributed under GNU Lesser General Public License version 3 as published by the Free Software Foundation. See `LICENSE` file for more information.

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

The library consists of just two files; bgnm.hpp (header file) and bgnm.cpp (cpp file). The rest of the project files are all just related to the testing module and are not needed by the library itself. To incorperate into your project, simply include the bgnm.hpp header file and bgnm.cpp file to your project directory and compile.


### Installation

_Below is an example of how you can instruct your audience on installing and setting up your app. This template doesn't rely on any external dependencies or services._

1. Get a free API Key at [https://example.com](https://example.com)
2. Clone the repo
   ```sh
   git clone https://github.com/your_username_/Project-Name.git
   ```
3. Install NPM packages
   ```sh
   npm install
   ```
4. Enter your API in `config.js`
   ```js
   const API_KEY = 'ENTER YOUR API';
   ```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

I would appreciate any help in further developing this library. If you have a suggestion that would make it better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>





<!-- TO DO -->
## Ideas, Issues and To Do

     need to add ability to choose accuracy to how many decimal places (e.g., give me accuracy to 40 decimal places)

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

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Ben Harvey - sharpermarbles@yahoo.com

Project Link: [https://github.com/sharpermarbles/Big-Numbers-cpp](https://github.com/sharpermarbles/Big-Numbers-cpp)

<p align="right">(<a href="#top">back to top</a>)</p>





