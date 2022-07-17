/*
opt.hpp

Copyright © 2021 Ben Harvey.
Created by Ben Harvey on 4/11/21.
 
This file is part of Command Parse C++ Library

Command Parse C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Command Parse C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Command C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

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
