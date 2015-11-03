// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __OPTION_H
#define __OPTION_H

#include <string>

char* get_option(char** begin, char** end, const std::string& option);
bool option_exists(char** begin, char** end, const std::string& option);

std::string load_from_file(std::string filename);

#endif
