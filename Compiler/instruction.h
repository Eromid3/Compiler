#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

struct Instruction {
    virtual ~Instruction() = default;
    virtual void execute(std::unordered_map<std::string, int> variables) = 0;
};

struct AssignmentInstruction : Instruction {
    std::string left;
    std::string right;
    void execute(std::unordered_map<std::string, int> variables) override;
};

struct IfInstruction : Instruction {
    std::string condition;
    std::vector<std::unique_ptr<Instruction>> block;
    void execute(std::unordered_map<std::string, int> variables) override;
};

struct GotoInstruction : Instruction {
    std::string tag;
    void execute(std::unordered_map<std::string, int> variables) override;
};
