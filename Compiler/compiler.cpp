#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "parser.h"
#include "instruction.h"

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

int main() {
    try {
        auto lines = readFileLines("main.txt");
        auto program = parseProgram(lines);

        // test: wypisz wszystkie linie w pierwszym AssignmentInstruction
        for (auto& instr : program) {
            if (auto assign = dynamic_cast<AssignmentInstruction*>(instr.get())) {
                std::cout << "Assignment: " << assign->expr << "\n";
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
