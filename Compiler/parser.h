#pragma once
#include <string>

using namespace std;

class Parser
{
public:
	Parser(string& sourceCode);

private:
	string code;
	size_t pos;

	void skipWhitespace();
	string getToken();

};