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

int evaluateExpression(string expression, std::unordered_map<std::string, int> variables)
{
    skipWhitespace(expression);
    stack<char> ops;
    vector<string> output;
    string token;
    while (expression != "")
    {
        token = getNextToken(expression);
    }

}

void AssignmentInstruction::execute(std::unordered_map<std::string, int> variables)
{
	int value = stoi(right);
	variables[left] = value;
}

void IfInstruction::execute(std::unordered_map<std::string, int> variables)
{

}

void GotoInstruction::execute(std::unordered_map<std::string, int> variables)
{

}