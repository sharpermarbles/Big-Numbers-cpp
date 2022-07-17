/*
command.cpp

Copyright © 2021 Ben Harvey.
Created by Ben Harvey on 4/11/21.
 
This file is part of Command Parse C++ Library

Command Parse C++ Library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3 as published by the Free Software Foundation.

Command Parse C++ Library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License version 3 along with Command C++ Library. If not, see <https://www.gnu.org/licenses/>.
*/

#include "command.h"


bool              Command::keep_going = true;
std::string       Command::welcome_msg = "";
std::string       Command::exit_msg = "";
std::vector<Com*> Command::coms;

Command::Command(){}

Command::~Command()
{
    for(int i = 0; i < coms.size(); i++) delete coms[i];
}

void Command::help(Parsed const * const parsed)
{
    std::cout << std::endl << "  All commands and their options: " << std::endl << std::endl;
    for(int i = 0; i < Command::get_command_count(); i++)
    {
        Com* ptr = Command::get_com_ptr(i);
        if(ptr != NULL) ptr->print();
    }
}

void Command::exit( Parsed const * const parsed)
{
    Command::set_keep_going(false);
    std::cout << exit_msg << std::endl;
}

static std::string clean_string( const std::string & str) // removes leading spaces
{
    const auto strBegin = str.find_first_not_of(' ');
    if (strBegin == std::string::npos) return ""; // no content
    return str.substr(strBegin);
}

// checks to be sure that there is at least one more quote ( ' or " ) after the first char in a string
static bool closed_quotes(const std::string & s, const char & c)
{
    for (int i = 1; i < s.size(); i++)
    {
        if (s[i] == c) return true;
    }
    return false;
}

static std::string get_argument(std::string & str, int & from_the_right)
{
    if(str.size() < 2 ) return "";
    else
    {
        // if the next char is not a space, then obviously no argument is provided (the next char is likely another nickname)
        if(str[1] != ' ')
        {
            from_the_right = (int)str.size()-1;
            return "";
        }
        else
        {
            std::string temp = str.substr(1);
            temp = clean_string(temp); // first get rid of extra spaces if there are more than one
            // is there anything left in the string to check?
            if(temp.size() < 1) return "";
            else
            {
                // is the next char either a ' or a "? if so, get everything between the two (assuming there are actually two quotes)
                if( (temp[0] == '\'' && closed_quotes(temp,'\'') ) || (temp[0] == '"' && closed_quotes(temp,'"')))
                {
                    char delimit;
                    temp[0] == '\'' ? delimit = '\'' : delimit = '"';
                    std::string found_text = "";
                    auto start_position = temp.find(delimit);
                    ++start_position; // start after first delimiter.
                    auto end_position = start_position;
                    for(; end_position < temp.size(); end_position++)
                    {
                        if(end_position + 1 < temp.size())
                        {
                            // take into account escape character '/'
                            if(temp[end_position] == '\\' && temp[end_position + 1] == delimit){
                                temp.erase(end_position,1);
                                continue;
                            }
                        }
                        if(temp[end_position] == delimit) break;
                    }
                    found_text = temp.substr(start_position, end_position - start_position);
                    from_the_right = (int)(temp.size() - (start_position + end_position));
                    return found_text;
                }
                // otherwise return substring up until the next appearance of a " -" (space then dash)
                else
                {
                    // find location of next nickname or fullname if there is one
                    int loc;
                    if(temp[0] == '-') loc = 0; // (for when string begins with option, but no space)
                    else
                    {
                        loc = (int)temp.find(" -");
                    }
                    // if there is no such instance, then return entire string
                    if(loc == std::string::npos) return temp;
                    // otherwise return string up until instance
                    else
                    {
                        from_the_right = (int)(temp.size() - loc);
                        return temp.substr(0,loc);
                    }
                }
            }
        }
    }
}

/* Command::parser() parses the input_string and creates a vector 'parsed' of objects that the custom function can inspect. If the parser determines that the provided input string matches one or more of the command's qualified options, it puts that information in the 'parsed' object. Any incorrect or extraneous information found in the input string by the parser will send an error message back to the user
 */
