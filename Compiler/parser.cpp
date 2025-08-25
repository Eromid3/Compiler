#pragma once
#include "parser.h"
#include <string>
#include <iostream>

ConditionBlock findBlock(std::string conditon)
{

}

std::unique_ptr<Instruction> parseLine(const std::string& line)
{

    if (line.substr(0, 5) == "goto ") {
        auto instr = std::make_unique<GotoInstruction>();
        instr->tag = line.substr(5); // after "goto "
        return instr;
    }
    else if (!line.empty() && line[0] == '#') {
        auto instr = std::make_unique<SaveTagInstruction>();
        instr->tag = line;
        return instr;
    }
    else if (line.find('=') != std::string::npos) {
        auto instr = std::make_unique<AssignmentInstruction>();
        instr->expr = line;
        return instr;
    }
    else if (line.substr(0, 2) == "if" || line.substr(0, 4) == "elif" || line.substr(0, 4) == "else") {
        auto instr = std::make_unique<IfInstruction>();
        size_t start = line.find("(");
        size_t end = line.find(")");
        std::string condition = (start == std::string::npos) ? "" : line.substr(start + 1, end - 1);
        instr->blocks.push_back(findBlock(condition));

    }
    else {
        throw std::runtime_error("Unknown instruction: " + line);
    }
}


std::vector<std::unique_ptr<Instruction>> parseProgram(const std::vector<std::string>& lines)
{
	std::vector<std::unique_ptr<Instruction>> program;

	for (const auto& line : lines) {
		if (line.empty()) continue;
		auto instr = parseLine(line);
		if (instr) {
			program.push_back(std::move(instr));
		}
	}

	return program;
}