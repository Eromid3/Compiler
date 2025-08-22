#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        const std::unordered_map<std::string, size_t>& labelPositions) = 0;
};

struct AssignmentInstruction : Instruction {
    std::string left;
    std::string right;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        const std::unordered_map<std::string, size_t>& labelPositions) override;
};


struct ConditionBlock {
    std::string condition; // empty string = else
    std::vector<std::unique_ptr<Instruction>> block;
};

struct IfInstruction : Instruction {
    std::string condition;
    std::vector<ConditionBlock> blocks;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        const std::unordered_map<std::string, size_t>& labelPositions) override;
};

struct GotoInstruction : Instruction {
    std::string tag;
    void execute(std::unordered_map<std::string, int>& variables,
        size_t& instructionPointer,
        const std::unordered_map<std::string, size_t>& labelPositions) override;
};