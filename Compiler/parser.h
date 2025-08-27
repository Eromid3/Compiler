#pragma once
#include <string>
#include <vector>
#include <memory>
#include "instruction.h"

// Removes leading spaces, tabs, and newline characters
std::string removeFirstWhiteSpaces(std::string line);

// Finds a block of instructions between { and }, with a given condition
ConditionBlock findBlock(const std::vector<std::string>& lines, size_t& i, const std::string& condition);

// Parses a single line of code and returns the corresponding instruction
std::unique_ptr<Instruction> parseLine(const std::string& line);

// Parses the entire program (vector of lines) and returns a vector of instructions
std::vector<std::unique_ptr<Instruction>> parseProgram(const std::vector<std::string>& lines);
