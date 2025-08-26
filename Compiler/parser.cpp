#pragma once
#include "parser.h"
#include <string>
#include <iostream>


std::string removeFirstWhiteSpaces(const std::string& line) {
    size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r' || line[i] == '\n')) {
        i++;
    }
    return line.substr(i);
}

ConditionBlock findBlock(const std::vector<std::string>& lines, size_t& i, const std::string& condition)
{
    ConditionBlock block;
    block.condition = condition;

    if (removeFirstWhiteSpaces(lines[i]).find('{') == std::string::npos) {
        i++;
    }

    int braceCount = 0;
    for (; i < lines.size(); i++) {
        std::string line = removeFirstWhiteSpaces(lines[i]);
        if (line.find('{') != std::string::npos) {
            braceCount++;
            continue;
        }
        else if (line.find('}') != std::string::npos) {
            braceCount--;
            if (braceCount == 0) break;
            continue;
        }
        else {
            block.block.push_back(line);
        }
        block.block.push_back(line);
    }

    return block;
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
        instr->tag = line.substr(1);
        return instr;
    }
    else if (line.find('=') != std::string::npos) {
        auto instr = std::make_unique<AssignmentInstruction>();
        instr->expr = line;
        return instr;
    }
    else {
        throw std::runtime_error("Unknown instruction: " + line);
    }
}


std::vector<std::unique_ptr<Instruction>> parseProgram(const std::vector<std::string>& lines)
{
	std::vector<std::unique_ptr<Instruction>> program;

    for (size_t i = 0; i < lines.size(); i++) {
        std::string line = removeFirstWhiteSpaces(lines[i]);
        if (line.empty()) continue;
        if (line.substr(0, 2) == "if" || line.substr(0, 4) == "elif" || line.substr(0, 4) == "else") {
            auto instr = std::make_unique<IfInstruction>();
            size_t start = line.find("(");
            size_t end = line.find(")");
            std::string condition = (start == std::string::npos) ? "" : line.substr(start + 1, end - start - 1);
            instr->blocks.push_back(findBlock(lines, i, condition));
            program.push_back(std::move(instr));
        }
        else {
			program.push_back(std::move(parseLine(line)));
		}
	}

	return program;
}