/*
parsed.cpp

Copyright © 2021 Ben Harvey.
Created by Ben Harvey on 4/11/21.
 
This file is part of Command Parse C++ Library

Command Parse C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Command Parse C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Command C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

#include "parsed.h"
#include "opt.h"

Submitted::Submitted(const Opt * const _opt, const std::string& _argument)
    : opt(_opt), argument(_argument)
{
}

Submitted::~Submitted(){}

Parsed::Parsed()
{
}

Parsed::~Parsed()
{
    for(int i = 0; i < submitteds.size(); i++) delete submitteds[i];
}

void Parsed::add(const Opt * const _opt, const std::string& _argument)
{
    Submitted * sub = new Submitted(_opt,_argument);
    submitteds.push_back(sub);
}

#define OVERLOAD_ERROR "Using incorrect overload of Parsed::check() method for this option for this command. Check the options for this command and select appropriate overload of method Parsed::check().\n"

std::string Parsed::check() const
{
    if(this->submitteds.size() == 1)
    {
        if(submitteds[0]->opt->simple)
        {
            return submitteds[0]->argument;
        }
        else throw OVERLOAD_ERROR;
    }
    else if (this->submitteds.size() > 1) throw OVERLOAD_ERROR; // this version of check() should not be used for commands with multiple options
    return "";
}

bool Parsed::check(const char& nickname ) const
{
    bool ret = false;
    for(int i = 0; i < submitteds.size(); i++ )
        if (submitteds[i]->opt->nickname == nickname)
        {
            if(submitteds[i]->opt->may_have_argument) throw OVERLOAD_ERROR;
            ret = true;
        }
    return ret;
}

bool Parsed::check(const char& nickname, std::string & arg) const
{
    bool ret = false;
    for(int i = 0; i < submitteds.size(); i++ )
    {
        if (submitteds[i]->opt->nickname == nickname)
        {
            if(submitteds[i]->opt->may_have_argument == false) throw OVERLOAD_ERROR;
            ret = true;
            arg = submitteds[i]->argument;
        }
    }
    return ret;
}

bool Parsed::check(const std::string& fullname ) const
{
    bool ret = false;
    for(int i = 0; i < submitteds.size(); i++ )
        if (submitteds[i]->opt->fullname == fullname)
        {
            if(submitteds[i]->opt->may_have_argument) throw OVERLOAD_ERROR;
            ret = true;
        }
    return ret;
}

bool Parsed::check(const std::string& fullname, std::string & arg) const
{
    bool ret = false;
    for(int i = 0; i < submitteds.size(); i++ )
    {
        if (submitteds[i]->opt->fullname == fullname)
        {
            if(submitteds[i]->opt->may_have_argument == false) throw OVERLOAD_ERROR;
            ret = true;
            arg = submitteds[i]->argument;
        }
    }
    return ret;
}