bool Command::
parser( Com const * const com, Parsed* const parsed, std::string input_string, std::string & err_msg )
{
    // does this command have options?
    if(com->options.size() > 0)
    {
        // does command have a simple option?
        if(com->options[0]->simple)
        {
            // get the argument/input-string if there is one
            int trashvariable;
            std::string arg = get_argument(input_string.insert(0, 1, ' '),trashvariable);
            // is an argument required?
            if(com->options[0]->argument_required)
            {
                // if yes, check to see if argument provided
                if(arg.size() > 0)
                {
                    // if it was, then add the input string as a  to the parsed->submitteds vector
                    parsed->add(com->options[0],arg);
                }
                // else if not, then tell user they messed up
                else
                {
                    err_msg = "Option requires argument. None provided.";
                    return false;
                }
            }
            // if argument is not required, then...
            else
            {
                // check to see if something was provided
                if(arg.size() > 0)
                    // if it was, then add the input string as a argument to the parsed->submitteds vector
                    parsed->add(com->options[0],arg);
                // else if nothing was provided, no big deal, proceed
            }
        }
        // else it has one or more normal options (nicknames and/or fullnames)
        else
        {
            // keep processing string, adding to Parsed object, checking for errors, until nothing is left in string
            while(input_string.size() > 0)
            {
                // get rid of leading spaces
                input_string = clean_string(input_string);
                if(input_string.size() > 1)
                {
                    // is it a single or double dash
                    // if single dash...
                    if(input_string[0] == '-' && input_string[1] != '-')
                    {
                        // go ahead and get rid of dash
                        input_string = input_string.substr(1);
                        // loop until chars aren't between a and Z anymore
                        while((input_string[0] >= 'a' && input_string[0] <= 'z') || (input_string[0] >= 'A' && input_string[0] <= 'Z'))
                        {
                            // check to see if char matches an option nickname
                            Opt * opt = com->check_nickname(input_string[0]);
                            if(opt == NULL)
                            {
                                std::string optname = {'-',input_string[0]};
                                err_msg = "Option "; err_msg.append(optname); err_msg.append(" does not exist.");
                                return false; // if NULL, that means nickname didn't match actual option
                            }
                            else
                            {
                                // check to see if the option may take an argument
                                if(opt->may_have_argument)
                                {
                                    // parse next section of string to see if argument is provided following nickname
                                    int from_the_right = 0;
                                    std::string arg = get_argument(input_string,from_the_right);
                                    // check to see if argument is required
                                    if(opt->argument_required && arg == "")
                                    {
                                        std::string optname = {'-',opt->nickname};
                                        err_msg = "Option "; err_msg.append(optname); err_msg.append(" reqiures an argument. None provided.");
                                        return false;
                                    }
                                    // if everythign looks good, add Submitted object to submitteds vector
                                    parsed->add(opt,arg);
                                    // delete everything you just parsed and dealt with
                                    input_string = input_string.substr(input_string.size() - from_the_right);
                                }
                                else // it's just a simple nickname, no arguments
                                {
                                    // if an argument was provided, then issue an error
                                    int from_the_right = 0;
                                    std::string arg = get_argument(input_string,from_the_right);
                                    if(arg.size() > 0)
                                    {
                                        std::string optname = {'-',opt->nickname};
                                        err_msg = "Option "; err_msg.append(optname); err_msg.append(" does not take an argument.");
                                        return false;
                                    }
                                    parsed->add(opt,"");
                                    // delete the nickname from input_string and move on
                                    input_string = input_string.substr(1);
                                }
                            }
                        }
                    }
                    // if double dash (fullname)...
                    else if(input_string[0] == '-' && input_string[1] == '-')
                    {
                        // is string too short?
                        if (input_string.size() < 4)
                        {
                            err_msg = "Option "; err_msg.append(input_string); err_msg.append(" is not a valid option.");
                            return false;
                        }
                        // go ahead and get rid of double dashes
                        input_string = input_string.substr(2);
                        // grab the next section of text before space or end of string
                        auto len = input_string.find(' ');
                        if (len == std::string::npos)
                        {
                            len = input_string.size();
                        }
                        std::string fullname = input_string.substr(0,len);
                        //go ahead and delete fullname from begining of input_string
                        input_string = input_string.substr(len);
                        input_string.insert (0, 1, ' ');
                        // check to see if string matches an option fullname
                        Opt * opt = com->check_fullname(fullname);
                        if(opt == NULL)
                        {
                            err_msg = "Option --"; err_msg.append(fullname); err_msg.append(" is not a valid option.");
                            return false;
                        }
                        // parse next section of string to see if argument is provided following option fullname
                        int from_the_right = 0;
                        std::string arg = get_argument(input_string,from_the_right);
                        if(opt->may_have_argument)
                        {
                            // check to see if argument is required
                            if(opt->argument_required && arg == "")
                            {
                                err_msg = "Option --"; err_msg.append(opt->fullname); err_msg.append(" requires an argument. None provided.");
                                return false;
                            }
                            // if everythign looks good, add Submitted object to submitteds vector
                            parsed->add(opt,arg);
                            // delete everything you just parsed and dealt with
                            input_string = input_string.substr(input_string.size() - from_the_right);
                        }
                        else
                        {
                            // since there isn't supposed to be an argument, then make sure there isn't one, and submit the option
                            if(arg.size() > 0)
                            {
                                err_msg = "Option --"; err_msg.append(opt->fullname); err_msg.append(" does not take an argument.");
                                return false;
                            }
                            else
                            {
                                parsed->add(opt,arg);
                            }
                        }
                    }
                    // if something else, there's a problem, need to alert user
                    else
                    {
                        // how to tell the user they entered bad input
                        err_msg = "Please try again.";
                        return false;
                    }
                }
            }
        }
    }
    // if doesn't have options, then tell user they messed up if anything was typed in after the command name (other than extra spaces).
    else
    {
        if(input_string.find_first_not_of(' ') != std::string::npos)
        {
            err_msg = "This command does not take any options or arguments.";
            return false;
        }
    }
    return true;
}

