#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <iostream>
#include "parser.h"

std::vector<std::string> readFileLines(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

void printVariables(const std::unordered_map<std::string, int>& variables) {
    std::cout << "=== VARIABLES ===\n";
    for (const auto& kv : variables) {
        std::cout << kv.first << " = " << kv.second << "\n";
    }
    std::cout << "=================\n";
}


int main() {
    try {
        auto lines = readFileLines("main.txt");
        auto program = parseProgram(lines);

        std::unordered_map<std::string, int> variables;
        size_t instructionPointer = 0;
        std::unordered_map<std::string, size_t> labelPositions;

        // wykonujemy instrukcje
        while (instructionPointer < program.size()) {
            program[instructionPointer]->execute(variables, instructionPointer, labelPositions);
        }

        //printVariables(variables);

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
