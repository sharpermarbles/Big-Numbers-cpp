//
//  coms.hpp
//  command
//
//  Created by Ben Harvey on 4/11/21.
//

#ifndef _com_h_
#define _com_h_

#include <iostream>
#include <string>
#include <vector>
#include "opt.h"
#include "parsed.h"

class Parsed;

class Com
{
    friend class Command;
    
    typedef void (*func_type)(Parsed const * const);
    func_type         func;
    
    std::string       name;
    std::string       notes;
    std::vector<Opt*> options;
    
    static std::string welcome_msg;
    static std::string exit_msg;
    
    static void load(std::vector<Com*> * const vec);
    static void add_com(std::vector<Com*> * const vec, Com * const com);
    
    
    void run(Parsed const * const parsed) const;
    void add_opt(Opt* const opt);
    void print();
    Opt* check_nickname(const char) const;
    Opt* check_fullname(const std::string &) const;
    
    Com(const func_type func, const std::string &name, const std::string &notes);
    ~Com();
    
    bool operator < (const Com& com);
    Opt* get_opt_ptr(const int i) ;
        
};



#endif /* _com_h_ */
