/*
opt.cpp

Copyright © 2021 Ben Harvey.
Created by Ben Harvey on 4/11/21.
 
This file is part of Command Parse C++ Library

Command Parse C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Command Parse C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Command C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

#include "opt.h"

Opt::Opt(const char _nickname, const char * _fullname, const char * _notes, const bool _may_have_argument, const bool _argument_required )
{
    nickname           = _nickname;
    fullname           = _fullname;
    notes              = _notes;
    may_have_argument  = _may_have_argument; // if true, parser will look to see if an argument is provided after the nickname/fullname
    argument_required  = _argument_required; // if true, parser will require user to provide argument after nickname or fullname
    simple             = false; // records if an option is a simple option (does not have a nickname or fullname)
    
    if(_may_have_argument == false && _argument_required == true)
        throw "For any command option, you cannot set argument_required to TRUE if _may_have_argument is FALSE.";
    
    if(nickname == '!' && fullname == "")
    {
        simple = true;
        if (_may_have_argument == false) throw "You cannot indicated that a command will have a simple option, but also set option->may_have_argument to FALSE. Review the arguments you are passing to opt::add() in function Com::load().";
    }
    
    if(fullname.find(' ') != std::string::npos)
    {
        throw "Command option fullname is not allowed to contain a space ' ' character. Review the arguments you are passing to opt::add() in function Com::load().";
    }
}

Opt::~Opt(){}

bool Opt::operator < (const Opt& opt)
{
    if (nickname != '!' && opt.nickname != '!')
    {
        return nickname < opt.nickname;
    }
    else if (nickname == '!' && opt.nickname == '!')
    {
        return fullname < opt.fullname;
    }
    else if (nickname == '!' && opt.nickname != '!')
    {
        if(fullname[0] == opt.nickname) return false; // in case nickname and fullname start with same char
        return fullname[0] < opt.nickname;
    }
    else
    {
        if(nickname == opt.fullname[0]) return true; // in case nickname and fullname start with same char
        return nickname < opt.fullname[0];
    }
}



