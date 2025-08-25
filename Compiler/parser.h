#pragma once
#include "instruction.h"

class Parser
{
public:
	Parser(std::string& sourceCode);

private:
	std::string code;

	std::vector<std::unique_ptr<Instruction>> parseProgram(const std::vector<std::string>& lines);
	std::unique_ptr<Instruction> parseLine(const std::string& line);


};