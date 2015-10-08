// Luke Mitchell, 2015
// luke.mitchell.2011@my.bristol.ac.uk

#ifndef __COMMON_H
#define __COMMON_H

#include <cstdint>
#include <string>

#define STRING(x) #x

int strtoi(std::string str);

std::string removeWhitespace(std::string str);
std::string removeCharacter(std::string str, char chr);

std::string hexify(uint32_t data);

std::string optos(uint8_t opcode);
uint8_t stoop(std::string opcode);

std::string rtos(uint8_t register);
uint8_t stor(std::string register);

#endif
