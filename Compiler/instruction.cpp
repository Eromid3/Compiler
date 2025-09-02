#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "instruction.h"
#include <stack>
#include "parser.h"

void skipWhitespace(std::string& line) {
    std::string result;
    for (char c : line) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            result += c; // dodajemy tylko znaki, które nie s¹ bia³e
        }
    }
    line = result;
}

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool isOp(std::string token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

std::string getNextToken(std::string& expression)
{
    int i = 0;
   
    if (isOp(std::string(1, expression[i])) || expression[i] == '(' || expression[i] == ')') {
        std::string result = std::string(1, expression[i]);
        expression.erase(0, 1);
        return result;
    }
    else
    {
        while (i < expression.size() && !isOp(std::string(1, expression[i])) && expression[i] != '(' && expression[i] != ')' && !isspace(expression[i])) i++;
        std::string result = expression.substr(0, i);
        expression.erase(0, i);
        return result;
    }
}

std::vector<std::string> makeRPN(std::string expression)
{
    skipWhitespace(expression);
    std::stack<char> ops;
    std::vector<std::string> output;
    std::string token;
    while (!expression.empty())
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
                output.push_back(std::string(1, top));
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

int evaluateExpression(std::string expression, std::unordered_map<std::string, int> variables)
{
    std::vector<std::string> rpn = makeRPN(expression);
    std::stack<int> values;
    for (std::string& token : rpn)
    {
        if (isOp(token))
        {
            if (values.size() < 2) throw std::runtime_error("Invalid expression");

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
                    throw std::runtime_error("Undefined variable: " + token);
                value = variables[token];
            }
            values.push(value);
        }
    }

    if (values.size() != 1) throw std::runtime_error("Invalid expression result");

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
        skipWhitespace(left);
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
    std::string leftExpr, rightExpr, cmpOp;
    std::vector<std::string> correctBlock;
    for (auto& block : blocks)
    {
        if (!block.condition.empty()) {
            std::string leftExpr, rightExpr, cmpOp;
            splitCondition(block.condition, leftExpr, rightExpr, cmpOp);

            int leftValue = evaluateExpression(leftExpr, variables);
            int rightValue = evaluateExpression(rightExpr, variables);

            if (!evaluateCondition(leftValue, rightValue, cmpOp))
                continue;
        }
        auto blockProgram = parseProgram(block.block);
        size_t before = instructionPointer;
        for (auto& instr : blockProgram) {
            instr->execute(variables, instructionPointer, labelPositions);
            if (instructionPointer != before + 1) return;
            else instructionPointer--;
        }
        break;
    }
    instructionPointer++; // if with elif, else are counted as one instruction
}

void GotoInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{  
    auto tagPos = labelPositions.find(tag);
    if (tagPos == labelPositions.end())
        throw std::runtime_error("Unknown label: " + tag);

    instructionPointer = tagPos->second;
}

void SaveTagInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{
    labelPositions[tag] = instructionPointer;
    instructionPointer++;
}

void PrintInstruction::execute(
    std::unordered_map<std::string, int>& variables,
    size_t& instructionPointer,
    std::unordered_map<std::string, size_t>& labelPositions)
{
    if (variable) std::cout << variables[text];
    else std::cout << text;
    instructionPointer++;
}