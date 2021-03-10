
//  main.cpp
//
//  Created by Ben Harvey on 1/6/20.
//  Copyright © 2020 Ben Harvey. All rights reserved.

//  main() function for testing and playing with bgnm library

#include <iostream>
#include <string>


//#include <cfloat> // for LDBL_EPSILON DBL_EPSILON FLT_EPSILON only
//#include <math.h> // for frexp() only

//#include <sstream>
//#include <iomanip> // for std::setprecision()


//big_nums library
#include "big_nums.hpp"
#include "command.h"

template <typename T>
void p(T n){
    //std::cout << n << std::endl;
}

int  main  () {
    
    try{

//    std::cout << "All examples for initiating and using big number class bgnm" << std::endl;
//
//    //temp stuff for examples
//    std::string string1 = "5";
//    std::string string2 = "6";
//    std::string string3 = "11";
//    bgnm bgnm1(7);
//    bgnm bgnm2(8);
//    bgnm bgnm3(12);
//    bgnm bgnm4(25);
//    bgnm pos_ten(10);
//    bgnm neg_ten(-10);
//    bgnm i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,aa,ab,ac,ad,ae,af;
//
//    //OVERLOAD CONSTRUCTOR
//    // constructor w int
//    bgnm a(1.1);
//    // constructor w = and int
//    bgnm b = 2;
//    // constructor w float
//        //  0.00009876543000300405021
//    bgnm b_float((float)66655577744488833399.9222000111);
//    // constructor w double
//    bgnm b_double((double)666555.777444888333999222000111);
//    // constructor w long double
//    long double ld = 66655.577744488833399922200011L;
//    bgnm b_ldouble(ld);
//    // constructor w Cstring
//    bgnm c("3");
//    // constructor w = and Cstring
//    bgnm d = "4";
//    // constructor w string
//    bgnm e(string1);
//    // constructor w = and string
//    bgnm f = string2;
//    // constructor w bgnm
//    bgnm g(bgnm1);
//    // constructor w = and bgnm
//    bgnm h = bgnm2;
//
//    // OVERLOAD = OPERATOR
//    // overload = using int
//    i = 9;
//    // overload = using Cstring
//    j = "10";
//    // overload = using string
//    k = string3;
//    // overload = using bgnm
//    l = bgnm3;
//
//    // OVERLOAD + OPERATOR
//    // overload + using int
//    m = l + 1;
//    // overload + using Cstring
//    n = l + "2";
//    // overload + using string
//    o = j + string1;
//    // overload + using bgnm
//    p = o + a;
//
//    // OVERLOAD - OPERATOR
//    // overload - using int
//    q = bgnm4 - 8;
//    // overload - using Cstring
//    r = bgnm4 - "7";
//    // overload - using string
//    s = bgnm4 - string2;
//    // overload - using bgnm
//    t = bgnm4 - e;
//
//    //test addition and subtraction of positive and negative values
//    // + + +
//    u = pos_ten + "11";
//    // + + -
//    v = pos_ten + "-32";
//    // - + +
//    w = neg_ten + "33";
//    // - + -
//    x = neg_ten + "-14";
//    // + - +
//    y = pos_ten - "35";
//    // + - -
//    z = pos_ten - "-16";
//    // - - +
//    aa = neg_ten - "17";
//    // - - -
//    ab = neg_ten - "-38";
//
//    //OVERLOAD * OPERATOR
//    //overload * using int
//    bgnm yy("12");
//    bgnm zz("-1242.9484");
//    ac = yy * zz;
//
//    std::cout << a << std::endl;
//    std::cout << b << std::endl;
//    std::cout << b_float << std::endl;
//    std::cout << b_double << std::endl;
//    std::cout << b_ldouble << std::endl;
//    std::cout << c << std::endl;
//    std::cout << d << std::endl;
//    std::cout << e << std::endl;
//    std::cout << f << std::endl;
//    std::cout << g << std::endl;
//    std::cout << h << std::endl;
//    std::cout << i << std::endl;
//    std::cout << j << std::endl;
//    std::cout << k << std::endl;
//    std::cout << l << std::endl;
//    std::cout << m << std::endl;
//    std::cout << n << std::endl;
//    std::cout << o << std::endl;
//    std::cout << p << std::endl;
//    std::cout << q << std::endl;
//    std::cout << r << std::endl;
//    std::cout << s << std::endl;
//    std::cout << t << std::endl;
//    std::cout << u << std::endl;
//    std::cout << v << std::endl;
//    std::cout << w << std::endl;
//    std::cout << x << std::endl;
//    std::cout << y << std::endl;
//    std::cout << z << std::endl;
//    std::cout << aa << std::endl;
//    std::cout << ab << std::endl;
//    std::cout << ac << std::endl;
//
//    std::cout << sizeof(float) << std::endl;
//    std::cout << sizeof(double) << std::endl;
//    std::cout << sizeof(long double) << std::endl;
//    std::cout << FLT_EPSILON << std::endl;
//    std::cout << DBL_EPSILON << std::endl;
//    std::cout << LDBL_EPSILON << std::endl;
//    int xxx = 0; double ddd;
//    ddd = frexp(ld, &xxx);
//
//    std::ostringstream bla;
//    bla << std::fixed;
//    bla << std::setprecision(2);
//    bla << pow(2,xxx);
//    std::cout << bla.str() << std::endl;
//    int places = floor(xxx * 0.30102999566); // convert from binary exponent to base ten exponent
//    int precision = 19;
//    int rightofdecimal = precision - places;
//    std::cout << "digits: " << places << std::endl;
//    std::cout << ddd << " - " << xxx << std::endl;
    
//    bgnm s1 = "-161.6900000", s2 = "1723500000.", s3;
//
//    //0.0000000938149115172613867130838410211778357992457209167391935015955903684363214389324049898462431099
////    //0.000000093814911517261386713083840
//    s3 = s1 / s2;
//    std::cout << s3 << '=' << s1 << '/'<<s2<< std::endl;
    
   bgnm b00((float)0.31415926535897932384626433832795028841971);
    bgnm b0((float).31415926535897932384626433832795028841971);
    bgnm b1((float)3.1415926535897932384626433832795028841971);
    bgnm b2((float)314.15926535897932384626433832795028841971);
    bgnm b3((float)31415.926535897932384626433832795028841971);
    bgnm b4((float)3141592.6535897932384626433832795028841971);
    bgnm b5((float)314159265.35897932384626433832795028841971);
    bgnm b6((float)31415926535.897932384626433832795028841971);
    bgnm b7((float)3141592653589.7932384626433832795028841971);
    bgnm b8((float)314159265358979.32384626433832795028841971);
    bgnm b9((float)31415926535897932.384626433832795028841971);
    bgnm b10((float)314159265358979323.84626433832795028841971);
    bgnm b11((float)31415926535897932384.626433832795028841971);
    bgnm b12((float)3141592653589793238462.6433832795028841971);

bgnm d00((double)0.31415926535897932384626433832795028841971);
     bgnm d0((double).31415926535897932384626433832795028841971);
    bgnm d1((double)3.1415926535897932384626433832795028841971);
    bgnm d2((double)314.15926535897932384626433832795028841971);
    bgnm d3((double)31415.926535897932384626433832795028841971);
    bgnm d4((double)3141592.6535897932384626433832795028841971);
    bgnm d5((double)314159265.35897932384626433832795028841971);
    bgnm d6((double)31415926535.897932384626433832795028841971);
    bgnm d7((double)3141592653589.7932384626433832795028841971);
    bgnm d8((double)314159265358979.32384626433832795028841971);
    bgnm d9((double)31415926535897932.384626433832795028841971);
    bgnm d10((double)314159265358979323.84626433832795028841971);
    bgnm d11((double)31415926535897932384.626433832795028841971);
    bgnm d12((double)3141592653589793238462.6433832795028841971);

    bgnm ld00((long double)0.031415926535897932384626433832795028841971);
    bgnm ld0((long double).31415926535897932384626433832795028841971);
    bgnm ld1((long double).00000031415926535897932384626433832795028841971);
    bgnm ld2((long double)314.15926535897932384626433832795028841971);
    bgnm ld3((long double)31415.926535897932384626433832795028841971);
    bgnm ld4((long double)3141592.6535897932384626433832795028841971);
    bgnm ld5((long double)314159265.35897932384626433832795028841971);
    bgnm ld6((long double)31415926535.897932384626433832795028841971);
    bgnm ld7((long double)3141592653589.7932384626433832795028841971);
    bgnm ld8((long double)314159265358979.32384626433832795028841971);
    bgnm ld9((long double)31415926535897932.384626433832795028841971);
    bgnm ld10((long double)314159265358979323.84626433832795028841971);
    bgnm ld11((long double)31415926535897932384.626433832795028841971);
    bgnm ld12((long double)3141592653589793238462.6433832795028841971);
    
    p("3.1415926535897932384626433832795028841971");
    p(b00);
    p(b0);
    p(b1);
    p(b2);
    p(b3);
    p(b4);
    p(b5);
    p(b6);
    p(b7);
    p(b8);
    p(b9);
    p(b10);
    p(b11);
    p(b12);
    
    p("3.1415926535897932384626433832795028841971");
    p(d00);
    p(d0);
    p(d1);
    p(d2);
    p(d3);
    p(d4);
    p(d5);
    p(d6);
    p(d7);
    p(d8);
    p(d9);
    p(d10);
    p(d11);
    p(d12);
    
    p("3.1415926535897932384626433832795028841971");
    p(ld00);
    p(ld0);
    p(ld1);
    p(ld2);
    p(ld3);
    p(ld4);
    p(ld5);
    p(ld6);
    p(ld7);
    p(ld8);
    p(ld9);
    p(ld10);
    p(ld11);
    p(ld12);
    
//    bgnm bn, bn2 = "-9834850322.09820935245";
//    p(ld8);
//    bn = ld8 + (long double)314.1592653589793238462643383;
//    std::cout << bn << std::endl;
//    bn--;
//    std::cout << bn << std::endl;
//    bn++;
//    std::cout << bn << std::endl;
    
    
    
    Bgnm obj(3);
    Bgnm obj1(3);
    p(obj);
    p(obj1);

    // Operator function is called, only then value of obj is assigned to obj1
    obj1 = ++obj;
    p(obj);
    p(obj1);

    // Assigns value of obj to obj1, only then operator function is called.
    obj1 = obj++;
    p(obj);
    p(obj1);
    
    obj++;
    obj1++;
    p(obj);
    p(obj1);
    p(obj++);
    p(obj1++);
    p(obj);
    p(obj1);
    
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
    
    bgnm bg = "-4.004";
    int integer = bg.to_int();
    long long longlong = bg.to_long_long();
    float floating = bg.to_float();
    double doubling = bg.to_double();
    long double longdouble = bg.to_long_double();
    char* cstring = bg.to_c_string();
    std::string stdstring = bg.to_string();
    
    p(integer);
    p(longlong);
    p(floating);
    p(doubling);
    p(longdouble);
    p(cstring);
    p(stdstring);
    p("----------------");
    p(bg.ceil());
    p(bg.floor());

        std::string tempy = "12344";
        Bgnm bgnm100="12345";
        //bgnm100 == "123..23";
        bgnm100 *= tempy;
        p(bgnm100);

            
    } catch (const std::exception & e){
        std::cout << e.what() << std::endl;
    }
    
    
    //------------
    //seting up for getting user input
    
    Command cmd;
    
    try
    {
        cmd.get();
    }
    catch (...)
    {
        
    }
    
    return  0;
}


