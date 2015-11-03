// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#include <algorithm> 

#include "option.h"

// Retrieve the value of a specified command-line option
// iain at StackOverflow.com
char* get_option(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

// Returns true is a command-line option exists
// iain at StackOverflow.com
bool option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}