void Command::load(std::vector<Com*> * const com_vec)
{
    
    // load built-int commands
    Com::add_com(com_vec, new Com(
        help,
        "help",
        "List all possible user commands."
        ));
    Com::add_com(com_vec, new Com(
        exit,
        "exit",
        "Terminate program."
        ));
    // load custom functions
    Com::load(com_vec);
    
    // load welcome and exit messages
    set_welcome_msg(Com::welcome_msg);
    set_exit_msg(Com::exit_msg);
    
    //sort commands alphabetically
    std::sort(com_vec->begin(),com_vec->end(),[](Com* a, Com* b){return *a < *b;});
}

// function to get and process user input commands - loops forever until Command::keep_going set to false
void Command::main_input_loop()
{
    // first time loop is run (load commands and print welcome message)
    if(coms.size() == 0)
    {
        load(&coms);
        std::cout << "\n" << welcome_msg << "\n\n";
    }
    
	std::string s, opts = "";
    bool not_found = false;

    std::cout << "Enter command: ";
	getline(std::cin, s);
    
    // remove spaces from beginning of input string if any
    while (s[0] == ' ') s.erase(0,1);
    // get first word of entered string (to differentiate from any options)
    unsigned long space_loc = s.find(' ');
    if(space_loc != std::string::npos) {
        // substring containing options (if any)
        opts = s.substr(space_loc);
        // substring containing the command itself
        s    = s.substr(0,space_loc);
    }
    
    // search input string for match against all loaded commands
	for (int i = 0; i < (int)coms.size(); i++)
    {
		if (s == coms[i]->name)
        {
            // first check to see if user asked for "--help"
            if(opts.find(" --help")!=std::string::npos)
            {
                //print help message for this command
                std::cout << std::endl;
                coms[i]->print();
                break;
            }
            // having found a match, create a Parsed object that will contain information about parsed options entered by the user for this command
            Parsed* parsed = new Parsed();
            // if inputed options (opts) can be parsed, then go ahead, otherwise if false, break out
            std::string err_msg = "";
            if(!parser(coms[i],parsed,opts,err_msg))
            {
                // upon failure, output error message to user
                std::cout << "    Invalid input for command '" << coms[i]->name << "'." << " " << err_msg << std::endl ;
                break;
            }
            // if parsed() was succesful, then run actual function based on user-inputted command/options
            coms[i]->run(parsed);
            delete parsed;
			break;
		}
        // if no matching command was found, break out of loop
        if (i == (int)coms.size() - 1)
        {
            not_found = true;
            break;
        }
        //std::cout << std::endl;
	}
    
    if(not_found) std::cout << "    Not a command. Please try again or type 'help'." << std::endl << std::endl;
    // continue user input loop, or exit program
	if(keep_going) main_input_loop();
}

void Command::set_keep_going(bool b)
{
    keep_going = b;
}

void Command::set_welcome_msg(std::string & s)
{
    welcome_msg = s;
}

void Command::set_exit_msg(std::string & s)
{
    exit_msg = s;
}

int Command::get_command_count()
{
    return (int)coms.size();
}

Com* Command::get_com_ptr(int i)
{
    if(i > -1 && i < coms.size())
    {
        return  coms[i];
    }
    else
    {
        return NULL;
    }
}
