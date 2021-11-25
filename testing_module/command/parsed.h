//
//  Parsed.hpp
//  command
//
//  Created by Ben Harvey on 4/25/21.
//

#ifndef parsed_hpp
#define parsed_hpp

#include <string>
#include <vector>
#include <stdio.h>

class Opt;

class Submitted
{
    friend class Parsed;
    friend class Command;
    
    const Opt* opt;
    const std::string argument;
    
    Submitted(const Opt * const _opt, const std::string& _argument);
    ~Submitted();
    
};

class Parsed
{
    
    friend class Command;
    friend class Com;
    
    //Com * com;
    std::vector<Submitted*> submitteds;
    
    void add(const Opt * const, const std::string&);
    
    Parsed();
    ~Parsed();
    
public:
    
    // if command has a 'simple' option, use this method (in other words, no nicknamed or fullname options)
    std::string check() const;  // if simple option (argument), put parsed argument into string
    
    // use to check option with nickname that does not expect an argument
    bool check(const char& nickname) const;  // for nickname, return true or false

    // use to check option with nickname and that should expect an argument
    bool check(const char& nickname, std::string & arg) const;  // for nickname, return true or false, pass parsed input if true

    // use to check option with fullname and that should not expect an argument
    bool check(const std::string& fullname) const;  // for fullname, return true or false
    
    // use to check option with fullname that should expect argument
    bool check(const std::string& fullname, std::string & arg) const;  // for fullname, return true or false, pass parsed input if true
    
};

#endif /* parsed_hpp */
