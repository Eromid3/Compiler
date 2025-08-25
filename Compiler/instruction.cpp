#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "instruction.h"
#include <stack>

using namespace std;

void skipWhitespace(string& line) {
    line.erase(0, line.find_first_not_of(" \t\n\r"));
}

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

string getNextToken(string& expression)
{
    int i = 0;
    while (i < expression.size() && std::isdigit(expression[i]))
    { 
        i++;
    }
    string result = expression.substr(0, i);
    expression.erase(0, i);
    return result;
}

bool isOp(string token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

vector<string> makeRPN(string expression)
{
    skipWhitespace(expression);
    stack<char> ops;
    vector<string> output;
    string token;
    while (expression != "")
    {
        token = getNextToken(expression);
        if (isOp(token))
        {
            int priority = getPriority(token[0]);
            while (!ops.empty())
            {
                char top = ops.top();
                ops.pop();
                if (top == '(' or getPriority(top) < priority)
                {
                    ops.push(top);
                    break;
                }
                output.push_back(string(1, top));
            }
            ops.push(token[0]);
        }
        else if (token == "(") ops.push(token[0]);
        else if (token == ")")
        {
            while (!ops.empty())
            {
                token = ops.top();
                ops.pop();
                if (token == "(") break;
                else output.push_back(token);
            }
        }
        else output.push_back(token);
    }
    while (!ops.empty())
    {
        token = ops.top();
        ops.pop();
        output.push_back(token);
    }

    return output;
}

int evaluateExpression(string expression, std::unordered_map<std::string, int> variables)
{
    vector<string> rpn = makeRPN(expression);
    stack<int> values;
    for (string& token : rpn)
    {
        if (isOp(token))
        {
            if (values.size() < 2) throw runtime_error("Invalid expression");

            int b = values.top(); values.pop();
            int a = values.top(); values.pop();

            int result = 0;
            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else if (token == "/") result = a / b;

            values.push(result);
        }
        else
        {
            int value;
            if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
                value = stoi(token);
            }
            else {
                if (variables.find(token) == variables.end())
                    throw runtime_error("Undefined variable: " + token);
                value = variables[token];
            }
            values.push(value);
        }
    }

    if (values.size() != 1) throw runtime_error("Invalid expression result");

    return values.top();
}

bool evaluateCondition(int leftValue, int rightValue, const std::string& cmpOp) {
    if (cmpOp == "==") return leftValue == rightValue;
    if (cmpOp == "!=") return leftValue != rightValue;
    if (cmpOp == "<")  return leftValue < rightValue;
    if (cmpOp == ">")  return leftValue > rightValue;
    if (cmpOp == "<=") return leftValue <= rightValue;
    if (cmpOp == ">=") return leftValue >= rightValue;
    throw std::runtime_error("Unknown comparison operator: " + cmpOp);
}

void splitCondition(const std::string& condition,
    std::string& leftExpr,
    std::string& rightExpr,
    std::string& cmpOp)
{
    std::string cmpOps[] = { "==", "!=", "<=", ">=", "<", ">" };
    for (const auto& op : cmpOps) {
        size_t pos = condition.find(op);
        if (pos != std::string::npos) {
            cmpOp = op;
            leftExpr = condition.substr(0, pos);
            rightExpr = condition.substr(pos + op.size());
            return;
        }
    }
    throw std::runtime_error("Invalid condition: " + condition);
}



void AssignmentInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions) 
{
    size_t pos = expr.find('=');
    if (pos != std::string::npos) {
        std::string left = expr.substr(0, pos);
        std::string right = expr.substr(pos + 1);
        int value = evaluateExpression(right, variables);
        variables[left] = value;
        instructionPointer++;
    }
}

void IfInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{
    string leftExpr, rightExpr, cmpOp;
    std::vector<std::unique_ptr<Instruction>>* correctBlock = nullptr;
    for (auto& block : blocks)
    {
        if (block.condition != "")
        {
            splitCondition(block.condition, leftExpr, rightExpr, cmpOp);

            int leftValue = evaluateExpression(leftExpr, variables);
            int rightValue = evaluateExpression(rightExpr, variables);


            if (!evaluateCondition(leftValue, rightValue, cmpOp)) continue;
        }
        correctBlock = &block.block;
        break;
    }
    for (auto& instr : *correctBlock) {
        instr->execute(variables, instructionPointer, labelPositions);
    }
    instructionPointer++;
}

void GotoInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{  
    auto tagPos = labelPositions.find(tag);
    if (tagPos == labelPositions.end())
        throw std::runtime_error("Unknown label: " + tag);

    instructionPointer = tagPos->second + 1;
}

void SaveTagInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{
    labelPositions[tag.substr(1)] = instructionPointer;
}