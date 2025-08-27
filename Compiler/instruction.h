#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) = 0;
};

struct AssignmentInstruction : Instruction {
    std::string expr;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) override;
};


struct ConditionBlock {
    std::string condition; // empty string = else
    std::vector<std::string> block;
};

struct IfInstruction : Instruction {
    std::vector<ConditionBlock> blocks;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) override;
};

struct GotoInstruction : Instruction {
    std::string tag;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) override;
};

struct SaveTagInstruction : Instruction {
    std::string tag;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) override;
};

struct PrintInstruction : Instruction {
    std::string text;
    bool variable = false;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        std::unordered_map<std::string, size_t>& labelPositions) override;
};