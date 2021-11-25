//
//  Option.hpp
//  command
//
//  Created by Ben Harvey on 4/11/21.
//

#ifndef _opt_h_
#define _opt_h_

#include <string>

class Com;
class Command;
class Parsed;

class Opt
{
    friend class Com;
    friend class Command;
    friend class Parsed;
    
    char         nickname;
    std::string  fullname;
    bool         may_have_argument;
    bool         argument_required;
    bool         simple;
    std::string  notes;
    
public:
    
    Opt(const char   _nickname,
        const char * _fullname,
        const char * _notes,
        const bool   _may_have_argument = false,
        const bool   _argument_required = false);
    ~Opt();
    
    bool operator < (const Opt& opt);
    
};

#endif /* _opt_h_ */
